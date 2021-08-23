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

export module calculator.math.core;

import<bit>;
import<cmath>;
import<concepts>;
export import<variant>;

namespace calculator {
namespace math {

export using tstorage = std::variant<int64_t, uint64_t, double>;

export template <class T>
concept is_storage = std::same_as<T, int64_t> || std::same_as<T, uint64_t> ||
    std::same_as<T, double>;

#ifdef __cpp_lib_bit_cast
export using std::bit_cast;
#else
export template <class To, class From> To bit_cast(const From &from) noexcept {
  To to;
  std::memcpy(&to, &from, sizeof(To));
  return to;
}
#endif

static uint64_t bitwise_cast(int64_t value) { return value; }

static uint64_t bitwise_cast(uint64_t value) { return value; }

static uint64_t bitwise_cast(double value) { return bit_cast<uint64_t>(value); }

/** Catches changes of @ref tstorage. */
template <class T> static uint64_t bitwise_cast(T) = delete;

/**
 * @see https://mordante.github.io/rpn/calculation.html#bitwise-operations-cast
 */
export uint64_t bitwise_cast(const tstorage &value) {
  return std::visit([](auto v) { return bitwise_cast(v); }, value);
}

static uint64_t positive_integral_cast(int64_t value) {
  if (value <= 0)
    throw std::range_error("Not a positive value");
  return value;
}

static uint64_t positive_integral_cast(uint64_t value) {
  if (value == 0)
    throw std::range_error("Not a positive value");
  return value;
}

static uint64_t positive_integral_cast(double value) {
  // This tests means we don't need to test for -0 later.
  if (value <= 0.)
    throw std::range_error("Not a positive value");

  if (value > static_cast<double>(std::numeric_limits<uint64_t>::max()))
    throw std::range_error("Value too large");

  double result;
  if (modf(value, &result) != 0.)
    throw std::range_error("Not an integral");
  return result;
}

/** Catches changes of @ref tstorage. */
template <class T> static uint64_t positive_integral_cast(T) = delete;

export uint64_t positive_integral_cast(const tstorage &value) {
  return std::visit([](auto v) { return positive_integral_cast(v); }, value);
}

} // namespace math
} // namespace calculator
