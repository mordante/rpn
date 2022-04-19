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

TEST(controller, key_char_control_pow_underflow) {
  tmodel model;
  tcontroller controller{model};

  std::string_view input{"23456789"};
  for (char c : input) {
    controller.handle_keyboard_input(tmodifiers::control, c);
    EXPECT_EQ(model.diagnostics_get(),
              format_error("The stack doesn't contain an element"));
    EXPECT_TRUE(model.stack().empty());
    EXPECT_TRUE(model.input_get().empty());
  }
}

TEST(controller, key_char_control_2_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::none, '2');
  controller.handle_keyboard_input(tmodifiers::control, '2');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"4"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_3_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::none, '2');
  controller.handle_keyboard_input(tmodifiers::control, '3');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"8"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_4_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::none, '2');
  controller.handle_keyboard_input(tmodifiers::control, '4');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"16"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_5_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::none, '2');
  controller.handle_keyboard_input(tmodifiers::control, '5');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"32"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_6_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::none, '2');
  controller.handle_keyboard_input(tmodifiers::control, '6');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"64"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_7_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::none, '2');
  controller.handle_keyboard_input(tmodifiers::control, '7');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"128"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_8_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::none, '2');
  controller.handle_keyboard_input(tmodifiers::control, '8');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"256"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_9_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::none, '2');
  controller.handle_keyboard_input(tmodifiers::control, '9');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"512"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_2_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "2");
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::control, '2');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"4"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_3_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "2");
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::control, '3');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"8"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_4_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "2");
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::control, '4');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"16"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_5_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "2");
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::control, '5');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"32"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_6_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "2");
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::control, '6');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"64"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_7_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "2");
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::control, '7');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"128"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_8_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "2");
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::control, '8');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"256"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_9_stack) {
  tmodel model;
  tcontroller controller{model};
  handle_input(controller, model, "2");
  model.diagnostics_set("Cleared");

  controller.handle_keyboard_input(tmodifiers::control, '9');

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"512"});
  EXPECT_TRUE(model.input_get().empty());
}

} // namespace calculator
