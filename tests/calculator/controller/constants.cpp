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

static void validate(std::string_view input, std::string expected) {
  {
    tmodel model;
    tcontroller controller{model};
    model.diagnostics_set("Cleared");
    model.input_append(input);
    controller.handle_keyboard_input(tkey::enter);

    EXPECT_TRUE(model.diagnostics_get().empty());
    EXPECT_EQ(model.stack().strings(), std::vector<std::string>{expected});
    EXPECT_TRUE(model.input_get().empty());
  }
  {
    tmodel model;
    tcontroller controller{model};
    model.diagnostics_set("Cleared");
    model.input_append(input);
    model.input_append(" 0");
    controller.handle_keyboard_input(tkey::enter);

    EXPECT_TRUE(model.diagnostics_get().empty());
    EXPECT_EQ(model.stack().strings(),
              (std::vector<std::string>{{expected}, {"0"}}));
    EXPECT_TRUE(model.input_get().empty());
  }
}

TEST(controller, constant_signed_integral) {
  validate("int8_min", "-128");
  validate("int16_min", "-32768");
  validate("int32_min", "-2147483648");
  validate("int64_min", "-9223372036854775808");

  validate("int8_max", "127");
  validate("int16_max", "32767");
  validate("int32_max", "2147483647");
  validate("int64_max", "9223372036854775807");
}

TEST(controller, constant_unsigned_integral) {
  validate("uint8_max", "255");
  validate("uint16_max", "65535");
  validate("uint32_max", "4294967295");
  validate("uint64_max", "18446744073709551615");
}

TEST(controller, constant_floating_point) {
  validate("float_min", "1.17549e-38");
  validate("double_min", "2.22507e-308");

  validate("float_max", "3.40282e+38");
  validate("double_max", "1.79769e+308");
}

TEST(controller, constant_special) {
  validate("e", "2.71828");
  validate("pi", "3.14159");
}
} // namespace calculator
