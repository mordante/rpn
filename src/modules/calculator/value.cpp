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

export module calculator.value;

import calculator.math.arithmetic;
import calculator.math.bitwise;
import calculator.math.core;
import calculator.math.round;
import std;
import std.compat;

namespace calculator {
/**
 * Basic value class for the entries on the calculator's stack.
 *
 * At the moment class is a simple wrapper for a @c math::tstorage. For the
 * future some changes are planned:
 * - Cache the output of @ref to_string.
 *
 * For these reasons the stack uses this class instead of a @c math::tstorage
 * directly.  To make it easy to use the class with math functions the class is
 * implicitly convertible from and to the storage type.
 */
export class tvalue final {
public:
  explicit constexpr tvalue(int64_t value) noexcept : value_(value) {}
  explicit constexpr tvalue(uint64_t value) noexcept : value_(value) {}
  explicit constexpr tvalue(double value) noexcept : value_(value) {}

  constexpr tvalue(math::tstorage value) noexcept : value_(value) {}
  constexpr operator math::tstorage() const { return value_; }

  /**
   * A visitor to use the internal values of the stored value.
   *
   * @todo This there are no unit tests for this interface yet.
   */
  template <class Visitor> auto visit(Visitor &&visitor) const {
    return std::visit(std::forward<Visitor>(visitor), value_);
  }

private:
  math::tstorage value_;
};
} // namespace calculator
