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

#include <bit>

#include <gtest/gtest.h>

namespace calculator {
namespace math {

// We can't directly use the zero and one value for doubles to create helpers.
// The test xor_double_double validates these are the proper values.
// TODO make constexpr.
const double zero = std::bit_cast<double>(uint64_t(0));
const double one = std::bit_cast<double>(uint64_t(1));

TEST(bitwise, xor_int64_t_int64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      bit_xor(tstorage{int64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(
      std::get<int64_t>(bit_xor(tstorage{int64_t(0)}, tstorage{int64_t(0)})),
      0);
  EXPECT_EQ(
      std::get<int64_t>(bit_xor(tstorage{int64_t(0)}, tstorage{int64_t(1)})),
      1);
  EXPECT_EQ(
      std::get<int64_t>(bit_xor(tstorage{int64_t(1)}, tstorage{int64_t(0)})),
      1);
  EXPECT_EQ(
      std::get<int64_t>(bit_xor(tstorage{int64_t(1)}, tstorage{int64_t(1)})),
      0);
}

TEST(bitwise, xor_int64_t_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      bit_xor(tstorage{int64_t(1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{int64_t(0)}, tstorage{uint64_t(0)})),
      0);
  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{int64_t(0)}, tstorage{uint64_t(1)})),
      1);
  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{int64_t(1)}, tstorage{uint64_t(0)})),
      1);
  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{int64_t(1)}, tstorage{uint64_t(1)})),
      0);
}

TEST(bitwise, xor_int64_t_double) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      bit_xor(tstorage{int64_t(1)}, tstorage{double(1)})));

  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{int64_t(0)}, tstorage{zero})),
            0);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{int64_t(0)}, tstorage{one})),
            1);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{int64_t(1)}, tstorage{zero})),
            1);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{int64_t(1)}, tstorage{one})),
            0);
}

TEST(bitwise, xor_uint64_t_int64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      bit_xor(tstorage{uint64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{uint64_t(0)}, tstorage{int64_t(0)})),
      0);
  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{uint64_t(0)}, tstorage{int64_t(1)})),
      1);
  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{uint64_t(1)}, tstorage{int64_t(0)})),
      1);
  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{uint64_t(1)}, tstorage{int64_t(1)})),
      0);
}

TEST(bitwise, xor_uint64_t_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      bit_xor(tstorage{uint64_t(1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{uint64_t(0)}, tstorage{uint64_t(0)})),
      0);
  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{uint64_t(0)}, tstorage{uint64_t(1)})),
      1);
  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{uint64_t(1)}, tstorage{uint64_t(0)})),
      1);
  EXPECT_EQ(
      std::get<uint64_t>(bit_xor(tstorage{uint64_t(1)}, tstorage{uint64_t(1)})),
      0);
}

TEST(bitwise, xor_uint64_t_double) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      bit_xor(tstorage{uint64_t(1)}, tstorage{double(1)})));

  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{uint64_t(0)}, tstorage{zero})),
            0);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{uint64_t(0)}, tstorage{one})),
            1);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{uint64_t(1)}, tstorage{zero})),
            1);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{uint64_t(1)}, tstorage{one})),
            0);
}

TEST(bitwise, xor_double_int64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      bit_xor(tstorage{double(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{zero}, tstorage{int64_t(0)})),
            0);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{one}, tstorage{int64_t(0)})),
            1);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{zero}, tstorage{int64_t(1)})),
            1);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{one}, tstorage{int64_t(1)})),
            0);
}

TEST(bitwise, xor_double_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      bit_xor(tstorage{double(1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{zero}, tstorage{uint64_t(0)})),
            0);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{one}, tstorage{uint64_t(0)})),
            1);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{zero}, tstorage{uint64_t(1)})),
            1);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{one}, tstorage{uint64_t(1)})),
            0);
}

TEST(bitwise, xor_double_double) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      bit_xor(tstorage{double(1)}, tstorage{double(1)})));

  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{zero}, tstorage{zero})), 0);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{zero}, tstorage{one})), 1);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{one}, tstorage{zero})), 1);
  EXPECT_EQ(std::get<uint64_t>(bit_xor(tstorage{one}, tstorage{one})), 0);
}

} // namespace math
} // namespace calculator
