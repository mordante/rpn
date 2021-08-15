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

TEST(controller, key_backspace) {
  tmodel model;
  tcontroller controller{model};
  static_assert(noexcept(controller.handle_keyboard_input(tkey::backspace)));
}

TEST(controller, key_backspace_empty_input_empty_stack) {
  tmodel model;
  tcontroller controller{model};

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_EQ(model.diagnostics_get(), format_error("Stack is empty"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_backspace_non_empty_input_non_empty_stack) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);
  model.input_append("abc");

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.input_get(), "ab");

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.input_get(), "a");

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_TRUE(model.input_get().empty());

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}
} // namespace calculator
