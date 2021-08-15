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
TEST(controller, math_mul_too_few_elements) {
  tmodel model;
  tcontroller controller{model};

  controller.math_mul();
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain two elements"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());

  model.stack_push(42);
  controller.math_mul();
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain two elements"));
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_mul_stack_input) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(2);
  model.input_append("21");

  controller.math_mul();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_mul_stack_stack) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(2);
  model.stack_push(21);

  controller.math_mul();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_mul_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  model.stack_push(2);
  model.stack_push(21);

  controller.math_mul();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_mul_input_invalid) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);
  model.input_append("abc");

  controller.math_mul();
  EXPECT_EQ(model.diagnostics_get(), "Invalid numeric value");
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}
} // namespace calculator
