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
TEST(controller, key_char_minus_too_few_elements) {
  tmodel model;
  tcontroller controller{model};

  controller.handle_keyboard_input(tmodifiers::none, '-');
  EXPECT_EQ(model.diagnostics_get(),
            format_error("The stack doesn't contain two elements"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_TRUE(model.input_get().empty());

  handle_input(controller, model, "42");
  controller.handle_keyboard_input(tmodifiers::none, '-');
  EXPECT_EQ(model.diagnostics_get(),
            format_error("The stack doesn't contain two elements"));
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_minus_stack_input) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "42");
  model.input_append("21");

  controller.handle_keyboard_input(tmodifiers::none, '-');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"21"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_minus_stack_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "42");
  handle_input(controller, model, "21");

  controller.handle_keyboard_input(tmodifiers::none, '-');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"21"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_minus_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  handle_input(controller, model, "42");
  handle_input(controller, model, "21");

  controller.handle_keyboard_input(tmodifiers::none, '-');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"21"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_minus_input_invalid) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "42");
  model.input_append("abc");

  controller.handle_keyboard_input(tmodifiers::none, '-');
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Invalid numeric value or command"));
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_EQ(model.input_get(), "abc");
}

TEST(controller, key_char_minus_float_exponent) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Unchanged");
  model.input_append("1e");

  controller.handle_keyboard_input(tmodifiers::none, '-');
  EXPECT_EQ(model.diagnostics_get(), "Unchanged");
  EXPECT_TRUE(model.stack().strings().empty());
  EXPECT_EQ(model.input_get(), "1e-");
}

TEST(controller, key_char_minus_signed_integral) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Unchanged");
  model.input_append("i");

  controller.handle_keyboard_input(tmodifiers::none, '-');
  EXPECT_EQ(model.diagnostics_get(), "Unchanged");
  EXPECT_TRUE(model.stack().strings().empty());
  EXPECT_EQ(model.input_get(), "i-");
}
} // namespace calculator
