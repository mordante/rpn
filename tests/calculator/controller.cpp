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

import calculator.controller;

import calculator.model;
import tests.format_error;

#include <gtest/gtest.h>

namespace calculator {

// *** SMF ***

TEST(controller, constructor) {
  tmodel model;
  const tcontroller controller{model};
  static_assert(noexcept(tcontroller{model}));
}

TEST(controller, copy_constructor) {
  static_assert(!std::is_copy_constructible_v<tcontroller>,
                "Implement the proper tests.");
}

TEST(controller, move_constructor) {
  static_assert(!std::is_move_constructible_v<tcontroller>,
                "Implement the proper tests.");
}

TEST(controller, destructor) {
  tmodel model;
  static_assert(noexcept(tcontroller{model}.~tcontroller()));
}

TEST(controller, copy_assignment) {
  static_assert(!std::is_copy_assignable_v<tcontroller>,
                "Implement the proper tests.");
}

TEST(controller, move_assignment) {
  static_assert(!std::is_move_assignable_v<tcontroller>,
                "Implement the proper tests.");
}

// *** append ***

TEST(controller, append) {
  tmodel model;
  tcontroller controller{model};
  static_assert(noexcept(controller.append(std::string_view())));

  controller.append("abc");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.input_get(), "abc");

  controller.append("");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.input_get(), "abc");

  controller.append("def");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.input_get(), "abcdef");
}

TEST(controller, append_diagnostics_unchanged) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Unchanged");

  controller.append("abc");
  EXPECT_EQ(model.diagnostics_get(), "Unchanged");
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.input_get(), "abc");
}
} // namespace calculator
