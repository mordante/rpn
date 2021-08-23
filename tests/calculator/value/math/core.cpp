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

import calculator.math.core;

import<cmath>;
import<limits>;

#include <gtest/gtest.h>

namespace calculator {
namespace math {

TEST(core, bitwise_cast) {
  const tstorage value{int64_t(1)};
  static_assert(std::same_as<decltype(bitwise_cast(value)), uint64_t>);
}

TEST(core, bitwise_cast_int64_t) {
  EXPECT_EQ(bitwise_cast(tstorage{INT64_MIN}),
            static_cast<uint64_t>(INT64_MIN));
  EXPECT_EQ(bitwise_cast(tstorage{int64_t(-1)}), static_cast<uint64_t>(-1));
  EXPECT_EQ(bitwise_cast(tstorage{int64_t(0)}), 0);
  EXPECT_EQ(bitwise_cast(tstorage{int64_t(1)}), 1);
  EXPECT_EQ(bitwise_cast(tstorage{INT64_MAX}), INT64_MAX);
}

TEST(core, bitwise_cast_uint64_t) {
  EXPECT_EQ(bitwise_cast(tstorage{uint64_t(0)}), 0);
  EXPECT_EQ(bitwise_cast(tstorage{uint64_t(1)}), 1);
  EXPECT_EQ(bitwise_cast(tstorage{UINT64_MAX}), UINT64_MAX);
}

TEST(core, bitwise_cast_double) {
  EXPECT_EQ(bitwise_cast(tstorage{DBL_MIN}), bit_cast<uint64_t>(DBL_MIN));
  EXPECT_EQ(bitwise_cast(tstorage{double(-1)}), bit_cast<uint64_t>(double(-1)));
  EXPECT_EQ(bitwise_cast(tstorage{double(0)}), bit_cast<uint64_t>(double(0)));
  EXPECT_EQ(bitwise_cast(tstorage{double(1)}), bit_cast<uint64_t>(double(1)));
  EXPECT_EQ(bitwise_cast(tstorage{DBL_MAX}), bit_cast<uint64_t>(DBL_MAX));

  using limit = std::numeric_limits<double>;
  EXPECT_EQ(bitwise_cast(tstorage{-limit::infinity()}),
            bit_cast<uint64_t>(-limit::infinity()));
  EXPECT_EQ(bitwise_cast(tstorage{limit::infinity()}),
            bit_cast<uint64_t>(limit::infinity()));
  EXPECT_EQ(bitwise_cast(tstorage{limit::quiet_NaN()}),
            bit_cast<uint64_t>(limit::quiet_NaN()));
  EXPECT_EQ(bitwise_cast(tstorage{limit::signaling_NaN()}),
            bit_cast<uint64_t>(limit::signaling_NaN()));
}

TEST(core, positive_integral_cast_int64_t) {
  static_assert(
      std::same_as<decltype(positive_integral_cast(tstorage{int64_t(1)})),
                   uint64_t>);
  EXPECT_THROW(positive_integral_cast(tstorage{INT64_MIN}), std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{-1}), std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{0}), std::range_error);
  EXPECT_EQ(positive_integral_cast(tstorage{int64_t(1)}), 1);
  EXPECT_EQ(positive_integral_cast(tstorage{INT64_MAX}), INT64_MAX);
}

TEST(core, positive_integral_cast_uint64_t) {
  static_assert(
      std::same_as<decltype(positive_integral_cast(tstorage{uint64_t(1)})),
                   uint64_t>);
  EXPECT_THROW(positive_integral_cast(tstorage{uint64_t(0)}), std::range_error);
  EXPECT_EQ(positive_integral_cast(tstorage{uint64_t(1)}), 1);
  EXPECT_EQ(positive_integral_cast(tstorage{UINT64_MAX}), UINT64_MAX);
}

TEST(core, positive_integral_cast_double) {
  static_assert(
      std::same_as<decltype(positive_integral_cast(tstorage{double(1)})),
                   uint64_t>);

  EXPECT_THROW(positive_integral_cast(tstorage{DBL_MIN}), std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{double(-1.)}), std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{double(-0.)}), std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{double(-0.)}), std::range_error);
  EXPECT_EQ(positive_integral_cast(tstorage{double(1.)}), 1);
  EXPECT_THROW(positive_integral_cast(tstorage{std::nextafter(
                   std::numeric_limits<uint64_t>::max(), 1e300)}),
               std::range_error);

  EXPECT_THROW(positive_integral_cast(tstorage{std::nextafter(2., 1.)}),
               std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{std::nextafter(2., 3.)}),
               std::range_error);

  using limit = std::numeric_limits<double>;
  EXPECT_THROW(positive_integral_cast(tstorage{-limit::infinity()}),
               std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{limit::infinity()}),
               std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{limit::quiet_NaN()}),
               std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{limit::signaling_NaN()}),
               std::range_error);
}

} // namespace math
} // namespace calculator
