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

TEST(controller, key_backspace) {
  tmodel model;
  tcontroller controller{model};
  static_assert(noexcept(controller.handle_keyboard_input(tkey::backspace)));
}

TEST(controller, key_backspace_empty_input_empty_stack) {
  tmodel model;
  tcontroller controller{model};

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_EQ(model.diagnostics_get(),
            format_error("The stack doesn't contain an element"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_backspace_non_empty_input_non_empty_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "42");
  model.input_append("abc");

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_EQ(model.input_get(), "ab");

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_EQ(model.input_get(), "a");

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_TRUE(model.input_get().empty());

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack().empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_backspace_diagnostics_not_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("abc");
  model.diagnostics_set("Unchanged");

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_EQ(model.diagnostics_get(), "Unchanged");
  EXPECT_TRUE(model.stack().strings().empty());
  EXPECT_EQ(model.input_get(), "ab");
}

TEST(controller, key_backspace_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "42");
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tkey::backspace);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack().strings().empty());
  EXPECT_TRUE(model.input_get().empty());
}
} // namespace calculator
