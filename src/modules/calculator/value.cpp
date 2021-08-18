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

import<cinttypes>;
import<string>;

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

  /** Converting constructor. */
  constexpr tvalue(int64_t value) noexcept : value_(value) {}

  [[nodiscard]] constexpr int64_t get() const noexcept { return value_; }

private:
  int64_t value_{0};
};

} // namespace calculator
