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

#include <cmath>
#include <variant>

export module calculator.math.logarithm;

export import calculator.math.core;

namespace calculator {
namespace math {

// Note since logarithms are expected to be used on floating-point values these
// functions always return a floating-point value.
export tstorage lg(tstorage value) { return std::log2(double_cast(value)); }

export tstorage ln(tstorage value) { return std::log(double_cast(value)); }

export tstorage log(tstorage value) { return std::log10(double_cast(value)); }

} // namespace math
} // namespace calculator
