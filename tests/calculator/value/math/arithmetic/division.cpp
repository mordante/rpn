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

TEST(arithmetic, div_int64_t_int64_t) {
  ASSERT_TRUE(std::holds_alternative<double>(
      div(tstorage{int64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(std::get<double>(div(tstorage{int64_t(-1)}, tstorage{int64_t(-1)})),
            1.);
  EXPECT_EQ(std::get<double>(div(tstorage{int64_t(0)}, tstorage{int64_t(-1)})),
            0.);
  EXPECT_EQ(std::get<double>(div(tstorage{int64_t(-1)}, tstorage{int64_t(1)})),
            -1.);
  EXPECT_EQ(std::get<double>(div(tstorage{int64_t(3)}, tstorage{int64_t(2)})),
            1.5);

  EXPECT_THROW(div(tstorage{int64_t(3)}, tstorage{int64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, div_int64_t_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<double>(
      div(tstorage{int64_t(1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(std::get<double>(div(tstorage{int64_t(1)}, tstorage{uint64_t(1)})),
            1);
  EXPECT_EQ(std::get<double>(div(tstorage{int64_t(3)}, tstorage{uint64_t(2)})),
            1.5);

  EXPECT_THROW(div(tstorage{int64_t(3)}, tstorage{uint64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, div_int64_t_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      div(tstorage{int64_t(1)}, tstorage{double(1)})));

  ASSERT_EQ(std::get<double>(div(tstorage{int64_t(-1)}, tstorage{double(-1)})),
            1.);
  ASSERT_EQ(std::get<double>(div(tstorage{int64_t(0)}, tstorage{double(1)})),
            0.);
  ASSERT_EQ(std::get<double>(div(tstorage{int64_t(3)}, tstorage{double(2)})),
            1.5);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isnan(std::get<double>(
      div(tstorage{int64_t(1)}, tstorage{limit::quiet_NaN()}))));
  EXPECT_TRUE(isnan(std::get<double>(
      div(tstorage{int64_t(1)}, tstorage{limit::signaling_NaN()}))));

  EXPECT_THROW(div(tstorage{int64_t(3)}, tstorage{double(0)}),
               std::domain_error);
}

TEST(arithmetic, div_uint64_t_int64_t) {
  ASSERT_TRUE(std::holds_alternative<double>(
      div(tstorage{uint64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(std::get<double>(div(tstorage{uint64_t(3)}, tstorage{int64_t(2)})),
            1.5);

  EXPECT_THROW(div(tstorage{uint64_t(3)}, tstorage{int64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, div_uint64_t_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<double>(
      div(tstorage{uint64_t(1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(std::get<double>(div(tstorage{uint64_t(3)}, tstorage{uint64_t(2)})),
            1.5);

  EXPECT_THROW(div(tstorage{uint64_t(3)}, tstorage{uint64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, div_double_int64_t) {
  ASSERT_TRUE(std::holds_alternative<double>(
      div(tstorage{double(1)}, tstorage{int64_t(1)})));

  ASSERT_EQ(std::get<double>(div(tstorage{double(-3)}, tstorage{int64_t(-2)})),
            1.5);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isinf(std::get<double>(
      div(tstorage{-limit::infinity()}, tstorage{int64_t(1)}))));
  EXPECT_TRUE(isinf(std::get<double>(
      div(tstorage{limit::infinity()}, tstorage{int64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      div(tstorage{limit::quiet_NaN()}, tstorage{int64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      div(tstorage{limit::signaling_NaN()}, tstorage{int64_t(1)}))));

  EXPECT_THROW(div(tstorage{double(3)}, tstorage{int64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, div_double_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<double>(
      div(tstorage{double(1)}, tstorage{uint64_t(1)})));

  ASSERT_EQ(std::get<double>(div(tstorage{double(-3)}, tstorage{uint64_t(2)})),
            -1.5);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isinf(std::get<double>(
      div(tstorage{-limit::infinity()}, tstorage{uint64_t(1)}))));
  EXPECT_TRUE(isinf(std::get<double>(
      div(tstorage{limit::infinity()}, tstorage{uint64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      div(tstorage{limit::quiet_NaN()}, tstorage{uint64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      div(tstorage{limit::signaling_NaN()}, tstorage{uint64_t(1)}))));

  EXPECT_THROW(div(tstorage{double(3)}, tstorage{uint64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, div_double_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      div(tstorage{double(1)}, tstorage{double(1)})));

  ASSERT_EQ(std::get<double>(div(tstorage{double(-3)}, tstorage{double(-2)})),
            1.5);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isinf(std::get<double>(
      div(tstorage{-limit::infinity()}, tstorage{double(1)}))));
  EXPECT_TRUE(isinf(
      std::get<double>(div(tstorage{limit::infinity()}, tstorage{double(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      div(tstorage{limit::quiet_NaN()}, tstorage{double(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      div(tstorage{limit::signaling_NaN()}, tstorage{double(1)}))));

  EXPECT_THROW(div(tstorage{double(3)}, tstorage{double(0)}),
               std::domain_error);
}

} // namespace math
} // namespace calculator
