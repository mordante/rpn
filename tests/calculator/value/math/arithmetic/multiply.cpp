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

import<cmath>;
import<limits>;

#include <gtest/gtest.h>

namespace calculator {
namespace math {

TEST(arithmetic, mul_int64_t_int64_t_result_int64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      mul(tstorage{int64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(
      std::get<int64_t>(mul(tstorage{int64_t(-1)}, tstorage{int64_t(-1)})), 1);
  EXPECT_EQ(std::get<int64_t>(mul(tstorage{int64_t(0)}, tstorage{int64_t(-1)})),
            0);
  EXPECT_EQ(std::get<int64_t>(mul(tstorage{int64_t(-1)}, tstorage{int64_t(0)})),
            0);
  EXPECT_EQ(std::get<int64_t>(mul(tstorage{int64_t(0)}, tstorage{int64_t(0)})),
            0);
  EXPECT_EQ(std::get<int64_t>(mul(tstorage{int64_t(-1)}, tstorage{int64_t(1)})),
            -1);
  EXPECT_EQ(std::get<int64_t>(mul(tstorage{int64_t(1)}, tstorage{int64_t(-1)})),
            -1);
  EXPECT_EQ(std::get<int64_t>(mul(tstorage{int64_t(1)}, tstorage{int64_t(1)})),
            1);

  EXPECT_EQ(std::get<int64_t>(mul(tstorage{std::numeric_limits<int64_t>::min()},
                                  tstorage{int64_t(1)})),
            std::numeric_limits<int64_t>::min());

  EXPECT_EQ(std::get<int64_t>(mul(tstorage{std::numeric_limits<int64_t>::max()},
                                  tstorage{int64_t(1)})),
            std::numeric_limits<int64_t>::max());
}

TEST(arithmetic, mul_int64_t_int64_t_result_overflow_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(mul(
      tstorage{std::numeric_limits<int64_t>::max()}, tstorage{int64_t(2)})));

  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{std::numeric_limits<int64_t>::min() / 2},
                             tstorage{int64_t(-2)})),
      static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + uint64_t(1));
  EXPECT_EQ(std::get<uint64_t>(
                mul(tstorage{std::numeric_limits<int64_t>::max() / 2 + 1},
                    tstorage{int64_t(2)})),
            static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) +
                uint64_t(1));
}

TEST(arithmetic, mul_int64_t_int64_t_result_underflow_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{(std::numeric_limits<int64_t>::min() / 2) - 1},
          tstorage{int64_t(2)})));

  EXPECT_EQ(std::get<double>(
                mul(tstorage{(std::numeric_limits<int64_t>::min() / 2) - 1},
                    tstorage{int64_t(2)})),
            double((__int128_t(std::numeric_limits<int64_t>::min()) - 2)));
}

TEST(arithmetic, mul_int64_t_int64_t_result_overflow_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{(std::numeric_limits<int64_t>::max()) + 2},
          tstorage{int64_t(2)})));

  EXPECT_EQ(std::get<double>(
                mul(tstorage{(std::numeric_limits<int64_t>::max() / 2) + 2},
                    tstorage{int64_t(4)})),
            double(__int128_t(std::numeric_limits<uint64_t>::max()) + 3));
}

TEST(arithmetic, mul_int64_t_uint64_t_result_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      mul(tstorage{int64_t(1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{int64_t(1)}, tstorage{uint64_t(1)})), 1);
  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{int64_t(0)}, tstorage{uint64_t(0)})), 0);

  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{std::numeric_limits<int64_t>::max()},
                             tstorage{uint64_t(2)})),
      std::numeric_limits<uint64_t>::max() - 1);
}

TEST(arithmetic, mul_int64_t_uint64_t_result_negative_int64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      mul(tstorage{int64_t(-1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(
      std::get<int64_t>(mul(tstorage{int64_t(-1)}, tstorage{uint64_t(1)})), -1);

  ASSERT_TRUE(std::holds_alternative<int64_t>(
      mul(tstorage{std::numeric_limits<int64_t>::min() / 2},
          tstorage{uint64_t(2)})));

  EXPECT_EQ(
      std::get<int64_t>(mul(tstorage{std::numeric_limits<int64_t>::min() / 2},
                            tstorage{uint64_t(2)})),
      std::numeric_limits<int64_t>::min());
}

TEST(arithmetic, mul_int64_t_uint64_t_result_underflow_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{(std::numeric_limits<int64_t>::min() / 2) - 1},
          tstorage{uint64_t(2)})));

  EXPECT_EQ(std::get<double>(
                mul(tstorage{(std::numeric_limits<int64_t>::min() / 2) - 1},
                    tstorage{uint64_t(2)})),
            double((__int128_t(std::numeric_limits<int64_t>::min()) - 2)));
}

TEST(arithmetic, mul_int64_t_uint64_t_result_overflow_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{(std::numeric_limits<int64_t>::max()) + 2},
          tstorage{uint64_t(2)})));

  EXPECT_EQ(std::get<double>(
                mul(tstorage{(std::numeric_limits<int64_t>::max() / 2) + 2},
                    tstorage{uint64_t(4)})),
            double(__int128_t(std::numeric_limits<uint64_t>::max()) + 3));
}

TEST(arithmetic, mul_int64_t_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{int64_t(1)}, tstorage{double(1)})));

  ASSERT_EQ(std::get<double>(mul(tstorage{int64_t(-1)}, tstorage{double(-1)})),
            1.);
  ASSERT_EQ(std::get<double>(mul(tstorage{int64_t(0)}, tstorage{double(0)})),
            0.);
  ASSERT_EQ(std::get<double>(mul(tstorage{int64_t(1)}, tstorage{double(1)})),
            1.);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isinf(std::get<double>(
      mul(tstorage{int64_t(1)}, tstorage{-limit::infinity()}))));
  EXPECT_TRUE(isinf(std::get<double>(
      mul(tstorage{int64_t(1)}, tstorage{limit::infinity()}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mul(tstorage{int64_t(1)}, tstorage{limit::quiet_NaN()}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mul(tstorage{int64_t(1)}, tstorage{limit::signaling_NaN()}))));
}

TEST(arithmetic, mul_uint64_t_int64_t_result_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      mul(tstorage{uint64_t(1)}, tstorage{int64_t(1)})));

  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{uint64_t(1)}, tstorage{int64_t(1)})), 1);
  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{uint64_t(0)}, tstorage{int64_t(0)})), 0);

  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{uint64_t(2)},
                             tstorage{std::numeric_limits<int64_t>::max()})),
      std::numeric_limits<uint64_t>::max() - 1);
}

TEST(arithmetic, mul_uint64_t_int64_t_result_negative_int64_t) {
  ASSERT_TRUE(std::holds_alternative<int64_t>(
      mul(tstorage{uint64_t(1)}, tstorage{int64_t(-1)})));

  EXPECT_EQ(
      std::get<int64_t>(mul(tstorage{uint64_t(1)}, tstorage{int64_t(-1)})), -1);

  ASSERT_TRUE(std::holds_alternative<int64_t>(
      mul(tstorage{uint64_t(2)},
          tstorage{std::numeric_limits<int64_t>::min() / 2})));

  EXPECT_EQ(
      std::get<int64_t>(mul(tstorage{uint64_t(2)},
                            tstorage{std::numeric_limits<int64_t>::min() / 2})),
      std::numeric_limits<int64_t>::min());
}

TEST(arithmetic, mul_uint64_t_int64_t_result_underflow_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{uint64_t(2)},
          tstorage{(std::numeric_limits<int64_t>::min() / 2) - 1})));

  EXPECT_EQ(std::get<double>(
                mul(tstorage{uint64_t(2)},
                    tstorage{(std::numeric_limits<int64_t>::min() / 2) - 1})),
            double((__int128_t(std::numeric_limits<int64_t>::min()) - 2)));
}

TEST(arithmetic, mul_uint64_t_int64_t_result_overflow_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{uint64_t(2)},
          tstorage{(std::numeric_limits<int64_t>::max()) + 2})));

  EXPECT_EQ(std::get<double>(
                mul(tstorage{uint64_t(4)},
                    tstorage{(std::numeric_limits<int64_t>::max() / 2) + 2})),
            double(__int128_t(std::numeric_limits<uint64_t>::max()) + 3));
}

TEST(arithmetic, mul_uint64_t_uint64_t_result_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      mul(tstorage{uint64_t(1)}, tstorage{uint64_t(1)})));

  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{uint64_t(0)}, tstorage{uint64_t(0)})), 0);
  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{uint64_t(0)}, tstorage{uint64_t(1)})), 0);
  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{uint64_t(1)}, tstorage{uint64_t(0)})), 0);
  EXPECT_EQ(
      std::get<uint64_t>(mul(tstorage{uint64_t(1)}, tstorage{uint64_t(1)})), 1);
  EXPECT_EQ(std::get<uint64_t>(mul(
                tstorage{uint64_t(std::numeric_limits<uint64_t>::max() / 2)},
                tstorage{uint64_t(2)})),
            std::numeric_limits<uint64_t>::max() - 1);
}

TEST(arithmetic, mul_uint64_t_uint64_t_result_overflow_double) {
  ASSERT_TRUE(std::holds_alternative<double>(mul(
      tstorage{std::numeric_limits<uint64_t>::max()}, tstorage{uint64_t(2)})));

  EXPECT_EQ(
      std::get<double>(
          mul(tstorage{uint64_t(std::numeric_limits<uint64_t>::max() / 2) + 2},
              tstorage{uint64_t(2)})),
      double(__uint128_t(std::numeric_limits<uint64_t>::max()) + 1));
}

TEST(arithmetic, mul_uint64_t_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{uint64_t(1)}, tstorage{double(1)})));

  ASSERT_EQ(std::get<double>(mul(tstorage{uint64_t(1)}, tstorage{double(-1)})),
            -1.);
  ASSERT_EQ(std::get<double>(mul(tstorage{uint64_t(0)}, tstorage{double(0)})),
            0.);
  ASSERT_EQ(std::get<double>(mul(tstorage{uint64_t(1)}, tstorage{double(1)})),
            1.);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isinf(std::get<double>(
      mul(tstorage{uint64_t(1)}, tstorage{-limit::infinity()}))));
  EXPECT_TRUE(isinf(std::get<double>(
      mul(tstorage{uint64_t(1)}, tstorage{limit::infinity()}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mul(tstorage{uint64_t(1)}, tstorage{limit::quiet_NaN()}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mul(tstorage{uint64_t(1)}, tstorage{limit::signaling_NaN()}))));
}

TEST(arithmetic, mul_double_int64_t) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{double(1)}, tstorage{int64_t(1)})));

  ASSERT_EQ(std::get<double>(mul(tstorage{double(-1)}, tstorage{int64_t(-1)})),
            1.);
  ASSERT_EQ(std::get<double>(mul(tstorage{double(1)}, tstorage{int64_t(-1)})),
            -1.);
  ASSERT_EQ(std::get<double>(mul(tstorage{double(-1)}, tstorage{int64_t(1)})),
            -1.);
  ASSERT_EQ(std::get<double>(mul(tstorage{double(0)}, tstorage{int64_t(0)})),
            0.);
  ASSERT_EQ(std::get<double>(mul(tstorage{double(1)}, tstorage{int64_t(1)})),
            1.);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isinf(std::get<double>(
      mul(tstorage{-limit::infinity()}, tstorage{int64_t(1)}))));
  EXPECT_TRUE(isinf(std::get<double>(
      mul(tstorage{limit::infinity()}, tstorage{int64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mul(tstorage{limit::quiet_NaN()}, tstorage{int64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mul(tstorage{limit::signaling_NaN()}, tstorage{int64_t(1)}))));
}

TEST(arithmetic, mul_double_uint64_t) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{double(1)}, tstorage{uint64_t(1)})));

  ASSERT_EQ(std::get<double>(mul(tstorage{double(-1)}, tstorage{uint64_t(1)})),
            -1.);
  ASSERT_EQ(std::get<double>(mul(tstorage{double(0)}, tstorage{uint64_t(0)})),
            0.);
  ASSERT_EQ(std::get<double>(mul(tstorage{double(1)}, tstorage{uint64_t(1)})),
            1.);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isinf(std::get<double>(
      mul(tstorage{-limit::infinity()}, tstorage{uint64_t(1)}))));
  EXPECT_TRUE(isinf(std::get<double>(
      mul(tstorage{limit::infinity()}, tstorage{uint64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mul(tstorage{limit::quiet_NaN()}, tstorage{uint64_t(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mul(tstorage{limit::signaling_NaN()}, tstorage{uint64_t(1)}))));
}

TEST(arithmetic, mul_double_double) {
  ASSERT_TRUE(std::holds_alternative<double>(
      mul(tstorage{double(1)}, tstorage{double(1)})));

  ASSERT_EQ(std::get<double>(mul(tstorage{double(-1)}, tstorage{double(-1)})),
            1.);
  ASSERT_EQ(std::get<double>(mul(tstorage{double(1)}, tstorage{double(-1)})),
            -1.);
  ASSERT_EQ(std::get<double>(mul(tstorage{double(-1)}, tstorage{double(1)})),
            -1.);
  ASSERT_EQ(std::get<double>(mul(tstorage{double(0)}, tstorage{double(0)})),
            0.);
  ASSERT_EQ(std::get<double>(mul(tstorage{double(1)}, tstorage{double(1)})),
            1.);

  using limit = std::numeric_limits<double>;
  EXPECT_TRUE(isinf(std::get<double>(
      mul(tstorage{-limit::infinity()}, tstorage{double(1)}))));
  EXPECT_TRUE(isinf(
      std::get<double>(mul(tstorage{limit::infinity()}, tstorage{double(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mul(tstorage{limit::quiet_NaN()}, tstorage{double(1)}))));
  EXPECT_TRUE(isnan(std::get<double>(
      mul(tstorage{limit::signaling_NaN()}, tstorage{double(1)}))));
}

} // namespace math
} // namespace calculator
