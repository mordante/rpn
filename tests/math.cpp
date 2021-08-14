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

import math;

#include <gtest/gtest.h>

TEST(math, add) {
  EXPECT_EQ(math::add(0, 0), 0);
  EXPECT_EQ(math::add(0, 1), 1);
  EXPECT_EQ(math::add(1, 0), 1);
  EXPECT_EQ(math::add(1, 1), 2);
}

TEST(math, sub) {
  EXPECT_EQ(math::sub(0, 0), 0);
  EXPECT_EQ(math::sub(0, 1), -1);
  EXPECT_EQ(math::sub(1, 0), 1);
  EXPECT_EQ(math::sub(1, 1), 0);
}

TEST(math, mul) {
  EXPECT_EQ(math::mul(0, 0), 0);
  EXPECT_EQ(math::mul(2, -2), -4);
  EXPECT_EQ(math::mul(-2, 2), -4);
  EXPECT_EQ(math::mul(-2, -2), 4);
  EXPECT_EQ(math::mul(2, 2), 4);
}

TEST(math, div) {
  EXPECT_EQ(math::div(0, 1), 0);
  EXPECT_EQ(math::div(2, -2), -1);
  EXPECT_EQ(math::div(-2, 2), -1);
  EXPECT_EQ(math::div(-2, -2), 1);
  EXPECT_EQ(math::div(2, 2), 1);
  EXPECT_EQ(math::div(4, 2), 2);
  EXPECT_EQ(math::div(4, 3), 1);
  EXPECT_EQ(math::div(3, 4), 0);
}

TEST(math, bit_and) {
  EXPECT_EQ(math::bit_and(0, 1), 0);
  EXPECT_EQ(math::bit_and(2, 2), 2);
  EXPECT_EQ(math::bit_and(2, 3), 2);
}

TEST(math, bit_or) {
  EXPECT_EQ(math::bit_or(0, 1), 1);
  EXPECT_EQ(math::bit_or(2, 2), 2);
  EXPECT_EQ(math::bit_or(2, 3), 3);
}

TEST(math, bit_xor) {
  EXPECT_EQ(math::bit_xor(0, 1), 1);
  EXPECT_EQ(math::bit_xor(2, 2), 0);
  EXPECT_EQ(math::bit_xor(2, 3), 1);
}

TEST(math, bit_complement) {
  EXPECT_EQ(math::bit_complement(0), ~0);
  EXPECT_EQ(math::bit_complement(2), ~2);
  EXPECT_EQ(math::bit_complement(-2), ~-2);
}
