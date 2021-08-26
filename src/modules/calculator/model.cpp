/*
 * Copyright (C) Mark de Wever <koraq@xs4all.nl>
 * Part of the RPN project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY.
 *
 * See the COPYING file for more details.
 */

export module calculator.model;

export import calculator.value;

import<charconv>;
import<concepts>;
import<format>;
import<string>;
import<string_view>;
export import<vector>;

namespace calculator {

/** The supported bases in the calculator. */
export enum class tbase { binary, octal, decimal, hexadecimal };

/**
 * The model of the calculator.
 *
 * For now a very basic version of this class.
 *
 * The class is non-copyable and non-movable. These operations make little
 * sense for the model and are most likely logic errors.
 */
export class tmodel final {
public:
  // *** SMF ***

  tmodel() = default;
  tmodel(const tmodel &) = delete;
  tmodel(tmodel &&) = delete;
  ~tmodel() = default;

  tmodel &operator=(const tmodel &) = delete;
  tmodel &operator=(tmodel &&) = delete;

  // *** Query ***

  [[nodiscard]] bool stack_empty() const noexcept { return stack_.empty(); }
  [[nodiscard]] size_t stack_size() const noexcept { return stack_.size(); }
  [[nodiscard]] const std::vector<std::string> &stack() const noexcept {
    synchronise_display();
    return display_;
  }

  // *** Modifiers ***

  // * Diagnostics *

  void diagnostics_set(std::string &&diagnotics) {
    diagnotics_ = std::move(diagnotics);
  }

  [[nodiscard]] const std::string &diagnostics_get() const noexcept {
    return diagnotics_;
  }

  /** Clears the current diagnostics. */
  void diagnostics_clear() noexcept { diagnotics_.clear(); }

  // * Stack *

  /** Adds the @p value to the back of the stack. */
  void stack_push(tvalue value) {
    stack_.emplace_back(std::move(value));
    display_.emplace_back();
    dirty_ = true;
  }

  /**
   * @returns The last element at the back of the stack.
   * @throws @ref std::out_of_range when the stack is empty.
   */
  [[nodiscard]] tvalue stack_pop();

  /**
   * Removes the last element at the back of the stack
   * @throws @ref std::out_of_range when the stack is empty.
   */
  void stack_drop();

  // * Input *

  /**
   * Clears the current input.
   *
   * TODO Is this function still needed?
   */
  void input_clear() noexcept { input_.clear(); }

  /** Returns the clears the input and returns the original value. */
  [[nodiscard]] std::string input_steal() {
    std::string result;
    std::swap(input_, result);
    return result;
  }

  /**
   * Appends the @p data to the @ref input_. @note Since the expected usage
   * pattern is that the user only append (and maybe later removes) input
   * there's no setter required.
   */
  void input_append(char data) { input_.push_back(data); }
  void input_append(std::string_view data) { input_.append(data); }

  /**
   * Removes the last character of the @ref input_.
   *
   * @returns Whether an element was removed.
   */
  [[nodiscard]] bool input_pop_back() {
    if (input_.empty())
      return false;
    input_.pop_back();
    return true;
  }

  [[nodiscard]] const std::string &input_get() const noexcept { return input_; }

  // * Base *

  void base_set(tbase base) {
    base_ = base;
    // clear the cached strings.
    std::for_each(display_.begin(), display_.end(),
                  [](std::string &string) { string.clear(); });
    dirty_ = true;
  }

private:
  /** The execution issues to report to the user. */
  std::string diagnotics_{};

  /**
   * The stack with all values of the applications.
   *
   * The usage pattern is a LIFO. Since @ref std::vector operates faster on the
   * the back new elements are added to the back. So the first item is the
   * oldest item.
   */
  std::vector<tvalue> stack_{};

  /**
   * The shadow stack with values rendered as string.
   *
   * The size always matches the size of @ref stack_. The display is a cache of
   * the rendered value. When it contains an empty string it's contents are out
   * of sync. When invalidating a cache entry the @ref dirty_ flag must be set.
   */
  mutable std::vector<std::string> display_{};

  /** Is the display in sync with the values? */
  mutable bool dirty_{false};

  void synchronise_display() const;

  [[nodiscard]] std::string format(const tvalue &value) const;

  /** The input buffer used to store the current editting session. */
  std::string input_{};

  /**
   * The base used to display the contents of the stack.
   *
   * @note The input is always base 10, unless the user enters a base prefix.
   */
  tbase base_{tbase::decimal};
};

tvalue tmodel::stack_pop() {
  if (stack_.empty())
    throw std::out_of_range("Stack is empty");

  tvalue result = stack_.back();
  stack_.pop_back();
  display_.pop_back();
  return result;
}

void tmodel::stack_drop() {
  if (stack_.empty())
    throw std::out_of_range("Stack is empty");

  stack_.pop_back();
  display_.pop_back();
}

void tmodel::synchronise_display() const {
  if (!dirty_)
    return;

  for (size_t i = 0; i < display_.size(); ++i)
    if (display_[i].empty())
      display_[i] = format(stack_[i]);

  dirty_ = false;
}

#if !defined(__cpp_lib_format)
static std::string format_binary(uint64_t v) {
  std::string result;
  result += "0b";
  char buffer[64];
  char *ptr = std::to_chars(std::begin(buffer), std::end(buffer), v, 2).ptr;
  result.append(std::begin(buffer), ptr);
  return result;
}

static std::string format_octal(uint64_t v) {
  std::string result;
  if (v != 0)
    result += "0";
  char buffer[22];
  char *ptr = std::to_chars(std::begin(buffer), std::end(buffer), v, 8).ptr;
  result.append(std::begin(buffer), ptr);
  return result;
}

static std::string format_decimal(uint64_t v) { return std::to_string(v); }

static std::string format_hexadecimal(uint64_t v) {
  std::string result;
  result += "0x";
  char buffer[8];
  char *ptr = std::to_chars(std::begin(buffer), std::end(buffer), v, 16).ptr;
  result.append(std::begin(buffer), ptr);
  return result;
}

static std::string format(tbase base, const tvalue &value) {
  return value.visit([base](auto v) {
    static_assert(std::same_as<int64_t, decltype(v)> ||
                  std::same_as<uint64_t, decltype(v)> ||
                  std::same_as<double, decltype(v)>);
    if constexpr (std::same_as<int64_t, decltype(v)> ||
                  std::same_as<uint64_t, decltype(v)>) {
      std::string result;
      if constexpr (std::same_as<int64_t, decltype(v)>)
        if (v < 0) {
          result += '-';
          // Note -v may not work properly. However std::format doesn't have
          // this issue.
          v = -v;
        }
      switch (base) {
      case tbase::binary:
        result.append(format_binary(v));
        break;
      case tbase::octal:
        result.append(format_octal(v));
        break;
      case tbase::decimal:
        result.append(format_decimal(v));
        break;
      case tbase::hexadecimal:
        result.append(format_hexadecimal(v));
        break;
      }
      return result;
    } else {
      char buf[128];
      std::sprintf(buf, "%g", v);
      return std::string{buf};
    }
  });
}
#endif

[[nodiscard]] std::string tmodel::format(const tvalue &value) const {
#if defined(__cpp_lib_format)
  // TODO Adjust for visitor.
  switch (base_) {
  case tbase::binary:
    return std::format("{:#b}", value.get());
  case tbase::octal:
    return std::format("{:#o}", value.get());
  case tbase::decimal:
    return std::format("{}", value.get());
  case tbase::hexadecimal:
    return std::format("{:#x}", value.get());
  }
  __builtin_unreachable();
#else
  // Initialize with right alignment for FLTK.
  return calculator::format(base_, value);

#endif
}

} // namespace calculator
