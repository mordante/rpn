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
