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

TEST(controller, key_char_control_z_underflow) {
  tmodel model;
  tcontroller controller{model};

  controller.handle_keyboard_input(tmodifiers::control, 'z');
  EXPECT_EQ(model.diagnostics_get(), format_error("Undo stack underflow"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_z_overflow) {
  tmodel model;
  tcontroller controller{model};

  controller.handle_keyboard_input(tmodifiers::control, 'Z');
  EXPECT_EQ(model.diagnostics_get(), format_error("Undo stack overflow"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_z_basic) {
  tmodel model;
  tcontroller controller{model};
  controller.handle_keyboard_input(tmodifiers::none, '4');
  controller.handle_keyboard_input(tmodifiers::none, '2');
  controller.handle_keyboard_input(tkey::enter);
  model.diagnostics_set("Unchanged");

  controller.handle_keyboard_input(tmodifiers::control, 'z');
  EXPECT_EQ(model.diagnostics_get(), "Unchanged");
  EXPECT_TRUE(model.stack().strings().empty());
  EXPECT_EQ(model.input_get(), "42");

  controller.handle_keyboard_input(tmodifiers::control, 'Z');
  EXPECT_EQ(model.diagnostics_get(), "Unchanged");
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_TRUE(model.input_get().empty());
}

} // namespace calculator
