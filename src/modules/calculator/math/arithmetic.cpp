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

import calculator.math.core;

import<variant>;

namespace calculator {
namespace math {

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
