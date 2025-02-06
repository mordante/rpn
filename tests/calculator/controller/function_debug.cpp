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

#include <locale>

#include <gtest/gtest.h>

namespace calculator {

TEST(controller, debug_basic) {
  tmodel model;
  tcontroller controller{model};

  handle_input(controller, model, "42 i-42 42.0");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"42", "-42", "42"}}));
  EXPECT_TRUE(model.input_get().empty());

  model.diagnostics_set("Cleared");
  handle_input(controller, model, "debug");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"42 |u", "-42 |i", "42 |d"}}));
  EXPECT_TRUE(model.input_get().empty());

  model.diagnostics_set("Cleared");
  handle_input(controller, model, "debug");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"42", "-42", "42"}}));
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, debug_base_2) {
  tmodel model;
  model.base_set(lib::tbase::binary);
  tcontroller controller{model};

  handle_input(controller, model, "0b1111_0000 0b1111_0000");
  controller.handle_keyboard_input(tmodifiers::control, 'n');
  ASSERT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0b1111'0000"}, {"-0b1111'0000"}}));

  handle_input(controller, model, "debug");
  EXPECT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{{"0b1111'0000 |u"}, {"-0b1111'0000 |i"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0b11110000 |u"}, {"-0b11110000 |i"}}));
}

TEST(controller, debug_base_8) {
  tmodel model;
  model.base_set(lib::tbase::octal);
  tcontroller controller{model};

  handle_input(controller, model, "0777_555 0777_555");
  controller.handle_keyboard_input(tmodifiers::control, 'n');
  ASSERT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{
          {std::string{"0777"} +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "555"},
          {std::string{"-0777"} +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "555"}}));

  handle_input(controller, model, "debug");
  EXPECT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{
          {std::string{"0777"} +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "555 |u"},
          {std::string{"-0777"} +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "555 |i"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0777555 |u"}, {"-0777555 |i"}}));
}

TEST(controller, debug_base_10) {
  tmodel model;
  tcontroller controller{model};

  handle_input(controller, model, "123456, i-123456");
  ASSERT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{
          {std::string{"123"} +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "456"},
          {std::string{"-123"} +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "456"}}));

  handle_input(controller, model, "debug");
  EXPECT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{
          {std::string{"123"} +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "456 |u"},
          {std::string{"-123"} +
           // Uses the separator of the current locale.
           std::use_facet<std::numpunct<char>>(std::locale()).thousands_sep() +
           "456 |i"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"123456 |u"}, {"-123456 |i"}}));
}

TEST(controller, debug_base_16) {
  tmodel model;
  model.base_set(lib::tbase::hexadecimal);
  tcontroller controller{model};

  handle_input(controller, model, "0x1234_5678 0x1234_5678");
  controller.handle_keyboard_input(tmodifiers::control, 'n');
  ASSERT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0x1234'5678"}, {"-0x1234'5678"}}));

  handle_input(controller, model, "debug");
  EXPECT_EQ(
      model.stack().strings(),
      (std::vector<std::string>{{"0x1234'5678 |u"}, {"-0x1234'5678 |i"}}));

  controller.handle_keyboard_input(tmodifiers::control, 'g');
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0x12345678 |u"}, {"-0x12345678 |i"}}));
}

} // namespace calculator
