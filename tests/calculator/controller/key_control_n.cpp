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

TEST(controller, key_char_control_n_too_few_elements) {
  tmodel model;
  tcontroller controller{model};

  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain an element"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_n_input) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "3");

  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"-3"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_n_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "3");

  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"-3"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_n_round_trip) {
  tmodel model;
  tcontroller controller{model};
  // -std::numerical_limits<int64_t>::min() is the largest integral round-trip.
  handle_input(controller, model, "9223372036854775808");

  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            std::vector<std::string>{"-9223372036854775808"});
  EXPECT_TRUE(model.input_get().empty());

  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            std::vector<std::string>{"9223372036854775808"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_n_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  handle_input(controller, model, "42");

  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"-42"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_n_input_invalid) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("abc");

  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_EQ(model.diagnostics_get(), "Invalid numeric value");
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "abc");
}
} // namespace calculator
