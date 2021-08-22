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

export module calculator.math.bitwise;

export import calculator.math.core;

namespace calculator {
namespace math {

template <class T> static T bit_and(T lhs, T rhs) { return lhs & rhs; }

/** @see https://mordante.github.io/rpn/calculation.html#generic */
export tstorage bit_and(const tstorage &lhs, const tstorage &rhs) {
  if (std::holds_alternative<int64_t>(lhs) &&
      std::holds_alternative<int64_t>(rhs))
    return bit_and(std::get<int64_t>(lhs), std::get<int64_t>(rhs));

  return bit_and(bitwise_cast(lhs), bitwise_cast(rhs));
}

template <class T> static T bit_or(T lhs, T rhs) { return lhs | rhs; }

/** @see https://mordante.github.io/rpn/calculation.html#generic */
export tstorage bit_or(const tstorage &lhs, const tstorage &rhs) {
  if (std::holds_alternative<int64_t>(lhs) &&
      std::holds_alternative<int64_t>(rhs))
    return bit_or(std::get<int64_t>(lhs), std::get<int64_t>(rhs));

  return bit_or(bitwise_cast(lhs), bitwise_cast(rhs));
}

template <class T> static T bit_xor(T lhs, T rhs) { return lhs ^ rhs; }

/** @see https://mordante.github.io/rpn/calculation.html#generic */
export tstorage bit_xor(const tstorage &lhs, const tstorage &rhs) {
  if (std::holds_alternative<int64_t>(lhs) &&
      std::holds_alternative<int64_t>(rhs))
    return bit_xor(std::get<int64_t>(lhs), std::get<int64_t>(rhs));

  return bit_xor(bitwise_cast(lhs), bitwise_cast(rhs));
}

template <class T> static T complement(T value) { return ~value; }

/** @see https://mordante.github.io/rpn/calculation.html#complement */
export tstorage complement(const tstorage &value) {
  if (std::holds_alternative<int64_t>(value))
    return complement(std::get<int64_t>(value));

  return complement(bitwise_cast(value));
}
} // namespace math
} // namespace calculator
