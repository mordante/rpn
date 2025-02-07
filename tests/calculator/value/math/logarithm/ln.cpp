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

import calculator.math.logarithm;

#include <cmath>
#include <limits>

#include <gtest/gtest.h>

namespace calculator {
namespace math {

TEST(arithmetic, ln_int64_t) {
  EXPECT_TRUE(std::isnan(
      std::get<double>(ln(tstorage{std::numeric_limits<int64_t>::min()}))));
  EXPECT_TRUE(
      std::isnan(std::get<double>(ln(tstorage{static_cast<int64_t>(-1)}))));
  EXPECT_EQ(std::get<double>(ln(tstorage{static_cast<int64_t>(0)})),
            -std::numeric_limits<double>::infinity());
  EXPECT_EQ(std::get<double>(ln(tstorage{static_cast<int64_t>(2)})),
            std::log(2.));
  EXPECT_EQ(std::get<double>(ln(tstorage{std::numeric_limits<int64_t>::max()})),
            std::log(std::numeric_limits<int64_t>::max()));
}

TEST(arithmetic, ln_uint64_t) {
  EXPECT_EQ(std::get<double>(ln(tstorage{static_cast<uint64_t>(0)})),
            -std::numeric_limits<double>::infinity());
  EXPECT_EQ(std::get<double>(ln(tstorage{static_cast<uint64_t>(2)})),
            std::log(2.));
  EXPECT_EQ(
      std::get<double>(ln(tstorage{std::numeric_limits<uint64_t>::max()})),
      std::log(std::numeric_limits<uint64_t>::max()));
}

TEST(arithmetic, ln_double) {
  EXPECT_TRUE(std::isnan(
      std::get<double>(ln(tstorage{-std::numeric_limits<double>::max()}))));
  EXPECT_TRUE(std::isnan(std::get<double>(ln(tstorage{-1.}))));
  EXPECT_TRUE(std::isnan(
      std::get<double>(ln(tstorage{-std::numeric_limits<double>::min()}))));
  EXPECT_EQ(std::get<double>(ln(tstorage{-0.})),
            -std::numeric_limits<double>::infinity());
  EXPECT_EQ(std::get<double>(ln(tstorage{0.})),
            -std::numeric_limits<double>::infinity());
  EXPECT_EQ(std::get<double>(ln(tstorage{std::numeric_limits<double>::min()})),
            std::log(std::numeric_limits<double>::min()));
  EXPECT_NEAR(std::get<double>(ln(tstorage{2.71828})), 1., 1e-6);
  EXPECT_EQ(std::get<double>(ln(tstorage{std::numeric_limits<double>::max()})),
            std::log(std::numeric_limits<double>::max()));
}

} // namespace math
} // namespace calculator
