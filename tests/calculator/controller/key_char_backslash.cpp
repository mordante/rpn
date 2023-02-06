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

TEST(controller, key_char_backslash_too_few_elements) {
  tmodel model;
  tcontroller controller{model};

  controller.handle_keyboard_input(tmodifiers::none, '\\');
  EXPECT_EQ(model.diagnostics_get(),
            format_error("The stack doesn't contain two elements"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_TRUE(model.input_get().empty());

  handle_input(controller, model, "42");
  controller.handle_keyboard_input(tmodifiers::none, '\\');
  EXPECT_EQ(model.diagnostics_get(),
            format_error("The stack doesn't contain two elements"));
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_backslash_stack_input) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "99");
  model.input_append("42");

  controller.handle_keyboard_input(tmodifiers::none, '\\');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"2"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_backslash_stack_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "99");
  handle_input(controller, model, "42");

  controller.handle_keyboard_input(tmodifiers::none, '\\');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"2"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_backslash_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  handle_input(controller, model, "99");
  handle_input(controller, model, "42");

  controller.handle_keyboard_input(tmodifiers::none, '\\');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"2"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_backslash_input_invalid) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "42");
  model.input_append("abc");

  controller.handle_keyboard_input(tmodifiers::none, '\\');
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Invalid numeric value or command"));
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_EQ(model.input_get(), "abc");
}

TEST(controller, key_char_backslash_division_zero) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "42");
  model.input_append("0");

  controller.handle_keyboard_input(tmodifiers::none, '\\');
  EXPECT_EQ(model.diagnostics_get(), format_error("Division by zero"));
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_EQ(model.input_get(), "0");
}

} // namespace calculator
