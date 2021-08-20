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

import<variant>;

namespace calculator {
namespace math {

export using tstorage = std::variant<int64_t, uint64_t, double>;

export template <class T> T as(const tstorage &value) {
  if (std::holds_alternative<T>(value))
    return std::get<T>(value);

  return std::visit([](auto value) { return static_cast<T>(value); }, value);
}

} // namespace math
} // namespace calculator
