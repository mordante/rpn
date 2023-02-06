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

TEST(arithmetic, mod_int64_t_int64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      mod(tstorage{int64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(std::get<int64_t>(mod(tstorage{int64_t(4)}, tstorage{int64_t(3)})),
            1);
  EXPECT_EQ(std::get<int64_t>(mod(tstorage{int64_t(4)}, tstorage{int64_t(-3)})),
            1);
  EXPECT_EQ(std::get<int64_t>(mod(tstorage{int64_t(-1)}, tstorage{int64_t(3)})),
            -1);
  EXPECT_EQ(
      std::get<int64_t>(mod(tstorage{int64_t(-4)}, tstorage{int64_t(-3)})), -1);

  EXPECT_THROW(mod(tstorage{int64_t(3)}, tstorage{int64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, mod_int64_t_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      mod(tstorage{int64_t(4)}, tstorage{uint64_t(3)})));

  EXPECT_EQ(
      std::get<uint64_t>(mod(tstorage{int64_t(4)}, tstorage{uint64_t(3)})), 1);

  ASSERT_TRUE(std::holds_alternative<int64_t>(
      mod(tstorage{int64_t(-4)}, tstorage{uint64_t(3)})));
  EXPECT_EQ(
      std::get<int64_t>(mod(tstorage{int64_t(-4)}, tstorage{uint64_t(3)})), -1);

  EXPECT_THROW(mod(tstorage{int64_t(3)}, tstorage{uint64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, mod_int64_t_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mod(tstorage{int64_t(1)}, tstorage{double(1)})));

  EXPECT_EQ(std::get<double>(mod(tstorage{int64_t(4)}, tstorage{double(1.5)})),
            1.);
  EXPECT_EQ(std::get<double>(mod(tstorage{int64_t(4)}, tstorage{double(-1.5)})),
            1.);
  EXPECT_EQ(std::get<double>(mod(tstorage{int64_t(-4)}, tstorage{double(1.5)})),
            -1.);
  EXPECT_EQ(
      std::get<double>(mod(tstorage{int64_t(-4)}, tstorage{double(-1.5)})),
      -1.);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{int64_t(1)}, tstorage{limit::quiet_NaN()}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{int64_t(1)}, tstorage{limit::signaling_NaN()}))));

  EXPECT_THROW(mod(tstorage{int64_t(3)}, tstorage{double(0)}),
               std::domain_error);
}

TEST(arithmetic, mod_uint64_t_int64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      mod(tstorage{uint64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(
      std::get<uint64_t>(mod(tstorage{uint64_t(4)}, tstorage{int64_t(3)})), 1);

  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      mod(tstorage{uint64_t(4)}, tstorage{int64_t(-3)})));

  EXPECT_EQ(
      std::get<uint64_t>(mod(tstorage{uint64_t(4)}, tstorage{int64_t(-3)})), 1);

  EXPECT_THROW(mod(tstorage{uint64_t(3)}, tstorage{int64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, mod_uint64_t_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      mod(tstorage{uint64_t(4)}, tstorage{uint64_t(3)})));

  EXPECT_EQ(
      std::get<uint64_t>(mod(tstorage{uint64_t(4)}, tstorage{uint64_t(3)})), 1);

  EXPECT_THROW(mod(tstorage{uint64_t(3)}, tstorage{uint64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, mod_double_int64_t) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mod(tstorage{double(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(std::get<double>(mod(tstorage{double(4.5)}, tstorage{int64_t(3)})),
            1.5);
  EXPECT_EQ(std::get<double>(mod(tstorage{double(4.5)}, tstorage{int64_t(-3)})),
            1.5);
  EXPECT_EQ(std::get<double>(mod(tstorage{double(-4.5)}, tstorage{int64_t(3)})),
            -1.5);
  EXPECT_EQ(
      std::get<double>(mod(tstorage{double(-4.5)}, tstorage{int64_t(-3)})),
      -1.5);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{-limit::infinity()}, tstorage{int64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{limit::infinity()}, tstorage{int64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{limit::quiet_NaN()}, tstorage{int64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{limit::signaling_NaN()}, tstorage{int64_t(1)}))));

  EXPECT_THROW(mod(tstorage{double(3)}, tstorage{int64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, mod_double_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mod(tstorage{double(1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(std::get<double>(mod(tstorage{double(4.5)}, tstorage{uint64_t(3)})),
            1.5);
  EXPECT_EQ(
      std::get<double>(mod(tstorage{double(-4.5)}, tstorage{uint64_t(3)})),
      -1.5);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{-limit::infinity()}, tstorage{uint64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{limit::infinity()}, tstorage{uint64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{limit::quiet_NaN()}, tstorage{uint64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{limit::signaling_NaN()}, tstorage{uint64_t(1)}))));

  EXPECT_THROW(mod(tstorage{double(3)}, tstorage{uint64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, mod_double_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mod(tstorage{double(1)}, tstorage{double(1)})));

  EXPECT_EQ(
      std::get<double>(mod(tstorage{double(4.25)}, tstorage{double(2.5)})),
      1.75);
  EXPECT_EQ(
      std::get<double>(mod(tstorage{double(4.25)}, tstorage{double(-2.5)})),
      1.75);
  EXPECT_EQ(
      std::get<double>(mod(tstorage{double(-4.25)}, tstorage{double(2.5)})),
      -1.75);
  EXPECT_EQ(
      std::get<double>(mod(tstorage{double(-4.25)}, tstorage{double(-2.5)})),
      -1.75);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{-limit::infinity()}, tstorage{double(1)}))));
  EXPECT_TRUE(isnan(
      std::get<double>(mod(tstorage{limit::infinity()}, tstorage{double(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{limit::quiet_NaN()}, tstorage{double(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mod(tstorage{limit::signaling_NaN()}, tstorage{double(1)}))));

  EXPECT_THROW(mod(tstorage{double(3)}, tstorage{double(0)}),
               std::domain_error);
}

} // namespace math
} // namespace calculator
