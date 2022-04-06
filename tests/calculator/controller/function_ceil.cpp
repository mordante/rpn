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

TEST(controller, ceil_too_few_elements) {
  tmodel model;
  tcontroller controller{model};

  handle_input(controller, model, "ceil");

  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain an element"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "ceil");
}

TEST(controller, ceil_stack) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  handle_input(controller, model, "1.1");
  handle_input(controller, model, "ceil");

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"2"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, ceil_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  handle_input(controller, model, "1.1 ceil");

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"2"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, ceil_invalid_type) {
  tmodel model;
  tcontroller controller{model};

  handle_input(controller, model, "1 ceil");

  EXPECT_EQ(model.diagnostics_get(), format_error("Not a floating-point"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "1 ceil");
}

} // namespace calculator
