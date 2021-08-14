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

export module math;

import<cinttypes>;

export namespace math {
/**
 * @returns The value @p lhs added to @p rhs.
 * @pre INT64_MIN <= lhs + rhs <= INT64_MAX
 */
[[nodiscard]] int64_t add(int64_t lhs, int64_t rhs) noexcept {
  return lhs + rhs;
}

/**
 * @returns The value @p lhs subtracted to @p rhs.
 * @pre INT64_MIN <= lhs - rhs <= INT64_MAX
 */
[[nodiscard]] int64_t sub(int64_t lhs, int64_t rhs) noexcept {
  return lhs - rhs;
}

/**
 * @returns The value @p lhs multiplied by @p rhs.
 * @pre INT64_MIN <= lhs * rhs <= INT64_MAX
 */
[[nodiscard]] int64_t mul(int64_t lhs, int64_t rhs) noexcept {
  return lhs * rhs;
}

/**
 * @returns The value @p lhs multiplied by @p rhs.
 * @pre rhs != 0
 * @pre INT64_MIN <= lhs / rhs <= INT64_MAX
 */
[[nodiscard]] int64_t div(int64_t lhs, int64_t rhs) noexcept {
  return lhs / rhs;
}

/*** Bitwise operations ***/

/** @returns The value @p lhs and-ed with @p rhs. */
[[nodiscard]] int64_t bit_and(int64_t lhs, int64_t rhs) noexcept {
  return lhs & rhs;
}

/** @returns The value @p lhs or-ed with @p rhs. */
[[nodiscard]] int64_t bit_or(int64_t lhs, int64_t rhs) noexcept {
  return lhs | rhs;
}

/** @returns The value @p lhs xor-ed with @p rhs. */
[[nodiscard]] int64_t bit_xor(int64_t lhs, int64_t rhs) noexcept {
  return lhs ^ rhs;
}

/** @returns The bitwise complement of @p value. */
[[nodiscard]] int64_t bit_complement(int64_t value) noexcept { return ~value; }

} // namespace math
