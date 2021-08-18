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

TEST(controller, key_enter) {
  tmodel model;
  tcontroller controller{model};
  static_assert(noexcept(controller.handle_keyboard_input(tkey::enter)));
}

TEST(controller, key_enter_duplicate_empty_stack) {
  tmodel model;
  tcontroller controller{model};

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Stack is empty"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_duplicate_non_empty_stack) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 2);
  EXPECT_EQ(model.stack_pop(), 42);
  EXPECT_EQ(model.stack_pop(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_value_0) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop(), 0);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_value_min) {
  tmodel model;
  tcontroller controller{model};
  const int64_t minimum = -9223372036854775807ll - 1;

  model.input_append("-9223372036854775808");
  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop(), minimum);
  EXPECT_TRUE(model.input_get().empty());

  model.input_append("-9223372036854775809");
  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Value outside of the representable range"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_value_max) {
  tmodel model;
  tcontroller controller{model};
  const int64_t maximum = 9223372036854775807ll;

  model.input_append("9223372036854775807");
  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop(), maximum);
  EXPECT_TRUE(model.input_get().empty());

  model.input_append("9223372036854775808");
  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Value outside of the representable range"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_invalid_input) {
  tmodel model;
  tcontroller controller{model};

  model.input_append("a");
  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_invalid_input_after_valid_value) {
  tmodel model;
  tcontroller controller{model};

  model.input_append("0a");
  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  model.input_append("42");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_base_2_only_prefix) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0b");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_base_2_invalid_value) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0b2");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_base_8_valid) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("010");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop(), 8);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_base_8_invalid_value) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("08");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_base_16_valid) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0x10");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop(), 16);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_base_16_only_prefix) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0x");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_base_16_invalid_value) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0xg");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}
} // namespace calculator
