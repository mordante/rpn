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

TEST(controller, key_char_tilde_too_few_elements) {
  tmodel model;
  tcontroller controller{model};

  controller.handle_keyboard_input(tmodifiers::none, '~');
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain an element"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_tilde_input) {
  tmodel model;
  model.base_set(tbase::binary);
  tcontroller controller{model};
  model.input_append("3");

  controller.handle_keyboard_input(tmodifiers::none, '~');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            std::vector<std::string>{"0b111111111111111111111111111111111111111"
                                     "1111111111111111111111100"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_tilde_stack) {
  tmodel model;
  model.base_set(tbase::binary);
  tcontroller controller{model};
  handle_input(controller, model, "3");

  controller.handle_keyboard_input(tmodifiers::none, '~');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            std::vector<std::string>{"0b111111111111111111111111111111111111111"
                                     "1111111111111111111111100"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_tilde_diagnostics_cleared) {
  tmodel model;
  model.base_set(tbase::binary);
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  handle_input(controller, model, "3");

  controller.handle_keyboard_input(tmodifiers::none, '~');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            std::vector<std::string>{"0b111111111111111111111111111111111111111"
                                     "1111111111111111111111100"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_tilde_input_invalid) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("abc");

  controller.handle_keyboard_input(tmodifiers::none, '~');
  EXPECT_EQ(model.diagnostics_get(), "Invalid numeric value");
  EXPECT_TRUE(model.stack().empty());
  EXPECT_TRUE(model.input_get().empty());
}
} // namespace calculator
