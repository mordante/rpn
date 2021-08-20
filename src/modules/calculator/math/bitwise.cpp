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

import calculator.math.core;

import<variant>;

namespace calculator {
namespace math {

template <class T> static T bit_and(T lhs, T rhs) { return lhs & rhs; }

export tstorage bit_and(tstorage lhs, tstorage rhs) {
  if (std::holds_alternative<int64_t>(lhs) ||
      std::holds_alternative<int64_t>(rhs))
    return bit_and(std::get<int64_t>(lhs), std::get<int64_t>(rhs));

  return bit_and(as<uint64_t>(lhs), as<uint64_t>(rhs));
}

} // namespace math
} // namespace calculator
