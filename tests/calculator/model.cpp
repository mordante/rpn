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
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
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

TEST(model, stack_empty) {
  const tmodel model;
  EXPECT_TRUE(noexcept(model.stack_empty()));
}

TEST(model, stack_size) {
  const tmodel model;
  EXPECT_TRUE(noexcept(model.stack_size()));
}

TEST(model, stack_push) {
  tmodel model;

  model.stack_push(42);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_FALSE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 1);

  model.stack_push(42);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_FALSE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 2);
}

TEST(model, stack_pop) {
  tmodel model;
  tvalue value;

  model.stack_push(42);
  model.stack_push(1);

  value = model.stack_pop();
  EXPECT_EQ(value.get(), 1);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_FALSE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 1);

  value = model.stack_pop();
  EXPECT_EQ(value.get(), 42);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);

  EXPECT_THROW((void)model.stack_pop(), std::out_of_range);
}

TEST(model, diagnostics_set) {
  tmodel model;

  model.diagnostics_set("abc");
  EXPECT_EQ(model.diagnostics_get(), "abc");
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);

  model.diagnostics_set("");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
}

TEST(model, diagnostics_clear) {
  tmodel model;

  model.diagnostics_set("abc");

  model.diagnostics_clear();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
}

} // namespace calculator
