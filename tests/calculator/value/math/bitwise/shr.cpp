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

// We can't directly use the zero, two, and four value for doubles to create
// helpers.  The test shr_double_double validates these are the proper values.
// TODO make constexpr.
const double zero = std::bit_cast<double>(uint64_t(0));
const double two = std::bit_cast<double>(uint64_t(2));
const double four = std::bit_cast<double>(uint64_t(4));

TEST(bitwise, shr_int64_t_int64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      shr(tstorage{int64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_THROW(shr(tstorage{int64_t(0)}, tstorage{int64_t(-1)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{int64_t(0)}, tstorage{int64_t(0)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{int64_t(0)}, tstorage{int64_t(65)}),
               std::range_error);

  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(-4)}, tstorage{int64_t(2)})),
            -1);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(-2)}, tstorage{int64_t(1)})),
            -1);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(0)}, tstorage{int64_t(1)})),
            0);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(2)}, tstorage{int64_t(1)})),
            1);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(4)}, tstorage{int64_t(2)})),
            1);
}

TEST(bitwise, shr_int64_t_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      shr(tstorage{int64_t(1)}, tstorage{uint64_t(1)})));
  EXPECT_THROW(shr(tstorage{int64_t(0)}, tstorage{uint64_t(-1)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{int64_t(0)}, tstorage{uint64_t(0)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{int64_t(0)}, tstorage{uint64_t(65)}),
               std::range_error);

  EXPECT_EQ(
      std::get<int64_t>(shr(tstorage{int64_t(-4)}, tstorage{uint64_t(2)})), -1);
  EXPECT_EQ(
      std::get<int64_t>(shr(tstorage{int64_t(-2)}, tstorage{uint64_t(1)})), -1);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(0)}, tstorage{uint64_t(1)})),
            0);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(2)}, tstorage{uint64_t(1)})),
            1);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(4)}, tstorage{uint64_t(2)})),
            1);
}

TEST(bitwise, shr_int64_t_double) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      shr(tstorage{int64_t(1)}, tstorage{double(1)})));

  EXPECT_THROW(shr(tstorage{int64_t(0)}, tstorage{double(-1)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{int64_t(0)}, tstorage{double(0)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{int64_t(0)}, tstorage{double(65)}),
               std::range_error);

  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(-4)}, tstorage{double(2)})),
            -1);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(-2)}, tstorage{double(1)})),
            -1);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(0)}, tstorage{double(1)})),
            0);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(2)}, tstorage{double(1)})),
            1);
  EXPECT_EQ(std::get<int64_t>(shr(tstorage{int64_t(4)}, tstorage{double(2)})),
            1);
}

TEST(bitwise, shr_uint64_t_int64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shr(tstorage{uint64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_THROW(shr(tstorage{uint64_t(0)}, tstorage{int64_t(-1)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{uint64_t(0)}, tstorage{int64_t(0)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{uint64_t(0)}, tstorage{int64_t(65)}),
               std::range_error);

  EXPECT_EQ(
      std::get<uint64_t>(shr(tstorage{uint64_t(0)}, tstorage{int64_t(1)})), 0);
  EXPECT_EQ(
      std::get<uint64_t>(shr(tstorage{uint64_t(2)}, tstorage{int64_t(1)})), 1);
  EXPECT_EQ(
      std::get<uint64_t>(shr(tstorage{uint64_t(4)}, tstorage{int64_t(2)})), 1);
}

TEST(bitwise, shr_uint64_t_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shr(tstorage{uint64_t(1)}, tstorage{uint64_t(1)})));

  EXPECT_THROW(shr(tstorage{uint64_t(0)}, tstorage{uint64_t(0)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{uint64_t(0)}, tstorage{uint64_t(65)}),
               std::range_error);

  EXPECT_EQ(
      std::get<uint64_t>(shr(tstorage{uint64_t(0)}, tstorage{uint64_t(1)})), 0);
  EXPECT_EQ(
      std::get<uint64_t>(shr(tstorage{uint64_t(2)}, tstorage{uint64_t(1)})), 1);
  EXPECT_EQ(
      std::get<uint64_t>(shr(tstorage{uint64_t(4)}, tstorage{uint64_t(2)})), 1);
}

TEST(bitwise, shr_uint64_t_double) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shr(tstorage{uint64_t(1)}, tstorage{double(1)})));

  EXPECT_THROW(shr(tstorage{uint64_t(0)}, tstorage{double(0)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{uint64_t(0)}, tstorage{double(65)}),
               std::range_error);

  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{zero}, tstorage{double(1)})), 0);
  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{two}, tstorage{double(1)})), 1);
  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{four}, tstorage{double(2)})), 1);
}

TEST(bitwise, shr_double_int64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shr(tstorage{double(1)}, tstorage{int64_t(1)})));

  EXPECT_THROW(shr(tstorage{double(0)}, tstorage{int64_t(-1)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{double(0)}, tstorage{int64_t(0)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{double(0)}, tstorage{int64_t(65)}),
               std::range_error);

  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{zero}, tstorage{int64_t(1)})), 0);
  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{two}, tstorage{int64_t(1)})), 1);
  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{four}, tstorage{int64_t(2)})), 1);
}

TEST(bitwise, shr_double_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shr(tstorage{double(1)}, tstorage{uint64_t(1)})));

  EXPECT_THROW(shr(tstorage{double(0)}, tstorage{uint64_t(0)}),
               std::range_error);
  EXPECT_THROW(shr(tstorage{double(0)}, tstorage{uint64_t(65)}),
               std::range_error);

  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{zero}, tstorage{uint64_t(1)})), 0);
  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{two}, tstorage{uint64_t(1)})), 1);
  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{four}, tstorage{uint64_t(2)})), 1);
}

TEST(bitwise, shr_double_double) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      shr(tstorage{double(1)}, tstorage{double(1)})));

  EXPECT_THROW(shr(tstorage{double(0)}, tstorage{double(0)}), std::range_error);
  EXPECT_THROW(shr(tstorage{double(0)}, tstorage{double(65)}),
               std::range_error);

  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{zero}, tstorage{double(1)})), 0);
  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{two}, tstorage{double(1)})), 1);
  EXPECT_EQ(std::get<uint64_t>(shr(tstorage{four}, tstorage{double(2)})), 1);
}

} // namespace math
} // namespace calculator
