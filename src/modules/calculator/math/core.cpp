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

module;

#include <bit>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <variant>

export module calculator.math.core;

namespace calculator {
namespace math {

export using tstorage = std::variant<int64_t, uint64_t, double>;

export template <class T>
concept is_storage = std::same_as<T, int64_t> || std::same_as<T, uint64_t> ||
                     std::same_as<T, double>;

static uint64_t bitwise_cast(int64_t value) {
  return static_cast<uint64_t>(value);
}

static uint64_t bitwise_cast(uint64_t value) { return value; }

static uint64_t bitwise_cast(double value) {
  return std::bit_cast<uint64_t>(value);
}

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
  return static_cast<uint64_t>(value);
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
  return static_cast<uint64_t>(result);
}

/** Catches changes of @ref tstorage. */
template <class T> static uint64_t positive_integral_cast(T) = delete;

export uint64_t positive_integral_cast(const tstorage &value) {
  return std::visit([](auto v) { return positive_integral_cast(v); }, value);
}

static int64_t negative_integral_cast(int64_t value) {
  if (value >= 0)
    throw std::range_error("Not a negative value");
  return value;
}

static int64_t negative_integral_cast(uint64_t) {
  throw std::range_error("Not a negative value");
}

static int64_t negative_integral_cast(double value) {
  // This tests means we don't need to test for -0 later.
  if (value >= 0.)
    throw std::range_error("Not a negative value");

  if (value < static_cast<double>(std::numeric_limits<int64_t>::min()))
    throw std::range_error("Value too large");

  double result;
  if (modf(value, &result) != 0.)
    throw std::range_error("Not an integral");
  return static_cast<int64_t>(result);
}

/** Catches changes of @ref tstorage. */
template <class T> static int64_t negative_integral_cast(T) = delete;

export int64_t negative_integral_cast(const tstorage &value) {
  return std::visit([](auto v) { return negative_integral_cast(v); }, value);
}

template <class T>
  requires(std::same_as<T, int64_t> || std::same_as<T, uint64_t>)
static tstorage integral_cast(T value) {
  return value;
}

static tstorage integral_cast(double value) {
  if (value == 0.)
    return uint64_t(0);
  if (value < 0.)
    return negative_integral_cast(value);
  if (value > 0.)
    return positive_integral_cast(value);

  throw std::domain_error("value can't be converted to an integral");
}

/** Catches changes of @ref tstorage. */
template <class T> static tstorage integral_cast(T) = delete;

export tstorage integral_cast(const tstorage &value) {
  return std::visit([](auto v) { return integral_cast(v); }, value);
}

static double double_cast(int64_t value) { return static_cast<double>(value); }

static double double_cast(uint64_t value) { return static_cast<double>(value); }

static double double_cast(double value) { return value; }

/** Catches changes of @ref tstorage. */
template <class T> static double double_cast(T) = delete;

export double double_cast(const tstorage &value) {
  return std::visit([](auto v) { return double_cast(v); }, value);
}

/**
 * Converts the @p value to the proper @ref tstorage type.
 *
 * Some of the calculations use an @c __int128_t internally for their
 * calculations. This function does the generic store in the best storage type
 * action.
 *
 * When the result can be stored in both an @c uint64_t and an @c int64_t this
 * version preferes the @c uint64_t.
 */
export template <class T = uint64_t>
  requires(std::same_as<T, int64_t> || std::same_as<T, uint64_t>)
tstorage to_storage(__int128_t value) {
  if (value < std::numeric_limits<int64_t>::min() ||
      value > std::numeric_limits<uint64_t>::max())
    return static_cast<double>(value);

  if constexpr (std::same_as<T, uint64_t>) {
    if (value < 0)
      return static_cast<int64_t>(value);

    return static_cast<uint64_t>(value);
  } else {
    if (value > std::numeric_limits<int64_t>::max())
      return static_cast<uint64_t>(value);

    return static_cast<int64_t>(value);
  }
}

export tstorage to_storage(__uint128_t value) {
  if (value > std::numeric_limits<uint64_t>::max())
    return static_cast<double>(value);

  return static_cast<uint64_t>(value);
}
} // namespace math
} // namespace calculator
