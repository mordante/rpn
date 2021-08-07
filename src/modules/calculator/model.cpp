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
  void input_append(std::string_view data) { input_.append(data); }

  [[nodiscard]] const std::string &input_get() const noexcept { return input_; }

  // * Base *

  void base_set(tbase base) { base_ = base; }

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
   * The
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

void tmodel::synchronise_display() const {
  if (!dirty_)
    return;

  for (size_t i = 0; i < display_.size(); ++i)
    if (display_[i].empty())
      display_[i] = format(stack_[i]);

  dirty_ = false;
}

[[nodiscard]] std::string tmodel::format(const tvalue &value) const {
#if defined(__cpp_lib_format)
  // All formats use the '@r' prefix, this uses the right alignment in FLTK.
  switch (base_) {
  case tbase::binary:
    return std::format("@r{:#b}", value.get());
  case tbase::octal:
    return std::format("@r{:#o}", value.get());
  case tbase::decimal:
    return std::format("@r{}", value.get());
  case tbase::hexadecimal:
    return std::format("@r{:#x}", value.get());
  }
  __builtin_unreachable();
#else
  // Initialize with right alignment for FLTK.
  std::string result = "@r";

  switch (base_) {
  case tbase::binary: {
    result += "0b";
    char buffer[64];
    char *ptr =
        std::to_chars(std::begin(buffer), std::end(buffer), value.get(), 2).ptr;
    result.append(std::begin(buffer), ptr);
  } break;
  case tbase::octal: {
    if (value.get() != 0)
      result += "0";
    char buffer[22];
    char *ptr =
        std::to_chars(std::begin(buffer), std::end(buffer), value.get(), 8).ptr;
    result.append(std::begin(buffer), ptr);
  } break;
  case tbase::decimal:
    result += std::to_string(value.get());
    break;

  case tbase::hexadecimal: {
    result += "0x";
    char buffer[8];
    char *ptr =
        std::to_chars(std::begin(buffer), std::end(buffer), value.get(), 16)
            .ptr;
    result.append(std::begin(buffer), ptr);
  } break;
  }
  return result;
#endif
}

} // namespace calculator
