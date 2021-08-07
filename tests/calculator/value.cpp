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
  EXPECT_TRUE(noexcept(tvalue{}));
  constexpr tvalue value;
  EXPECT_EQ(value.get(), 0);
  EXPECT_EQ(value.format(), "0");
}

TEST(value, converting_constructor) {
  EXPECT_TRUE(noexcept(tvalue{1}));
  constexpr tvalue value = 1;
  EXPECT_EQ(value.get(), 1);
  EXPECT_EQ(value.format(), "1");
}

TEST(value, copy_constructor) {
  constexpr tvalue v = 1;
  EXPECT_TRUE(noexcept(tvalue{v}));

  constexpr tvalue value{v};
  EXPECT_EQ(v.get(), 1);
  EXPECT_EQ(value.format(), "1");
  EXPECT_EQ(value.get(), 1);
  EXPECT_EQ(value.format(), "1");
}

TEST(value, move_constructor) {
  constexpr tvalue v = 1;
  EXPECT_TRUE(noexcept(tvalue{std::move(v)}));

  constexpr tvalue value{std::move(v)};
  EXPECT_EQ(v.get(), 1);
  EXPECT_EQ(value.format(), "1");
}

TEST(value, destructor) { EXPECT_TRUE(noexcept(tvalue{}.~tvalue())); }

TEST(value, copy_assignment) {
  constexpr tvalue v = 1;
  tvalue value;
  EXPECT_TRUE(noexcept(value.operator=(v)));

  value = v;
  EXPECT_EQ(v.get(), 1);
  EXPECT_EQ(v.format(), "1");

  EXPECT_EQ(value.get(), 1);
  EXPECT_EQ(value.format(), "1");

  // Uses implicit constructor.
  value = 42;
  EXPECT_EQ(value.get(), 42);
  EXPECT_EQ(value.format(), "42");
}

TEST(value, move_assignment) {
  constexpr tvalue v = 1;
  tvalue value;
  EXPECT_TRUE(noexcept(value.operator=(std::move(v))));

  value = std::move(v);
  EXPECT_EQ(value.get(), 1);
  EXPECT_EQ(value.format(), "1");
}

TEST(value, set) {
  tvalue value;
  EXPECT_TRUE(noexcept(value.set(42)));
  value.set(42);
  EXPECT_EQ(value.get(), 42);
  EXPECT_EQ(value.format(), "42");

  value.set(1);
  EXPECT_EQ(value.get(), 1);
  EXPECT_EQ(value.format(), "1");
}

TEST(value, get) {
  constexpr tvalue value;
  EXPECT_TRUE(noexcept(value.get()));
}

} // namespace calculator
