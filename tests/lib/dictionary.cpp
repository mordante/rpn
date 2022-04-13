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

import lib.dictionary;

import <array>;

#include <gtest/gtest.h>

namespace lib {
TEST(dictionary, find) {
  static constexpr std::array lut = make_dictionary("Z", 'z', "A", 'a');
  {
    auto it = find(lut, "A");
    EXPECT_NE(it, lut.end());
    EXPECT_EQ(it->first, "A");
    EXPECT_EQ(it->second, 'a');
  }
  {
    auto it = find(lut, "Z");
    EXPECT_NE(it, lut.end());
    EXPECT_EQ(it->first, "Z");
    EXPECT_EQ(it->second, 'z');
  }
  {
    auto it = find(lut, "ZA");
    EXPECT_EQ(it, lut.end());
  }
}
} // namespace lib
