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

import calculator.value;

#include <gtest/gtest.h>

namespace calculator {
TEST(value, default_constructor) {
  static_assert(noexcept(tvalue{}));
  constexpr tvalue value;
  EXPECT_EQ(value, 0);
}

TEST(value, converting_constructor) {
  static_assert(noexcept(tvalue{1}));
  constexpr tvalue value = 1;
  EXPECT_EQ(value, 1);
}

TEST(value, copy_constructor) {
  constexpr tvalue v = 1;
  static_assert(noexcept(tvalue{v}));

  constexpr tvalue value{v};
  EXPECT_EQ(v, 1);
  EXPECT_EQ(value, 1);
}

TEST(value, move_constructor) {
  constexpr tvalue v = 1;
  static_assert(noexcept(tvalue{std::move(v)}));

  constexpr tvalue value{std::move(v)};
  EXPECT_EQ(v, 1);
}

TEST(value, destructor) { static_assert(noexcept(tvalue{}.~tvalue())); }

TEST(value, copy_assignment) {
  constexpr tvalue v = 1;
  tvalue value;
  static_assert(noexcept(value.operator=(v)));

  value = v;
  EXPECT_EQ(v, 1);

  EXPECT_EQ(value, 1);

  // Uses implicit constructor.
  value = 42;
  EXPECT_EQ(value, 42);
}

TEST(value, move_assignment) {
  constexpr tvalue v = 1;
  tvalue value;
  static_assert(noexcept(value.operator=(std::move(v))));

  value = std::move(v);
  EXPECT_EQ(value, 1);
}

TEST(value, get) {
  constexpr tvalue value;
  static_assert(noexcept(value.get()));
}

} // namespace calculator
