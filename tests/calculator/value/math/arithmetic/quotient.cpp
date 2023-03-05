/*
 * Copyright (C) Mark de Wever <koraq@xs4all.nl>
 * Part of the RPN project.
 *
 * This program is free software; you can redistribute it and/or
 * quotientify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY.
 *
 * See the COPYING file for more details.
 */

import calculator.math.arithmetic;

#include <cmath>
#include <limits>

#include <gtest/gtest.h>

namespace calculator {
namespace math {

TEST(arithmetic, quotient_int64_t_int64_t) {
  EXPECT_EQ(quotient(tstorage{int64_t(4)}, tstorage{int64_t(3)}),
            tstorage{int64_t(1)});
  EXPECT_EQ(quotient(tstorage{int64_t(4)}, tstorage{int64_t(-3)}),
            tstorage{int64_t(-1)});
  EXPECT_EQ(quotient(tstorage{int64_t(-4)}, tstorage{int64_t(3)}),
            tstorage{int64_t(-1)});
  EXPECT_EQ(quotient(tstorage{int64_t(-4)}, tstorage{int64_t(-3)}),
            tstorage{int64_t(1)});

  EXPECT_THROW(quotient(tstorage{int64_t(3)}, tstorage{int64_t(0)}),
               std::domain_error);
}
TEST(arithmetic, quotient_int64_t_uint64_t) {
  EXPECT_EQ(quotient(tstorage{int64_t(4)}, tstorage{uint64_t(3)}),
            tstorage{uint64_t(1)});

  EXPECT_EQ(quotient(tstorage{int64_t(-4)}, tstorage{uint64_t(3)}),
            tstorage{int64_t(-1)});

  EXPECT_THROW(quotient(tstorage{int64_t(3)}, tstorage{uint64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, quotient_int64_t_double) {
  EXPECT_EQ(quotient(tstorage{int64_t(4)}, tstorage{double(3)}),
            tstorage{uint64_t(1)});
  EXPECT_EQ(quotient(tstorage{int64_t(4)}, tstorage{double(-3)}),
            tstorage{int64_t(-1)});
  EXPECT_EQ(quotient(tstorage{int64_t(-4)}, tstorage{double(3)}),
            tstorage{int64_t(-1)});
  EXPECT_EQ(quotient(tstorage{int64_t(-4)}, tstorage{double(-3)}),
            tstorage{int64_t(1)});

  using limit = std::numeric_limits<double>;
  EXPECT_THROW(quotient(tstorage{int64_t(1)}, tstorage{limit::quiet_NaN()}),
               std::domain_error);
  EXPECT_THROW(quotient(tstorage{int64_t(1)}, tstorage{limit::signaling_NaN()}),
               std::domain_error);

  EXPECT_THROW(quotient(tstorage{int64_t(3)}, tstorage{double(0)}),
               std::domain_error);
}

TEST(arithmetic, quotient_uint64_t_int64_t) {
  EXPECT_EQ(quotient(tstorage{uint64_t(4)}, tstorage{int64_t(3)}),
            tstorage{uint64_t(1)});

  EXPECT_EQ(quotient(tstorage{uint64_t(4)}, tstorage{int64_t(-3)}),
            tstorage{int64_t(-1)});

  EXPECT_THROW(quotient(tstorage{uint64_t(3)}, tstorage{int64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, quotient_uint64_t_uint64_t) {
  EXPECT_EQ(quotient(tstorage{uint64_t(4)}, tstorage{uint64_t(3)}),
            tstorage{uint64_t(1)});

  EXPECT_THROW(quotient(tstorage{uint64_t(3)}, tstorage{uint64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, quotient_double_int64_t) {
  EXPECT_EQ(quotient(tstorage{double(4)}, tstorage{int64_t(3)}),
            tstorage{uint64_t(1)});
  EXPECT_EQ(quotient(tstorage{double(4)}, tstorage{int64_t(-3)}),
            tstorage{int64_t(-1)});
  EXPECT_EQ(quotient(tstorage{double(-4)}, tstorage{int64_t(3)}),
            tstorage{int64_t(-1)});
  EXPECT_EQ(quotient(tstorage{double(-4)}, tstorage{int64_t(-3)}),
            tstorage{int64_t(1)});

  using limit = std::numeric_limits<double>;
  EXPECT_THROW(quotient(tstorage{-limit::infinity()}, tstorage{int64_t(1)}),
               std::range_error);
  EXPECT_THROW(quotient(tstorage{limit::infinity()}, tstorage{int64_t(1)}),
               std::range_error);
  EXPECT_THROW(quotient(tstorage{limit::quiet_NaN()}, tstorage{int64_t(1)}),
               std::domain_error);
  EXPECT_THROW(quotient(tstorage{limit::signaling_NaN()}, tstorage{int64_t(1)}),
               std::domain_error);

  EXPECT_THROW(quotient(tstorage{double(3)}, tstorage{int64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, quotient_double_uint64_t) {
  EXPECT_EQ(quotient(tstorage{double(4)}, tstorage{uint64_t(3)}),
            tstorage{uint64_t(1)});
  EXPECT_EQ(quotient(tstorage{double(-4)}, tstorage{uint64_t(3)}),
            tstorage{int64_t(-1)});

  using limit = std::numeric_limits<double>;
  EXPECT_THROW(quotient(tstorage{-limit::infinity()}, tstorage{uint64_t(1)}),
               std::range_error);
  EXPECT_THROW(quotient(tstorage{limit::infinity()}, tstorage{uint64_t(1)}),
               std::range_error);
  EXPECT_THROW(quotient(tstorage{limit::quiet_NaN()}, tstorage{uint64_t(1)}),
               std::domain_error);
  EXPECT_THROW(
      quotient(tstorage{limit::signaling_NaN()}, tstorage{uint64_t(1)}),
      std::domain_error);

  EXPECT_THROW(quotient(tstorage{double(3)}, tstorage{uint64_t(0)}),
               std::domain_error);
}

TEST(arithmetic, quotient_double_double) {
  EXPECT_EQ(quotient(tstorage{double(4)}, tstorage{double(3)}),
            tstorage{uint64_t(1)});
  EXPECT_EQ(quotient(tstorage{double(4)}, tstorage{double(-3)}),
            tstorage{int64_t(-1)});
  EXPECT_EQ(quotient(tstorage{double(-4)}, tstorage{double(3)}),
            tstorage{int64_t(-1)});
  EXPECT_EQ(quotient(tstorage{double(-4)}, tstorage{double(-3)}),
            tstorage{int64_t(1)});

  using limit = std::numeric_limits<double>;
  EXPECT_THROW(quotient(tstorage{-limit::infinity()}, tstorage{double(1)}),
               std::range_error);
  EXPECT_THROW(quotient(tstorage{limit::infinity()}, tstorage{double(1)}),
               std::range_error);
  EXPECT_THROW(quotient(tstorage{limit::quiet_NaN()}, tstorage{double(1)}),
               std::domain_error);
  EXPECT_THROW(quotient(tstorage{limit::signaling_NaN()}, tstorage{double(1)}),
               std::domain_error);

  EXPECT_THROW(quotient(tstorage{double(3)}, tstorage{double(0)}),
               std::domain_error);
}

} // namespace math
} // namespace calculator
