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

TEST(controller, key_char_pipe_too_few_elements) {
  tmodel model;
  tcontroller controller{model};

  controller.handle_keyboard_input('|');
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain two elements"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());

  model.stack_push(42);
  controller.handle_keyboard_input('|');
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain two elements"));
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_pipe_stack_input) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(0b1101);
  model.input_append("2");

  controller.handle_keyboard_input('|');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 0b1111);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_pipe_stack_stack) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(0b1101);
  model.stack_push(2);

  controller.handle_keyboard_input('|');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 0b1111);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_pipe_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  model.stack_push(42);
  model.stack_push(42);

  controller.handle_keyboard_input('|');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_pipe_input_invalid) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);
  model.input_append("abc");

  controller.handle_keyboard_input('|');
  EXPECT_EQ(model.diagnostics_get(), "Invalid numeric value");
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}
} // namespace calculator
