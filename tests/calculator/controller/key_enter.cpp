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
import tests.handle_input;

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
  EXPECT_EQ(model.diagnostics_get(),
            format_error("The stack doesn't contain an element"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_duplicate_non_empty_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "42");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"42"}, {"42"}}));
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_value_0) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"0"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_value_max) {
  tmodel model;
  tcontroller controller{model};

  model.input_append("18446744073709551615");
  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            std::vector<std::string>{"18446744073709551615"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_value_overflow) {
  tmodel model;
  tcontroller controller{model};

  model.input_append("18446744073709551616");
  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Value outside of the representable range"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "18446744073709551616");
}

TEST(controller, key_enter_invalid_input) {
  tmodel model;
  tcontroller controller{model};

  model.input_append("a");
  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Invalid numeric value or command"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "a");
}

TEST(controller, key_enter_invalid_input_after_valid_value) {
  tmodel model;
  tcontroller controller{model};

  model.input_append("0a");
  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "0a");
}

TEST(controller, key_enter_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  model.input_append("42");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_base_2_only_prefix) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0b");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "0b");
}

TEST(controller, key_enter_base_2_invalid_value) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0b2");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "0b2");
}

TEST(controller, key_enter_base_8_valid) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("010");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"8"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_base_8_invalid_value) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("08");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "08");
}

TEST(controller, key_enter_base_16_valid) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0x10");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"16"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_base_16_only_prefix) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0x");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "0x");
}

TEST(controller, key_enter_base_16_invalid_value) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0xg");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "0xg");
}

TEST(controller, key_enter_float_valid) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("123.e-12");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"1.23e-10"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_enter_float_too_large) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("123.e-1234");

  controller.handle_keyboard_input(tkey::enter);
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "123.e-1234");
}
} // namespace calculator
