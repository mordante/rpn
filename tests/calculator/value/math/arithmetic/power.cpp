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

import <cmath>;
import <limits>;

#include <gtest/gtest.h>

namespace calculator {
namespace math {

TEST(arithmetic, pow_x_int64_t_result_int64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(pow<2>(tstorage{int64_t(-1)})));
  ASSERT_TRUE(std::holds_alternative<int64_t>(pow<3>(tstorage{int64_t(-1)})));

  EXPECT_EQ(std::get<int64_t>(pow<2>(tstorage{int64_t(0)})), 0);

  EXPECT_EQ(std::get<int64_t>(pow<2>(tstorage{int64_t(-1)})), 1);
  EXPECT_EQ(std::get<int64_t>(pow<2>(tstorage{int64_t(1)})), 1);

  EXPECT_EQ(std::get<int64_t>(pow<2>(tstorage{int64_t(-2)})), 4);
  EXPECT_EQ(std::get<int64_t>(pow<3>(tstorage{int64_t(-2)})), -8);
  EXPECT_EQ(std::get<int64_t>(pow<4>(tstorage{int64_t(-2)})), 16);
  EXPECT_EQ(std::get<int64_t>(pow<5>(tstorage{int64_t(-2)})), -32);
  EXPECT_EQ(std::get<int64_t>(pow<6>(tstorage{int64_t(-2)})), 64);
  EXPECT_EQ(std::get<int64_t>(pow<7>(tstorage{int64_t(-2)})), -128);
  EXPECT_EQ(std::get<int64_t>(pow<8>(tstorage{int64_t(-2)})), 256);
  EXPECT_EQ(std::get<int64_t>(pow<9>(tstorage{int64_t(-2)})), -512);
}

TEST(arithmetic, pow_x_int64_t_result_uint64_t) {
  ASSERT_TRUE(
      std::holds_alternative<uint64_t>(pow<2>(tstorage{int64_t(0xf000'0000)})));
  EXPECT_EQ(std::get<uint64_t>(pow<2>(tstorage{int64_t(0xf000'0000)})),
            0xe100'0000'0000'0000);

  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      pow<2>(tstorage{-1 * int64_t(0xf000'0000)})));
  EXPECT_EQ(std::get<uint64_t>(pow<2>(tstorage{-1 * int64_t(0xf000'0000)})),
            0xe100'0000'0000'0000);
}

TEST(arithmetic, pow_x_int64_t_result_double) {
  // Overflow after last iteration
  ASSERT_TRUE(std::holds_alternative<double>(
      pow<2>(tstorage{int64_t(0x7fff'ffff'ffff'ffff)})));

  EXPECT_EQ(std::get<double>(pow<2>(tstorage{int64_t(0x7fff'ffff'ffff'ffff)})),
            std::pow(double(int64_t(0x7fff'ffff'ffff'ffff)), 2.0));

  // Overflow during iterating
  ASSERT_TRUE(
      std::holds_alternative<double>(pow<8>(tstorage{int64_t(2147483648)})));

  EXPECT_EQ(std::get<double>(pow<8>(tstorage{int64_t(2147483648)})),
            std::pow(double(int64_t(2147483648)), 8.0));
}

TEST(arithmetic, pow_x_uint64_t_result_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(pow<2>(tstorage{uint64_t(1)})));

  EXPECT_EQ(std::get<uint64_t>(pow<2>(tstorage{uint64_t(0)})), 0);

  EXPECT_EQ(std::get<uint64_t>(pow<2>(tstorage{uint64_t(1)})), 1);

  EXPECT_EQ(std::get<uint64_t>(pow<2>(tstorage{uint64_t(2)})), 4);
  EXPECT_EQ(std::get<uint64_t>(pow<3>(tstorage{uint64_t(2)})), 8);
  EXPECT_EQ(std::get<uint64_t>(pow<4>(tstorage{uint64_t(2)})), 16);
  EXPECT_EQ(std::get<uint64_t>(pow<5>(tstorage{uint64_t(2)})), 32);
  EXPECT_EQ(std::get<uint64_t>(pow<6>(tstorage{uint64_t(2)})), 64);
  EXPECT_EQ(std::get<uint64_t>(pow<7>(tstorage{uint64_t(2)})), 128);
  EXPECT_EQ(std::get<uint64_t>(pow<8>(tstorage{uint64_t(2)})), 256);
  EXPECT_EQ(std::get<uint64_t>(pow<9>(tstorage{uint64_t(2)})), 512);
}

TEST(arithmetic, pow_x_uint64_t_result_double) {
  // Overflow after last iteration
  ASSERT_TRUE(std::holds_alternative<double>(pow<2>(tstorage{uint64_t(-1)})));

  EXPECT_EQ(std::get<double>(pow<2>(tstorage{uint64_t(-1)})),
            std::pow(double(uint64_t(-1)), 2.0));

  // Overflow during iterating
  ASSERT_TRUE(
      std::holds_alternative<double>(pow<8>(tstorage{uint64_t(4294967296)})));

  EXPECT_EQ(std::get<double>(pow<8>(tstorage{uint64_t(4294967296)})),
            std::pow(double(uint64_t(4294967296)), 8.0));
}

TEST(arithmetic, pow_x_double_result_double) {
  ASSERT_TRUE(std::holds_alternative<double>(pow<2>(tstorage{-1.})));
  ASSERT_TRUE(std::holds_alternative<double>(pow<3>(tstorage{-1.})));

  EXPECT_EQ(std::get<double>(pow<2>(tstorage{0.})), 0);

  EXPECT_EQ(std::get<double>(pow<2>(tstorage{-1.})), 1);
  EXPECT_EQ(std::get<double>(pow<2>(tstorage{1.})), 1);

  EXPECT_EQ(std::get<double>(pow<2>(tstorage{-2.})), 4);
  EXPECT_EQ(std::get<double>(pow<3>(tstorage{-2.})), -8);
  EXPECT_EQ(std::get<double>(pow<4>(tstorage{-2.})), 16);
  EXPECT_EQ(std::get<double>(pow<5>(tstorage{-2.})), -32);
  EXPECT_EQ(std::get<double>(pow<6>(tstorage{-2.})), 64);
  EXPECT_EQ(std::get<double>(pow<7>(tstorage{-2.})), -128);
  EXPECT_EQ(std::get<double>(pow<8>(tstorage{-2.})), 256);
  EXPECT_EQ(std::get<double>(pow<9>(tstorage{-2.})), -512);
}

TEST(arithmetic, pow_int64_t_int64_t_result_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      pow(tstorage{int64_t(-1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(0)}, tstorage{int64_t(2)})),
            0);

  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-1)}, tstorage{int64_t(2)})),
            1);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(1)}, tstorage{int64_t(2)})),
            1);

  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{int64_t(2)})),
            4);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{int64_t(3)})),
            -8);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{int64_t(4)})),
            16);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{int64_t(5)})),
            -32);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{int64_t(6)})),
            64);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{int64_t(7)})),
            -128);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{int64_t(8)})),
            256);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{int64_t(9)})),
            -512);
}

TEST(arithmetic, pow_int64_t_uint64_t_result_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      pow(tstorage{int64_t(-1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(0)}, tstorage{uint64_t(2)})),
            0);

  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-1)}, tstorage{uint64_t(2)})),
            1);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(1)}, tstorage{uint64_t(2)})),
            1);

  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{uint64_t(2)})),
            4);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{uint64_t(3)})),
            -8);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{uint64_t(4)})),
            16);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{uint64_t(5)})),
            -32);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{uint64_t(6)})),
            64);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{uint64_t(7)})),
            -128);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{uint64_t(8)})),
            256);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{uint64_t(9)})),
            -512);
}

TEST(arithmetic, pow_int64_t_double_result_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      pow(tstorage{int64_t(-1)}, tstorage{double(1)})));

  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(0)}, tstorage{double(2)})),
            0);

  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-1)}, tstorage{double(2)})),
            1);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(1)}, tstorage{double(2)})),
            1);

  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{double(2)})),
            4);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{double(3)})),
            -8);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{double(4)})),
            16);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{double(5)})),
            -32);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{double(6)})),
            64);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{double(7)})),
            -128);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{double(8)})),
            256);
  EXPECT_EQ(std::get<double>(pow(tstorage{int64_t(-2)}, tstorage{double(9)})),
            -512);
}

TEST(arithmetic, pow_uint64_t_int64_t_result_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      pow(tstorage{uint64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(0)}, tstorage{int64_t(2)})),
            0);

  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(1)}, tstorage{int64_t(2)})),
            1);

  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{int64_t(2)})),
            4);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{int64_t(3)})),
            8);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{int64_t(4)})),
            16);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{int64_t(5)})),
            32);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{int64_t(6)})),
            64);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{int64_t(7)})),
            128);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{int64_t(8)})),
            256);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{int64_t(9)})),
            512);
}

TEST(arithmetic, pow_uint64_t_uint64_t_result_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      pow(tstorage{uint64_t(1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(0)}, tstorage{uint64_t(2)})),
            0);

  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(1)}, tstorage{uint64_t(2)})),
            1);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(1)}, tstorage{uint64_t(2)})),
            1);

  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{uint64_t(2)})),
            4);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{uint64_t(3)})),
            8);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{uint64_t(4)})),
            16);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{uint64_t(5)})),
            32);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{uint64_t(6)})),
            64);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{uint64_t(7)})),
            128);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{uint64_t(8)})),
            256);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{uint64_t(9)})),
            512);
}

TEST(arithmetic, pow_uint64_t_double_result_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      pow(tstorage{uint64_t(1)}, tstorage{double(1)})));

  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(0)}, tstorage{double(2)})),
            0);

  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(1)}, tstorage{double(2)})),
            1);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(1)}, tstorage{double(2)})),
            1);

  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{double(2)})),
            4);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{double(3)})),
            8);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{double(4)})),
            16);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{double(5)})),
            32);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{double(6)})),
            64);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{double(7)})),
            128);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{double(8)})),
            256);
  EXPECT_EQ(std::get<double>(pow(tstorage{uint64_t(2)}, tstorage{double(9)})),
            512);
}

TEST(arithmetic, pow_double_int64_t_result_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      pow(tstorage{double(-1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(std::get<double>(pow(tstorage{double(0)}, tstorage{int64_t(2)})),
            0);

  EXPECT_EQ(std::get<double>(pow(tstorage{double(-1)}, tstorage{int64_t(2)})),
            1);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(1)}, tstorage{int64_t(2)})),
            1);

  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{int64_t(2)})),
            4);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{int64_t(3)})),
            -8);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{int64_t(4)})),
            16);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{int64_t(5)})),
            -32);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{int64_t(6)})),
            64);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{int64_t(7)})),
            -128);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{int64_t(8)})),
            256);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{int64_t(9)})),
            -512);
}

TEST(arithmetic, pow_double_uint64_t_result_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      pow(tstorage{double(-1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(std::get<double>(pow(tstorage{double(0)}, tstorage{uint64_t(2)})),
            0);

  EXPECT_EQ(std::get<double>(pow(tstorage{double(-1)}, tstorage{uint64_t(2)})),
            1);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(1)}, tstorage{uint64_t(2)})),
            1);

  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{uint64_t(2)})),
            4);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{uint64_t(3)})),
            -8);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{uint64_t(4)})),
            16);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{uint64_t(5)})),
            -32);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{uint64_t(6)})),
            64);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{uint64_t(7)})),
            -128);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{uint64_t(8)})),
            256);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{uint64_t(9)})),
            -512);
}

TEST(arithmetic, pow_double_double_result_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      pow(tstorage{double(-1)}, tstorage{double(1)})));

  EXPECT_EQ(std::get<double>(pow(tstorage{double(0)}, tstorage{double(2)})), 0);

  EXPECT_EQ(std::get<double>(pow(tstorage{double(-1)}, tstorage{double(2)})),
            1);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(1)}, tstorage{double(2)})), 1);

  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{double(2)})),
            4);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{double(3)})),
            -8);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{double(4)})),
            16);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{double(5)})),
            -32);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{double(6)})),
            64);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{double(7)})),
            -128);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{double(8)})),
            256);
  EXPECT_EQ(std::get<double>(pow(tstorage{double(-2)}, tstorage{double(9)})),
            -512);
}

} // namespace math
} // namespace calculator
