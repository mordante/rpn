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

import calculator.model;

import<type_traits>;

#include <gtest/gtest.h>

namespace calculator {

TEST(model, default_constructor) {
  tmodel model;
  EXPECT_TRUE(model.empty());
  EXPECT_EQ(model.size(), 0);
}

TEST(model, copy_constructor) {
  EXPECT_FALSE(std::is_copy_constructible_v<tmodel>)
      << "Implement the proper tests.";
}

TEST(model, move_constructor) {
  EXPECT_FALSE(std::is_move_constructible_v<tmodel>)
      << "Implement the proper tests.";
}

TEST(model, destructor) { EXPECT_TRUE(noexcept(tmodel{}.~tmodel())); }

TEST(model, copy_assignment) {
  EXPECT_FALSE(std::is_copy_assignable_v<tmodel>)
      << "Implement the proper tests.";
}

TEST(model, move_assignment) {
  EXPECT_FALSE(std::is_move_assignable_v<tmodel>)
      << "Implement the proper tests.";
}

TEST(model, empty) {
  const tmodel model;
  EXPECT_TRUE(noexcept(model.empty()));
}

TEST(model, size) {
  const tmodel model;
  EXPECT_TRUE(noexcept(model.size()));
}

TEST(model, push) {
  tmodel model;

  model.push(42);
  EXPECT_FALSE(model.empty());
  EXPECT_EQ(model.size(), 1);

  model.push(42);
  EXPECT_FALSE(model.empty());
  EXPECT_EQ(model.size(), 2);
}

TEST(model, pop) {
  tmodel model;
  tvalue value;

  model.push(42);
  model.push(1);

  value = model.pop();
  EXPECT_EQ(value.get(), 1);
  EXPECT_FALSE(model.empty());
  EXPECT_EQ(model.size(), 1);

  value = model.pop();
  EXPECT_EQ(value.get(), 42);
  EXPECT_TRUE(model.empty());
  EXPECT_EQ(model.size(), 0);

  EXPECT_THROW((void)model.pop(), std::out_of_range);
}

} // namespace calculator
