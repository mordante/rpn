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

import calculator.math.round;

#include <cmath>

#include <gtest/gtest.h>

namespace calculator {
namespace math {

TEST(arithmetic, floor_int64_t) {
  EXPECT_THROW(floor(tstorage{int64_t(0)}), std::domain_error);
}

TEST(arithmetic, floor_uint64_t) {
  EXPECT_THROW(floor(tstorage{uint64_t(0)}), std::domain_error);
}

TEST(arithmetic, floor_double) {
  EXPECT_EQ(std::get<double>(floor(tstorage{-1.1})), -2.);
  EXPECT_EQ(std::get<double>(floor(tstorage{-1.})), -1.);
  EXPECT_EQ(std::get<double>(floor(tstorage{-0.9})), -1.);
  EXPECT_EQ(std::get<double>(floor(tstorage{-0.5000001})), -1.);
  EXPECT_EQ(std::get<double>(floor(tstorage{-0.5})), -1.);
  EXPECT_EQ(std::get<double>(floor(tstorage{-0.499999})), -1.);
  EXPECT_EQ(std::get<double>(floor(tstorage{-0.1})), -1.);
  EXPECT_EQ(std::get<double>(floor(tstorage{-0.})), 0.);

  EXPECT_EQ(std::get<double>(floor(tstorage{0.})), 0.);
  EXPECT_EQ(std::get<double>(floor(tstorage{0.1})), 0.);
  EXPECT_EQ(std::get<double>(floor(tstorage{0.499999})), 0.);
  EXPECT_EQ(std::get<double>(floor(tstorage{0.5})), 0.);
  EXPECT_EQ(std::get<double>(floor(tstorage{0.5000001})), 0.);
  EXPECT_EQ(std::get<double>(floor(tstorage{0.9})), 0.);
  EXPECT_EQ(std::get<double>(floor(tstorage{1.})), 1.);
  EXPECT_EQ(std::get<double>(floor(tstorage{1.1})), 1.);
}

} // namespace math
} // namespace calculator
