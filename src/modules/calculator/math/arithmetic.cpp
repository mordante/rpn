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

export module calculator.math.arithmetic;

export import calculator.math.core;

export import <variant>;
import <cmath>;

namespace calculator {
namespace math {

static tstorage add(int64_t lhs, int64_t rhs) {
  return to_storage<int64_t>(static_cast<__int128_t>(lhs) +
                             static_cast<__int128_t>(rhs));
}

static tstorage add(uint64_t lhs, uint64_t rhs) {
  uint64_t result = lhs + rhs;
  if (result >= lhs)
    return result;

  return static_cast<double>(lhs) + static_cast<double>(rhs);
}

static tstorage add(uint64_t lhs, int64_t rhs) {
  return to_storage(static_cast<__int128_t>(lhs) +
                    static_cast<__int128_t>(rhs));
}

static double add(double lhs, double rhs) { return lhs + rhs; }

/** @see https://mordante.github.io/rpn/calculation.html#add */
export tstorage add(const tstorage &lhs, const tstorage &rhs) {
  if (std::holds_alternative<double>(lhs) ||
      std::holds_alternative<double>(rhs))
    return add(double_cast(lhs), double_cast(rhs));

  if (std::holds_alternative<int64_t>(lhs) &&
      std::holds_alternative<int64_t>(rhs))
    return add(get<int64_t>(lhs), get<int64_t>(rhs));

  if (std::holds_alternative<uint64_t>(lhs) &&
      std::holds_alternative<uint64_t>(rhs))
    return add(get<uint64_t>(lhs), get<uint64_t>(rhs));

  // At this point either lhs or rhs is an uint64_t and the other is an
  // int64_t. Since addition is communative use one helper function for both.

  if (std::holds_alternative<uint64_t>(lhs))
    return add(get<uint64_t>(lhs), get<int64_t>(rhs));

  return add(get<uint64_t>(rhs), get<int64_t>(lhs));
}

static tstorage sub(int64_t lhs, int64_t rhs) {
  const __int128_t result =
      static_cast<__int128_t>(lhs) - static_cast<__int128_t>(rhs);

  if (result >= std::numeric_limits<int64_t>::min() &&
      result <= std::numeric_limits<int64_t>::max())
    return static_cast<int64_t>(result);

  return static_cast<uint64_t>(result);
}

static tstorage sub(uint64_t lhs, uint64_t rhs) {
  return to_storage(static_cast<__int128_t>(lhs) -
                    static_cast<__int128_t>(rhs));
}

static tstorage sub(uint64_t lhs, int64_t rhs) {
  return to_storage(static_cast<__int128_t>(lhs) -
                    static_cast<__int128_t>(rhs));
}

static tstorage sub(int64_t lhs, uint64_t rhs) {
  const __int128_t result =
      static_cast<__int128_t>(lhs) - static_cast<__int128_t>(rhs);

  return to_storage(result);
}

static double sub(double lhs, double rhs) { return lhs - rhs; }

/** @see https://mordante.github.io/rpn/calculation.html#sub */
export tstorage sub(const tstorage &lhs, const tstorage &rhs) {
  if (std::holds_alternative<double>(lhs) ||
      std::holds_alternative<double>(rhs))
    return sub(double_cast(lhs), double_cast(rhs));

  if (std::holds_alternative<int64_t>(lhs) &&
      std::holds_alternative<int64_t>(rhs))
    return sub(get<int64_t>(lhs), get<int64_t>(rhs));

  if (std::holds_alternative<uint64_t>(lhs) &&
      std::holds_alternative<uint64_t>(rhs))
    return sub(get<uint64_t>(lhs), get<uint64_t>(rhs));

  // At this point either lhs or rhs is an uint64_t and the other is an
  // int64_t.
  if (std::holds_alternative<uint64_t>(lhs))
    return sub(get<uint64_t>(lhs), get<int64_t>(rhs));

  return sub(get<int64_t>(lhs), get<uint64_t>(rhs));
}

static tstorage mul(int64_t lhs, int64_t rhs) {
  return to_storage<int64_t>(static_cast<__int128_t>(lhs) *
                             static_cast<__int128_t>(rhs));
}

static tstorage mul(uint64_t lhs, uint64_t rhs) {
  return to_storage(static_cast<__uint128_t>(lhs) *
                    static_cast<__uint128_t>(rhs));
}

static tstorage mul(uint64_t lhs, int64_t rhs) {
  return to_storage(static_cast<__int128_t>(lhs) *
                    static_cast<__int128_t>(rhs));
}

static double mul(double lhs, double rhs) { return lhs * rhs; }

/** @see https://mordante.github.io/rpn/calculation.html#multiply */
export tstorage mul(const tstorage &lhs, const tstorage &rhs) {
  if (std::holds_alternative<double>(lhs) ||
      std::holds_alternative<double>(rhs))
    return mul(double_cast(lhs), double_cast(rhs));

  if (std::holds_alternative<int64_t>(lhs) &&
      std::holds_alternative<int64_t>(rhs))
    return mul(get<int64_t>(lhs), get<int64_t>(rhs));

  if (std::holds_alternative<uint64_t>(lhs) &&
      std::holds_alternative<uint64_t>(rhs))
    return mul(get<uint64_t>(lhs), get<uint64_t>(rhs));

  // At this point either lhs or rhs is an uint64_t and the other is an
  // int64_t. Since addition is communative use one helper function for both.

  if (std::holds_alternative<uint64_t>(lhs))
    return mul(get<uint64_t>(lhs), get<int64_t>(rhs));

  return mul(get<uint64_t>(rhs), get<int64_t>(lhs));
}

static double div(double lhs, double rhs) {
  if (rhs == 0.)
    throw std::domain_error("Division by zero");

  return lhs / rhs;
}

/** @see https://mordante.github.io/rpn/calculation.html#division */
export tstorage div(const tstorage &lhs, const tstorage &rhs) {
  return div(double_cast(lhs), double_cast(rhs));
}

static tstorage negate(int64_t value) {
  // Unlike other signed int operations negate prefers to store as unsigned.
  return to_storage(-static_cast<__int128_t>(value));
}

static tstorage negate(uint64_t value) {
  return to_storage(-static_cast<__int128_t>(value));
}

static tstorage negate(double value) { return -value; }

export tstorage negate(tstorage value) {
  if (std::holds_alternative<int64_t>(value))
    return negate(get<int64_t>(value));
  if (std::holds_alternative<uint64_t>(value))
    return negate(get<uint64_t>(value));

  return negate(get<double>(value));
}

// TODO static can't be used, since caller is a template.
/*static*/ tstorage pow(double value, int exp) { return std::pow(value, exp); }

// TODO static can't be used, since caller is a template.
/*static*/ tstorage pow(int64_t value, int exp) {

  // TODO use a smarter algorithm.
  __int128_t result = value;
  for (int i = 1; i < exp; ++i) {
    if (result > std::numeric_limits<int64_t>::max() ||
        result < std::numeric_limits<int64_t>::min())
      return pow(static_cast<double>(value), exp);
    result *= value;
  }

  return to_storage<int64_t>(result);
}

// TODO static can't be used, since caller is a template.
/*static*/ tstorage pow(uint64_t value, int exp) {

  // TODO use a smarter algorithm.
  __uint128_t result = value;
  for (int i = 1; i < exp; ++i) {
    if (result > std::numeric_limits<uint64_t>::max())
      return pow(static_cast<double>(value), exp);
    result *= value;
  }

  return to_storage(result);
}

export tstorage pow(tstorage value, tstorage exp) {
  // TODO Improve algorithm selection and return type.
  return std::pow(double_cast(value), double_cast(exp));
}

export template <int N>
  requires(N >= 2 && N <= 9)
tstorage pow(tstorage value) {
  // TODO N is a compile-time value this can be used to use a smarter algorithm.
  if (std::holds_alternative<int64_t>(value))
    return pow(get<int64_t>(value), N);
  if (std::holds_alternative<uint64_t>(value))
    return pow(get<uint64_t>(value), N);

  return pow(get<double>(value), N);
}

} // namespace math
} // namespace calculator
