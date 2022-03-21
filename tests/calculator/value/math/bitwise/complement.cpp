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

import<bit>;

#include <gtest/gtest.h>

namespace calculator {
namespace math {

TEST(bitwise, complement_int64_t) {
  ASSERT_TRUE(
      std::holds_alternative<int64_t>(complement(tstorage{int64_t(1)})));
  EXPECT_EQ(std::get<int64_t>(complement(tstorage{int64_t(0)})), -1);
  EXPECT_EQ(std::get<int64_t>(complement(tstorage{int64_t(-1)})), 0);
  EXPECT_EQ(std::get<int64_t>(complement(tstorage{int64_t(1)})), -2);
  EXPECT_EQ(std::get<int64_t>(complement(tstorage{int64_t(-2)})), 1);
}

TEST(bitwise, complement_uint64_t) {
  ASSERT_TRUE(
      std::holds_alternative<uint64_t>(complement(tstorage{uint64_t(1)})));
  EXPECT_EQ(std::get<uint64_t>(complement(tstorage{uint64_t(0)})),
            0xffff'ffff'ffff'ffffu);
  EXPECT_EQ(std::get<uint64_t>(
                complement(tstorage{uint64_t(0xffff'ffff'ffff'ffffu)})),
            0);
  EXPECT_EQ(std::get<uint64_t>(complement(tstorage{uint64_t(1)})),
            0xffff'ffff'ffff'fffeu);
  EXPECT_EQ(std::get<uint64_t>(
                complement(tstorage{uint64_t(0xffff'ffff'ffff'fffeu)})),
            1);
}

TEST(bitwise, complement_double) {
  ASSERT_TRUE(
      std::holds_alternative<uint64_t>(complement(tstorage{double(1)})));

  EXPECT_EQ(std::get<uint64_t>(complement(tstorage{
                std::bit_cast<uint64_t>(std::bit_cast<double>(uint64_t(0)))})),
            0xffff'ffff'ffff'ffffu);
  EXPECT_EQ(std::get<uint64_t>(complement(tstorage{std::bit_cast<uint64_t>(
                std::bit_cast<double>(uint64_t(0xffff'ffff'ffff'ffffu)))})),
            0);
  EXPECT_EQ(std::get<uint64_t>(complement(tstorage{
                std::bit_cast<uint64_t>(std::bit_cast<double>(uint64_t(1)))})),
            0xffff'ffff'ffff'fffeu);
  EXPECT_EQ(std::get<uint64_t>(complement(tstorage{std::bit_cast<uint64_t>(
                std::bit_cast<double>(uint64_t(0xffff'ffff'ffff'fffeu)))})),
            1);
}

} // namespace math
} // namespace calculator
