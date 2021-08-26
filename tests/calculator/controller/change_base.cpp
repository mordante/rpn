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
import tests.handle_input;

#include <gtest/gtest.h>

namespace calculator {

TEST(controller, key_char_control_base) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "42");
  EXPECT_EQ(model.strings(), (std::vector<std::string>{{"42"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'b');
  EXPECT_EQ(model.strings(), (std::vector<std::string>{{"0b101010"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'o');
  EXPECT_EQ(model.strings(), (std::vector<std::string>{{"052"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'd');
  EXPECT_EQ(model.strings(), (std::vector<std::string>{{"42"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'h');
  EXPECT_EQ(model.strings(), (std::vector<std::string>{{"0x2a"}}));
}
} // namespace calculator
