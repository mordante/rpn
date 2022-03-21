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

export module calculator.stack;

export import calculator.value;

import<algorithm>;
import<charconv>;
import<format>;
import<string>;
export import<vector>;

namespace calculator {

/** The supported bases in the calculator. */
export enum class tbase { binary, octal, decimal, hexadecimal };

export class tstack final {
public:
  // *** Query ***

  [[nodiscard]] bool empty() const noexcept { return values_.empty(); }
  [[nodiscard]] size_t size() const noexcept { return values_.size(); }
  [[nodiscard]] const std::vector<std::string> &strings() const noexcept {
    synchronise_display();
    return strings_;
  }

  // *** Modifiers ***

  /** Adds the @p value to the back of the stack. */
  void push(tvalue value) {
    values_.emplace_back(std::move(value));
    strings_.emplace_back();
    dirty_ = true;
  }

  /** Duplicates the last entry on the stack. */
  void duplicate();

  /**
   * @returns The last element at the back of the stack.
   * @throws @ref std::out_of_range when the stack is empty.
   */
  [[nodiscard]] tvalue pop();

  /**
   * Removes the last element at the back of the stack
   * @throws @ref std::out_of_range when the stack is empty.
   */
  void drop();

  void base_set(tbase base) {
    base_ = base;
    // clear the cached strings.
    std::for_each(strings_.begin(), strings_.end(),
                  [](std::string &string) { string.clear(); });
    dirty_ = true;
  }

private:
  /**
   * The stack with all values of the applications.
   *
   * The usage pattern is a LIFO. Since @ref std::vector operates faster on the
   * the back new elements are added to the back. So the first item is the
   * oldest item.
   */
  std::vector<tvalue> values_{};

  /**
   * The shadow stack with values rendered as strings.
   *
   * The size always matches the size of @ref values_. The display is a cache of
   * the rendered value. When it contains an empty string its contents are out
   * of sync. When invalidating a cache entry the @ref dirty_ flag must be set.
   */
  mutable std::vector<std::string> strings_{};

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

void tstack::duplicate() {
  if (values_.empty())
    throw std::out_of_range("Stack is empty");

  values_.push_back(values_.back());
  strings_.push_back(strings_.back());
}

tvalue tstack::pop() {
  if (values_.empty())
    throw std::out_of_range("Stack is empty");

  tvalue result = values_.back();
  values_.pop_back();
  strings_.pop_back();
  return result;
}

void tstack::drop() {
  if (values_.empty())
    throw std::out_of_range("Stack is empty");

  values_.pop_back();
  strings_.pop_back();
}

void tstack::synchronise_display() const {
  if (!dirty_)
    return;

  for (size_t i = 0; i < strings_.size(); ++i)
    if (strings_[i].empty())
      strings_[i] = format(values_[i]);

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

static std::string format(tbase base, uint64_t value) {
  switch (base) {
  case tbase::binary:
    return format_binary(value);
  case tbase::octal:
    return format_octal(value);
  case tbase::decimal:
    return format_decimal(value);
  case tbase::hexadecimal:
    return format_hexadecimal(value);
  }
}

static std::string format(tbase base, int64_t value) {
  std::string result;
  if (value < 0) {
    result += '-';
    // Note -value may not work properly. However std::format doesn't have
    // this issue.
    value = -value;
  }
  return result + format(base, static_cast<uint64_t>(value));
}

static std::string format(tbase, double value) {
  char buf[128];
  std::sprintf(buf, "%g", value);
  return std::string{buf};
}

/** Catches changes of @ref tstorage. */
template <class T> static uint64_t format(tbase, T) = delete;

static std::string format(tbase base, const tvalue &value) {
  return value.visit([base](auto v) { return format(base, v); });
}
#endif

[[nodiscard]] std::string tstack::format(const tvalue &value) const {
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
  return calculator::format(base_, value);
#endif
}

} // namespace calculator
