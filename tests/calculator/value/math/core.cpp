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

import <bit>;
import <cmath>;
import <limits>;

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
  EXPECT_EQ(bitwise_cast(tstorage{DBL_MIN}), std::bit_cast<uint64_t>(DBL_MIN));
  EXPECT_EQ(bitwise_cast(tstorage{double(-1)}),
            std::bit_cast<uint64_t>(double(-1)));
  EXPECT_EQ(bitwise_cast(tstorage{double(0)}),
            std::bit_cast<uint64_t>(double(0)));
  EXPECT_EQ(bitwise_cast(tstorage{double(1)}),
            std::bit_cast<uint64_t>(double(1)));
  EXPECT_EQ(bitwise_cast(tstorage{DBL_MAX}), std::bit_cast<uint64_t>(DBL_MAX));

  using limit = std::numeric_limits<double>;
  EXPECT_EQ(bitwise_cast(tstorage{-limit::infinity()}),
            std::bit_cast<uint64_t>(-limit::infinity()));
  EXPECT_EQ(bitwise_cast(tstorage{limit::infinity()}),
            std::bit_cast<uint64_t>(limit::infinity()));
  EXPECT_EQ(bitwise_cast(tstorage{limit::quiet_NaN()}),
            std::bit_cast<uint64_t>(limit::quiet_NaN()));
  EXPECT_EQ(bitwise_cast(tstorage{limit::signaling_NaN()}),
            std::bit_cast<uint64_t>(limit::signaling_NaN()));
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

  EXPECT_THROW(positive_integral_cast(tstorage{-DBL_MAX}), std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{double(-1.)}), std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{-DBL_MIN}), std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{double(-0.)}), std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{double(0.)}), std::range_error);
  EXPECT_THROW(positive_integral_cast(tstorage{DBL_MIN}), std::range_error);
  EXPECT_EQ(positive_integral_cast(tstorage{double(1.)}), 1);
  EXPECT_THROW(positive_integral_cast(tstorage{DBL_MAX}), std::range_error);

  EXPECT_THROW(positive_integral_cast(tstorage{std::nextafter(
                   std::numeric_limits<uint64_t>::max(), DBL_MAX)}),
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

TEST(core, negative_integral_cast_int64_t) {
  static_assert(
      std::same_as<decltype(negative_integral_cast(tstorage{int64_t(-1)})),
                   int64_t>);
  EXPECT_EQ(negative_integral_cast(tstorage{INT64_MIN}), INT64_MIN);
  EXPECT_EQ(negative_integral_cast(tstorage{-1}), -1);
  EXPECT_THROW(negative_integral_cast(tstorage{0}), std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{int64_t(1)}), std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{INT64_MAX}), std::range_error);
}

TEST(core, negative_integral_cast_uint64_t) {
  static_assert(
      std::same_as<decltype(negative_integral_cast(tstorage{uint64_t(1)})),
                   int64_t>);
  EXPECT_THROW(negative_integral_cast(tstorage{uint64_t(0)}), std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{uint64_t(1)}), std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{UINT64_MAX}), std::range_error);
}

TEST(core, negative_integral_cast_double) {
  static_assert(
      std::same_as<decltype(negative_integral_cast(tstorage{double(1)})),
                   int64_t>);

  EXPECT_THROW(negative_integral_cast(tstorage{-DBL_MAX}), std::range_error);
  EXPECT_EQ(negative_integral_cast(tstorage{double(-1.)}), -1);
  EXPECT_THROW(negative_integral_cast(tstorage{-DBL_MIN}), std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{double(-0.)}), std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{double(0.)}), std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{DBL_MIN}), std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{double(1.)}), std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{DBL_MAX}), std::range_error);

  EXPECT_THROW(negative_integral_cast(tstorage{std::nextafter(
                   std::numeric_limits<int64_t>::min(), -DBL_MAX)}),
               std::range_error);

  EXPECT_THROW(negative_integral_cast(tstorage{std::nextafter(-2., -1.)}),
               std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{std::nextafter(-2., -3.)}),
               std::range_error);

  using limit = std::numeric_limits<double>;
  EXPECT_THROW(negative_integral_cast(tstorage{-limit::infinity()}),
               std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{limit::infinity()}),
               std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{limit::quiet_NaN()}),
               std::range_error);
  EXPECT_THROW(negative_integral_cast(tstorage{limit::signaling_NaN()}),
               std::range_error);
}

TEST(core, integral_cast_int64_t) {
  static_assert(
      std::same_as<decltype(integral_cast(tstorage{int64_t(-1)})), tstorage>);
  EXPECT_EQ(integral_cast(tstorage{INT64_MIN}), tstorage{int64_t(INT64_MIN)});
  EXPECT_EQ(integral_cast(tstorage{-1}), tstorage{int64_t(-1)});
  EXPECT_EQ(integral_cast(tstorage{0}), tstorage{int64_t(0)});
  EXPECT_EQ(integral_cast(tstorage{int64_t(1)}), tstorage{int64_t(1)});
  EXPECT_EQ(integral_cast(tstorage{INT64_MAX}), tstorage{int64_t(INT64_MAX)});
}

TEST(core, integral_cast_uint64_t) {
  static_assert(
      std::same_as<decltype(integral_cast(tstorage{uint64_t(1)})), tstorage>);
  EXPECT_EQ(integral_cast(tstorage{uint64_t(0)}), tstorage{uint64_t(0)});
  EXPECT_EQ(integral_cast(tstorage{uint64_t(1)}), tstorage{uint64_t(1)});
  EXPECT_EQ(integral_cast(tstorage{UINT64_MAX}),
            tstorage{uint64_t(UINT64_MAX)});
}

TEST(core, integral_cast_double) {
  static_assert(
      std::same_as<decltype(integral_cast(tstorage{double(1)})), tstorage>);

  EXPECT_THROW(integral_cast(tstorage{-DBL_MAX}), std::range_error);
  EXPECT_EQ(integral_cast(tstorage{double(INT64_MIN)}),
            tstorage{int64_t(INT64_MIN)});
  EXPECT_EQ(integral_cast(tstorage{double(-1.)}), tstorage{int64_t(-1)});
  EXPECT_THROW(integral_cast(tstorage{-DBL_MIN}), std::range_error);
  EXPECT_EQ(integral_cast(tstorage{double(-0.)}), tstorage{uint64_t(0)});
  EXPECT_EQ(integral_cast(tstorage{double(0.)}), tstorage{uint64_t(0)});
  EXPECT_THROW(integral_cast(tstorage{DBL_MIN}), std::range_error);
  EXPECT_EQ(integral_cast(tstorage{double(1.)}), tstorage{uint64_t(1)});
  // Note the 64-bit maxima don't round-trip through a double due to the lack
  // of precision in a double.
  EXPECT_EQ(integral_cast(tstorage{double(INT32_MAX)}),
            tstorage{uint64_t(INT32_MAX)});
  EXPECT_EQ(integral_cast(tstorage{double(UINT32_MAX)}),
            tstorage{uint64_t(UINT32_MAX)});
  EXPECT_THROW(integral_cast(tstorage{DBL_MAX}), std::range_error);

  EXPECT_THROW(integral_cast(tstorage{std::nextafter(
                   std::numeric_limits<int64_t>::min(), -DBL_MAX)}),
               std::range_error);

  EXPECT_THROW(integral_cast(tstorage{std::nextafter(-2., -1.)}),
               std::range_error);
  EXPECT_THROW(integral_cast(tstorage{std::nextafter(-2., -3.)}),
               std::range_error);

  using limit = std::numeric_limits<double>;
  EXPECT_THROW(integral_cast(tstorage{-limit::infinity()}), std::range_error);
  EXPECT_THROW(integral_cast(tstorage{limit::infinity()}), std::range_error);
  EXPECT_THROW(integral_cast(tstorage{limit::quiet_NaN()}), std::domain_error);
  EXPECT_THROW(integral_cast(tstorage{limit::signaling_NaN()}),
               std::domain_error);
}

template <class I> static void to_storage_double() {
  ASSERT_TRUE(std::holds_alternative<double>(
      to_storage<I>(__int128_t(std::numeric_limits<int64_t>::min()) - 1)));
  EXPECT_EQ(
      std::get<double>(
          to_storage<I>(__int128_t(std::numeric_limits<int64_t>::min()) - 1)),
      static_cast<double>(__int128_t(std::numeric_limits<int64_t>::min()) - 1));

  ASSERT_TRUE(std::holds_alternative<double>(
      to_storage<I>(__int128_t(std::numeric_limits<uint64_t>::max()) + 1)));
  EXPECT_EQ(std::get<double>(to_storage<I>(
                __int128_t(std::numeric_limits<uint64_t>::max()) + 1)),
            static_cast<double>(
                __int128_t(std::numeric_limits<uint64_t>::max()) + 1));
}

TEST(core, to_storage_int128_prefer_int64_t) {
  /*** int64_t ***/

  ASSERT_TRUE(std::holds_alternative<int64_t>(
      to_storage<int64_t>(__int128_t(std::numeric_limits<int64_t>::min()))));
  EXPECT_EQ(std::get<int64_t>(to_storage<int64_t>(
                __int128_t(std::numeric_limits<int64_t>::min()))),
            std::numeric_limits<int64_t>::min());

  ASSERT_TRUE(std::holds_alternative<int64_t>(
      to_storage<int64_t>(__int128_t(std::numeric_limits<int64_t>::max()))));
  EXPECT_EQ(std::get<int64_t>(to_storage<int64_t>(
                __int128_t(std::numeric_limits<int64_t>::max()))),
            std::numeric_limits<int64_t>::max());

  /*** uint64_t ***/

  ASSERT_TRUE(std::holds_alternative<uint64_t>(to_storage<int64_t>(
      __int128_t(std::numeric_limits<int64_t>::max()) + 1)));
  EXPECT_EQ(std::get<uint64_t>(to_storage<int64_t>(
                __int128_t(std::numeric_limits<int64_t>::max()) + 1)),
            uint64_t(std::numeric_limits<int64_t>::max()) + 1);

  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      to_storage<int64_t>(__int128_t(std::numeric_limits<uint64_t>::max()))));
  EXPECT_EQ(std::get<uint64_t>(to_storage<int64_t>(
                __int128_t(std::numeric_limits<uint64_t>::max()))),
            std::numeric_limits<uint64_t>::max());

  /*** double ***/
  to_storage_double<int64_t>();
}

TEST(core, to_storage_int128_prefer_uint64_t) {
  /*** int64_t ***/

  ASSERT_TRUE(std::holds_alternative<int64_t>(
      to_storage<uint64_t>(__int128_t(std::numeric_limits<int64_t>::min()))));
  EXPECT_EQ(std::get<int64_t>(to_storage<uint64_t>(
                __int128_t(std::numeric_limits<int64_t>::min()))),
            std::numeric_limits<int64_t>::min());

  ASSERT_TRUE(
      std::holds_alternative<int64_t>(to_storage<uint64_t>(__int128_t(-1))));
  EXPECT_EQ(std::get<int64_t>(to_storage<uint64_t>(__int128_t(-1))), -1);

  /*** uint64_t ***/

  ASSERT_TRUE(
      std::holds_alternative<uint64_t>(to_storage<uint64_t>(__int128_t(0))));
  EXPECT_EQ(std::get<uint64_t>(to_storage<uint64_t>(__int128_t(0))), 0);

  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      to_storage<uint64_t>(__int128_t(std::numeric_limits<uint64_t>::max()))));
  EXPECT_EQ(std::get<uint64_t>(to_storage<uint64_t>(
                __int128_t(std::numeric_limits<uint64_t>::max()))),
            std::numeric_limits<uint64_t>::max());

  /*** double ***/
  to_storage_double<uint64_t>();
}

TEST(core, to_storage_int128_defaulted_preference) {
  /*** int64_t ***/

  ASSERT_TRUE(std::holds_alternative<int64_t>(
      to_storage(__int128_t(std::numeric_limits<int64_t>::min()))));
  EXPECT_EQ(std::get<int64_t>(
                to_storage(__int128_t(std::numeric_limits<int64_t>::min()))),
            std::numeric_limits<int64_t>::min());

  ASSERT_TRUE(std::holds_alternative<int64_t>(to_storage(__int128_t(-1))));
  EXPECT_EQ(std::get<int64_t>(to_storage(__int128_t(-1))), -1);

  /*** uint64_t ***/

  ASSERT_TRUE(std::holds_alternative<uint64_t>(to_storage(__int128_t(0))));
  EXPECT_EQ(std::get<uint64_t>(to_storage(__int128_t(0))), 0);

  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      to_storage(__int128_t(std::numeric_limits<uint64_t>::max()))));
  EXPECT_EQ(std::get<uint64_t>(
                to_storage(__int128_t(std::numeric_limits<uint64_t>::max()))),
            std::numeric_limits<uint64_t>::max());

  /*** double ***/
  ASSERT_TRUE(std::holds_alternative<double>(
      to_storage(__int128_t(std::numeric_limits<int64_t>::min()) - 1)));
  EXPECT_EQ(
      std::get<double>(
          to_storage(__int128_t(std::numeric_limits<int64_t>::min()) - 1)),
      static_cast<double>(__int128_t(std::numeric_limits<int64_t>::min()) - 1));

  ASSERT_TRUE(std::holds_alternative<double>(
      to_storage(__int128_t(std::numeric_limits<uint64_t>::max()) + 1)));
  EXPECT_EQ(std::get<double>(to_storage(
                __int128_t(std::numeric_limits<uint64_t>::max()) + 1)),
            static_cast<double>(
                __int128_t(std::numeric_limits<uint64_t>::max()) + 1));
}

TEST(core, to_storage_uint128) {
  ASSERT_TRUE(std::holds_alternative<uint64_t>(to_storage(__uint128_t(0))));
  ASSERT_EQ(std::get<uint64_t>(to_storage(__uint128_t(0))), 0);

  ASSERT_TRUE(std::holds_alternative<uint64_t>(
      to_storage(__uint128_t(std::numeric_limits<uint64_t>::max()))));
  ASSERT_EQ(std::get<uint64_t>(
                to_storage(__uint128_t(std::numeric_limits<uint64_t>::max()))),
            std::numeric_limits<uint64_t>::max());

  ASSERT_TRUE(std::holds_alternative<double>(
      to_storage(__uint128_t(std::numeric_limits<uint64_t>::max()) + 1)));
  ASSERT_EQ(std::get<double>(to_storage(
                __uint128_t(std::numeric_limits<uint64_t>::max()) + 1)),
            double(__uint128_t(std::numeric_limits<uint64_t>::max()) + 1));
}

} // namespace math
} // namespace calculator
