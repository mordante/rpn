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

#if __cplusplus <= 202002L
import<algorithm>;
#endif

#include <gtest/gtest.h>

constexpr std::string_view all_characters =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789"
    "`-=~!@#$%^&*()_+[]\\{}|;':\",./<>?";

constexpr std::string_view special_characters_no_modifier = "+-/*&|^~<>";
constexpr std::string_view special_characters_control = "bodhnzZ";

namespace calculator {
TEST(controller, key_char_default_special_characters_no_modifier) {
  for (char c : special_characters_no_modifier) {
    tmodel model;
    tcontroller controller{model};

    controller.handle_keyboard_input(tmodifiers::none, c);
    EXPECT_FALSE(model.diagnostics_get().empty());
    EXPECT_TRUE(model.stack().empty());
    EXPECT_TRUE(model.input_get().empty());
  }
}

TEST(controller, key_char_default_non_special_characters) {
  for (char c : all_characters) {
#if __cplusplus > 202002L
    if (special_characters_no_modifier.contains(c))
#else
    if (std::any_of(special_characters_no_modifier.begin(),
                    special_characters_no_modifier.end(),
                    [c](char special) { return c == special; }))
#endif
      continue;

    tmodel model;
    tcontroller controller{model};

    controller.handle_keyboard_input(tmodifiers::none, c);
    EXPECT_TRUE(model.diagnostics_get().empty());
    EXPECT_TRUE(model.stack().empty());
    EXPECT_EQ(model.input_get(), std::string(1, c));
  }
}

TEST(controller, key_char_default_control) {
  for (char c : all_characters) {
#if __cplusplus > 202002L
    if (special_characters_control.contains(c))
#else
    if (std::any_of(special_characters_control.begin(),
                    special_characters_control.end(),
                    [c](char special) { return c == special; }))
#endif
      continue;
    tmodel model;
    tcontroller controller{model};

    controller.handle_keyboard_input(tmodifiers::control, c);
    EXPECT_TRUE(model.diagnostics_get().empty());
    EXPECT_TRUE(model.stack().empty());
    EXPECT_TRUE(model.input_get().empty());
  }
}
} // namespace calculator
