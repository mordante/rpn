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

import calculator.math.arithmetic;

import <limits>;

#include <gtest/gtest.h>

namespace calculator {
namespace math {

TEST(arithmetic, negate_int64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(negate(tstorage{int64_t(1)})));
  EXPECT_EQ(std::get<uint64_t>(negate(tstorage{int64_t(0)})), 0);
  EXPECT_EQ(std::get<uint64_t>(negate(tstorage{int64_t(-1)})), 1);
  EXPECT_EQ(std::get<int64_t>(negate(tstorage{int64_t(1)})), -1);

  ASSERT_EQ(
      std::get<int64_t>(negate(tstorage{std::numeric_limits<int64_t>::max()})),
      -std::numeric_limits<int64_t>::max());

  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      negate(tstorage{std::numeric_limits<int64_t>::min()})));
  ASSERT_EQ(
      std::get<uint64_t>(negate(tstorage{std::numeric_limits<int64_t>::min()})),
      uint64_t(-__uint128_t(std::numeric_limits<int64_t>::min())));
}

TEST(arithmetic, negate_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(negate(tstorage{uint64_t(1)})));
  EXPECT_EQ(std::get<int64_t>(negate(tstorage{uint64_t(1)})), -1);

  EXPECT_EQ(std::get<uint64_t>(negate(tstorage{uint64_t(0)})), 0);
  EXPECT_EQ(std::get<int64_t>(negate(tstorage{
                uint64_t(-__uint128_t(std::numeric_limits<int64_t>::min()))})),
            std::numeric_limits<int64_t>::min());

  EXPECT_EQ(
      std::get<double>(negate(tstorage{std::numeric_limits<uint64_t>::max()})),
      -double(std::numeric_limits<uint64_t>::max()));
}

TEST(arithmetic, negate_double) {
  ASSERT_TRUE(std::holds_alternative<double>(negate(tstorage{double(1)})));

  EXPECT_EQ(std::get<double>(negate(tstorage{double(1)})), -1.);
  EXPECT_EQ(std::get<double>(negate(tstorage{double(0)})), 0.);
  EXPECT_EQ(std::get<double>(negate(tstorage{double(-1)})), 1.);
}

} // namespace math
} // namespace calculator
