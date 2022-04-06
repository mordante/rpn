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

export module calculator.math.round;

export import calculator.math.core;

import <cmath>;

namespace calculator {
namespace math {

/** @see https://mordante.github.io/rpn/calculation.html#round */
export tstorage round(tstorage value) {
  if (std::holds_alternative<double>(value))
    return std::round(get<double>(value));

  throw std::domain_error("Not a floating-point");
}

/** @see https://mordante.github.io/rpn/calculation.html#floor */
export tstorage floor(tstorage value) {
  if (std::holds_alternative<double>(value))
    return std::floor(get<double>(value));

  throw std::domain_error("Not a floating-point");
}

/** @see https://mordante.github.io/rpn/calculation.html#ceil */
export tstorage ceil(tstorage value) {
  if (std::holds_alternative<double>(value))
    return std::ceil(get<double>(value));

  throw std::domain_error("Not a floating-point");
}

} // namespace math
} // namespace calculator
