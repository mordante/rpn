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

TEST(controller, trunc_too_few_elements) {
  tmodel model;
  tcontroller controller{model};

  handle_input(controller, model, "trunc");

  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain an element"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "trunc");
}

TEST(controller, trunc_stack) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  handle_input(controller, model, "1.1");
  handle_input(controller, model, "trunc");

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"1"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, trunc_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  handle_input(controller, model, "1.1 trunc");

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"1"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, trunc_invalid_type) {
  tmodel model;
  tcontroller controller{model};

  handle_input(controller, model, "1 trunc");

  EXPECT_EQ(model.diagnostics_get(), format_error("Not a floating-point"));
  EXPECT_TRUE(model.stack().empty());
  EXPECT_EQ(model.input_get(), "1 trunc");
}

} // namespace calculator
