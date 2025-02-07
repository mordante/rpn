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
import lib.base;
import tests.handle_input;

import <locale>;

#include <gtest/gtest.h>

namespace calculator {

TEST(controller, key_char_control_g_basic) {
  tmodel model;
  model.base_set(lib::tbase::binary);
  tcontroller controller{model};

  model.diagnostics_set("Unchanged");

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_EQ(model.diagnostics_get(), "Unchanged");
  EXPECT_TRUE(model.stack().strings().empty());
  EXPECT_TRUE(model.input_get().empty());

  handle_input(controller, model, "0b1111111");

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0b111'1111"}}));
  EXPECT_TRUE(model.input_get().empty());

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{{"0b1111111"}}));
  EXPECT_TRUE(model.input_get().empty());

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0b111'1111"}}));
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_g_base_2) {
  tmodel model;
  model.base_set(lib::tbase::binary);
  tcontroller controller{model};

  handle_input(controller, model, "0b111");
  handle_input(controller, model, "0b1111111");
  handle_input(controller, model, "0b1111111");
  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{{"0b111"}, {"0b111'1111"}, {"-0b111'1111"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{{"0b111"}, {"0b1111111"}, {"-0b1111111"}}));
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_g_base_8) {
  tmodel model;
  model.base_set(lib::tbase::octal);
  tcontroller controller{model};

  handle_input(controller, model, "0777");
  handle_input(controller, model, "0777777");
  handle_input(controller, model, "0777777");
  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{
          {"0777"},
          {std::string("0777") +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "777"},
          {std::string("-0777") +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "777"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0777"}, {"0777777"}, {"-0777777"}}));
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_g_base_10) {
  tmodel model;
  tcontroller controller{model};

  handle_input(controller, model, "100");
  handle_input(controller, model, "100000");
  handle_input(controller, model, "100000");
  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{
          {"100"},
          {std::string("100") +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "000"},
          {std::string("-100") +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "000"}}));
  EXPECT_TRUE(model.input_get().empty());

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"100"}, {"100000"}, {"-100000"}}));
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, key_char_control_g_base_16) {
  tmodel model;
  model.base_set(lib::tbase::hexadecimal);
  tcontroller controller{model};

  handle_input(controller, model, "0x111");
  handle_input(controller, model, "0x1111111");
  handle_input(controller, model, "0x1111111");
  controller.handle_keyboard_input(tmodifiers::control, 'n');
  EXPECT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{{"0x111"}, {"0x111'1111"}, {"-0x111'1111"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{{"0x111"}, {"0x1111111"}, {"-0x1111111"}}));
  EXPECT_TRUE(model.input_get().empty());
}

} // namespace calculator
