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
import lib.base;
import std;

namespace calculator {

export class tstack final {
public:
  // *** Query ***

  [[nodiscard]] bool empty() const noexcept { return values_.empty(); }
  [[nodiscard]] std::size_t size() const noexcept { return values_.size(); }
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

  void base_set(lib::tbase base) {
    base_ = base;
    invalidate_cache();
  }

  void grouping_toggle() {
    grouping_ = !grouping_;
    invalidate_cache();
  }

  void debug_mode_toggle() {
    debug_mode_ = !debug_mode_;
    invalidate_cache();
  }

private:
  void invalidate_cache() {
    std::ranges::for_each(strings_,
                          [](std::string &string) { string.clear(); });
    dirty_ = true;
  }

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
  lib::tbase base_{lib::tbase::decimal};

  /** Whether or not the grouping symbols are shown in the output. */
  bool grouping_{true};

  /** Show debug information in the output? */
  bool debug_mode_{false};
};

void tstack::duplicate() {
  if (values_.empty())
    throw std::out_of_range("The stack doesn't contain an element");

  values_.push_back(values_.back());
  strings_.push_back(strings_.back());
}

tvalue tstack::pop() {
  if (values_.empty())
    throw std::out_of_range("The stack doesn't contain an element");

  tvalue result = values_.back();
  values_.pop_back();
  strings_.pop_back();
  return result;
}

void tstack::drop() {
  if (values_.empty())
    throw std::out_of_range("The stack doesn't contain an element");

  values_.pop_back();
  strings_.pop_back();
}

void tstack::synchronise_display() const {
  if (!dirty_)
    return;

  for (std::size_t i = 0; i < strings_.size(); ++i)
    if (strings_[i].empty())
      strings_[i] = format(values_[i]);

  dirty_ = false;
}

static std::string format_integral(lib::tbase base, std::string_view debug,
                                   auto value) {
  switch (base) {
  case lib::tbase::binary:
    return std::format("{:#b}{}", value, debug);
  case lib::tbase::octal:
    return std::format("{:#o}{}", value, debug);
  case lib::tbase::decimal:
    return std::format("{}{}", value, debug);
  case lib::tbase::hexadecimal:
    return std::format("{:#x}{}", value, debug);
  }
  std::unreachable();
}

static std::string format_integral_grouped(lib::tbase base,
                                           std::string_view debug, auto value) {
  // The hard-code grouping used in the bases.
  // Note this might becom more generic in the future.
  struct grouping_3 : std::numpunct<char> {
    explicit grouping_3(char separator) : separator_(separator) {}

    std::string do_grouping() const override { return "\3"; }
    char do_thousands_sep() const override { return separator_; }

    char separator_;
  };
  // Use separator of the locale
  static std::locale locale_3{
      std::locale(),
      new grouping_3(
          std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep())};

  struct grouping_4 : std::numpunct<char> {
    std::string do_grouping() const override { return "\4"; }
    char do_thousands_sep() const override { return '\''; }
  };

  static std::locale locale_4{std::locale(), new grouping_4()};

  switch (base) {
  case lib::tbase::binary:
    return std::format(locale_4, "{:#Lb}{}", value, debug);
  case lib::tbase::octal:
    return std::format(locale_3, "{:#Lo}{}", value, debug);
  case lib::tbase::decimal:
    return std::format(locale_3, "{:L}{}", value, debug);
  case lib::tbase::hexadecimal:
    return std::format(locale_4, "{:#Lx}{}", value, debug);
  }
  std::unreachable();
}

/** The integral types used in the value class. */
template <class T>
  requires std::same_as<T, std::int64_t> || std::same_as<T, std::uint64_t>
static std::string format(lib::tbase base, bool grouping, bool debug_mode,
                          T value) {
  std::string_view debug = [&] {
    if (!debug_mode)
      return "";
    if constexpr (std::same_as<T, std::int64_t>)
      return " |i";
    else
      return " |u";
  }();

  if (grouping)
    return format_integral_grouped(base, debug, value);

  return format_integral(base, debug, value);
}

static std::string format(lib::tbase, bool, bool debug_mode, double value) {
  char buf[128];
  if (debug_mode)
    std::sprintf(buf, "%g |d", value);
  else
    std::sprintf(buf, "%g", value);
  return std::string{buf};
}

/** Catches changes of @ref tstorage. */
template <class T> static std::uint64_t format(lib::tbase, bool, T) = delete;

static std::string format(lib::tbase base, bool grouping, bool debug_mode,
                          const tvalue &value) {
  return value.visit([base, debug_mode, grouping](auto v) {
    return format(base, grouping, debug_mode, v);
  });
}

[[nodiscard]] std::string tstack::format(const tvalue &value) const {
  return calculator::format(base_, grouping_, debug_mode_, value);
}

} // namespace calculator
