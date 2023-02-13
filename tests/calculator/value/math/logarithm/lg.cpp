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

import <cmath>;
import <limits>;

#include <gtest/gtest.h>

namespace calculator {
namespace math {

TEST(arithmetic, lg_int64_t) {
  EXPECT_TRUE(std::isnan(
      std::get<double>(lg(tstorage{std::numeric_limits<int64_t>::min()}))));
  EXPECT_TRUE(
      std::isnan(std::get<double>(lg(tstorage{static_cast<int64_t>(-1)}))));
  EXPECT_EQ(std::get<double>(lg(tstorage{static_cast<int64_t>(0)})),
            -std::numeric_limits<double>::infinity());
  EXPECT_EQ(std::get<double>(lg(tstorage{static_cast<int64_t>(2)})), 1.);
  EXPECT_EQ(std::get<double>(lg(tstorage{std::numeric_limits<int64_t>::max()})),
            std::log2(std::numeric_limits<int64_t>::max()));
}

TEST(arithmetic, lg_uint64_t) {
  EXPECT_EQ(std::get<double>(lg(tstorage{static_cast<uint64_t>(0)})),
            -std::numeric_limits<double>::infinity());
  EXPECT_EQ(std::get<double>(lg(tstorage{static_cast<uint64_t>(2)})), 1.);
  EXPECT_EQ(
      std::get<double>(lg(tstorage{std::numeric_limits<uint64_t>::max()})),
      std::log2(std::numeric_limits<uint64_t>::max()));
}

TEST(arithmetic, lg_double) {
  EXPECT_TRUE(std::isnan(
      std::get<double>(lg(tstorage{-std::numeric_limits<double>::max()}))));
  EXPECT_TRUE(std::isnan(std::get<double>(lg(tstorage{-1.}))));
  EXPECT_TRUE(std::isnan(
      std::get<double>(lg(tstorage{-std::numeric_limits<double>::min()}))));
  EXPECT_EQ(std::get<double>(lg(tstorage{-0.})),
            -std::numeric_limits<double>::infinity());
  EXPECT_EQ(std::get<double>(lg(tstorage{0.})),
            -std::numeric_limits<double>::infinity());
  EXPECT_EQ(std::get<double>(lg(tstorage{std::numeric_limits<double>::min()})),
            std::log2(std::numeric_limits<double>::min()));
  EXPECT_EQ(std::get<double>(lg(tstorage{2.})), 1.);
  EXPECT_EQ(std::get<double>(lg(tstorage{std::numeric_limits<double>::max()})),
            std::log2(std::numeric_limits<double>::max()));
}

} // namespace math
} // namespace calculator
