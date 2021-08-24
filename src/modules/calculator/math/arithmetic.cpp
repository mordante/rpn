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

export import<variant>;

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

/**
 * @todo Determine proper return type. It now toggles between the signed and
 * unsigned integral type. This is mainly done for testing. Instead it should
 * properly validate the best return type.
 */
export tstorage negate(tstorage value) {
  if (std::holds_alternative<int64_t>(value)) {
    int64_t v = std::get<int64_t>(value);
    if (v < 0)
      return uint64_t(-v);
    else
      return -v;
  }

  return -int64_t(std::get<uint64_t>(value));
}

} // namespace math
} // namespace calculator
