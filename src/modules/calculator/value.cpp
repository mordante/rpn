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

import<cinttypes>;
import<concepts>;
import<string>;
export import<variant>;

namespace calculator {

/**
 * Basic value class for the entries on the calculator's stack.
 *
 * At the moment class is a simple wrapper for a @c int64_t. For the future
 * some changes are planned:
 * - Allow @c __int128_t as storage type.
 * - Allow @c double as storage type.
 * - Cache the output of @ref to_string.
 *
 * For these reasons the stack uses this class instead of a @c int64_t directly.
 */
export class tvalue final {
public:
  tvalue() = default;

  /** @todo This constructor is a hack to avoid ambigious overloads. */
  explicit constexpr tvalue(int value) noexcept
      : tvalue(static_cast<int64_t>(value)) {}
  explicit constexpr tvalue(int64_t value) noexcept : value_(value) {}
  explicit constexpr tvalue(double value) noexcept : value_(value) {}

  /**
   * A visitor to use the internal values of the stored value.
   *
   * @todo This there are no unit tests for this interface yet.
   */
  template <class Visitor> auto visit(Visitor &&visitor) const {
    return std::visit(std::forward<Visitor>(visitor), value_);
  }

  /*** Arithmetic ***/

  /** Adds @p rhs to the current value. */
  void add(const tvalue &rhs) {
    value_ = std::get<int64_t>(value_) + std::get<int64_t>(rhs.value_);
  }

  /** Subtracts @p rhs from the current value. */
  void sub(const tvalue &rhs) {
    value_ = std::get<int64_t>(value_) - std::get<int64_t>(rhs.value_);
  }

  /** Multiplies the current value by @p rhs. */
  void mul(const tvalue &rhs) {
    value_ = std::get<int64_t>(value_) * std::get<int64_t>(rhs.value_);
  }

  /** Divides the current value by @p rhs. */
  void div(const tvalue &rhs) {
    value_ = std::get<int64_t>(value_) / std::get<int64_t>(rhs.value_);
  }

  /** Negates the current value. */
  void negate() { value_ = math::negate(value_); }

  /*** Bitwise ***/

  /** Bitwise and the current value with @p rhs. */
  void bit_and(const tvalue &rhs) {
    value_ = math::bit_and(value_, rhs.value_);
  }

  /** Bitwise or the current value with @p rhs. */
  void bit_or(const tvalue &rhs) {
    value_ = std::get<int64_t>(value_) | std::get<int64_t>(rhs.value_);
  }

  /** Bitwise xor the current value with @p rhs. */
  void bit_xor(const tvalue &rhs) {
    value_ = std::get<int64_t>(value_) ^ std::get<int64_t>(rhs.value_);
  }

  /** Replace the current value by its bitwise complement. */
  void complement() { value_ = ~std::get<int64_t>(value_); }

  /** Bitwise shifts the current value @p rhs positions to the left. */
  void shl(const tvalue &rhs) {
    value_ = std::get<int64_t>(value_) << std::get<int64_t>(rhs.value_);
  }

  /** Bitwise shifts the current value @p rhs positions to the right. */
  void shr(const tvalue &rhs) {
    value_ = std::get<int64_t>(value_) >> std::get<int64_t>(rhs.value_);
  }

private:
  math::tstorage value_{0};

  /** @todo This there are no unit tests for this interface yet. */
  friend auto operator==(const tvalue &lhs, int64_t rhs) {
    if (std::holds_alternative<int64_t>(lhs.value_))
      return std::get<int64_t>(lhs.value_) == rhs;

    // TODO first test whether it's in the proper range
    return static_cast<int64_t>(std::get<uint64_t>(lhs.value_)) == rhs;
  }
};

} // namespace calculator
