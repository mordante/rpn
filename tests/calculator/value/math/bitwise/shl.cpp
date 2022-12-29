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

import calculator.math.bitwise;

import <bit>;

#include <gtest/gtest.h>

namespace calculator {
namespace math {

// We can't directly use the zero and one value for doubles to create helpers.
// The test shl_double_double validates these are the proper values.
// TODO make constexpr.
const double zero = std::bit_cast<double>(uint64_t(0));
const double one = std::bit_cast<double>(uint64_t(1));

TEST(bitwise, shl_int64_t_int64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      shl(tstorage{int64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_THROW(shl(tstorage{int64_t(0)}, tstorage{int64_t(-1)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{int64_t(0)}, tstorage{int64_t(0)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{int64_t(0)}, tstorage{int64_t(65)}),
               std::range_error);

  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(-1)}, tstorage{int64_t(2)})),
            -4);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(-1)}, tstorage{int64_t(1)})),
            -2);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(0)}, tstorage{int64_t(1)})),
            0);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(1)}, tstorage{int64_t(1)})),
            2);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(1)}, tstorage{int64_t(2)})),
            4);
}

TEST(bitwise, shl_int64_t_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      shl(tstorage{int64_t(1)}, tstorage{uint64_t(1)})));
  EXPECT_THROW(shl(tstorage{int64_t(0)}, tstorage{uint64_t(-1)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{int64_t(0)}, tstorage{uint64_t(0)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{int64_t(0)}, tstorage{uint64_t(65)}),
               std::range_error);

  EXPECT_EQ(
      std::get<int64_t>(shl(tstorage{int64_t(-1)}, tstorage{uint64_t(2)})), -4);
  EXPECT_EQ(
      std::get<int64_t>(shl(tstorage{int64_t(-1)}, tstorage{uint64_t(1)})), -2);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(0)}, tstorage{uint64_t(1)})),
            0);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(1)}, tstorage{uint64_t(1)})),
            2);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(1)}, tstorage{uint64_t(2)})),
            4);
}

TEST(bitwise, shl_int64_t_double) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      shl(tstorage{int64_t(1)}, tstorage{double(1)})));

  EXPECT_THROW(shl(tstorage{int64_t(0)}, tstorage{double(-1)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{int64_t(0)}, tstorage{double(0)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{int64_t(0)}, tstorage{double(65)}),
               std::range_error);

  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(-1)}, tstorage{double(2)})),
            -4);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(-1)}, tstorage{double(1)})),
            -2);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(0)}, tstorage{double(1)})),
            0);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(1)}, tstorage{double(1)})),
            2);
  EXPECT_EQ(std::get<int64_t>(shl(tstorage{int64_t(1)}, tstorage{double(2)})),
            4);
}

TEST(bitwise, shl_uint64_t_int64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shl(tstorage{uint64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_THROW(shl(tstorage{uint64_t(0)}, tstorage{int64_t(-1)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{uint64_t(0)}, tstorage{int64_t(0)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{uint64_t(0)}, tstorage{int64_t(65)}),
               std::range_error);

  EXPECT_EQ(
      std::get<uint64_t>(shl(tstorage{uint64_t(0)}, tstorage{int64_t(1)})), 0);
  EXPECT_EQ(
      std::get<uint64_t>(shl(tstorage{uint64_t(1)}, tstorage{int64_t(1)})), 2);
  EXPECT_EQ(
      std::get<uint64_t>(shl(tstorage{uint64_t(1)}, tstorage{int64_t(2)})), 4);
}

TEST(bitwise, shl_uint64_t_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shl(tstorage{uint64_t(1)}, tstorage{uint64_t(1)})));

  EXPECT_THROW(shl(tstorage{uint64_t(0)}, tstorage{uint64_t(0)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{uint64_t(0)}, tstorage{uint64_t(65)}),
               std::range_error);

  EXPECT_EQ(
      std::get<uint64_t>(shl(tstorage{uint64_t(0)}, tstorage{uint64_t(1)})), 0);
  EXPECT_EQ(
      std::get<uint64_t>(shl(tstorage{uint64_t(1)}, tstorage{uint64_t(1)})), 2);
  EXPECT_EQ(
      std::get<uint64_t>(shl(tstorage{uint64_t(1)}, tstorage{uint64_t(2)})), 4);
}

TEST(bitwise, shl_uint64_t_double) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shl(tstorage{uint64_t(1)}, tstorage{double(1)})));

  EXPECT_THROW(shl(tstorage{uint64_t(0)}, tstorage{double(0)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{uint64_t(0)}, tstorage{double(65)}),
               std::range_error);

  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{uint64_t(0)}, tstorage{double(1)})),
            0);
  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{uint64_t(1)}, tstorage{double(1)})),
            2);
  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{uint64_t(1)}, tstorage{double(2)})),
            4);
}

TEST(bitwise, shl_double_int64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shl(tstorage{double(1)}, tstorage{int64_t(1)})));

  EXPECT_THROW(shl(tstorage{double(0)}, tstorage{int64_t(-1)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{double(0)}, tstorage{int64_t(0)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{double(0)}, tstorage{int64_t(65)}),
               std::range_error);

  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{zero}, tstorage{int64_t(1)})), 0);
  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{one}, tstorage{int64_t(1)})), 2);
  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{one}, tstorage{int64_t(2)})), 4);
}

TEST(bitwise, shl_double_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shl(tstorage{double(1)}, tstorage{uint64_t(1)})));

  EXPECT_THROW(shl(tstorage{double(0)}, tstorage{uint64_t(0)}),
               std::range_error);
  EXPECT_THROW(shl(tstorage{double(0)}, tstorage{uint64_t(65)}),
               std::range_error);

  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{zero}, tstorage{uint64_t(1)})), 0);
  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{one}, tstorage{uint64_t(1)})), 2);
  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{one}, tstorage{uint64_t(2)})), 4);
}

TEST(bitwise, shl_double_double) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shl(tstorage{double(1)}, tstorage{double(1)})));

  EXPECT_THROW(shl(tstorage{double(0)}, tstorage{double(0)}), std::range_error);
  EXPECT_THROW(shl(tstorage{double(0)}, tstorage{double(65)}),
               std::range_error);

  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{zero}, tstorage{double(1)})), 0);
  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{one}, tstorage{double(1)})), 2);
  EXPECT_EQ(std::get<uint64_t>(shl(tstorage{one}, tstorage{double(2)})), 4);
}

} // namespace math
} // namespace calculator
