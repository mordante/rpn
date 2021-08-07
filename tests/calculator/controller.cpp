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

#include <gtest/gtest.h>

namespace calculator {

TEST(controller, constructor) {
  tmodel model;
  const tcontroller controller{model};
  EXPECT_TRUE(noexcept(tcontroller{model}));

  //
  //
  //
  //
  //
  // XXX Backport to other tests..
}

TEST(controller, copy_constructor) {
  static_assert(!std::is_copy_constructible_v<tcontroller>,
                "Implement the proper tests.");
}

TEST(controller, move_constructor) {
  static_assert(!std::is_move_constructible_v<tcontroller>,
                "Implement the proper tests.");
}

TEST(controller, destructor) {
  tmodel model;
  EXPECT_TRUE(noexcept(tcontroller{model}.~tcontroller()));
}

TEST(controller, copy_assignment) {
  static_assert(!std::is_copy_assignable_v<tcontroller>,
                "Implement the proper tests.");
}

TEST(controller, move_assignment) {
  static_assert(!std::is_move_assignable_v<tcontroller>,
                "Implement the proper tests.");
}

TEST(controller, push) {
  tmodel model;
  tcontroller controller{model};
  EXPECT_TRUE(noexcept(controller.push()));
}

static std::string format_error(const char *message) {
#if defined(__cpp_lib_format)
  return std::format("[ERR]{:>80.79}", message);
#else
  return message;
#endif
}

TEST(controller, push_duplicate_empty_stack) {
  tmodel model;
  tcontroller controller{model};

  controller.push();
  EXPECT_EQ(model.diagnostics_get(), format_error("Stack is empty"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, push_duplicate_non_empty_stack) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);

  controller.push();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 2);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, push_value_0) {
  tmodel model;
  tcontroller controller{model};
  model.input_append("0");

  controller.push();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 0);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, push_value_min) {
  tmodel model;
  tcontroller controller{model};
  const int64_t minimum = -9223372036854775807ll - 1;
  static_assert(std::numeric_limits<decltype(model.stack_pop().get())>::min() ==
                    minimum,
                "Update the test below");

  model.input_append("-9223372036854775808");
  controller.push();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), minimum);
  EXPECT_TRUE(model.input_get().empty());

  model.input_append("-9223372036854775809");
  controller.push();
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Value outside of the representable range"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, push_value_max) {
  tmodel model;
  tcontroller controller{model};
  const int64_t maximum = 9223372036854775807ll;
  static_assert(std::numeric_limits<decltype(model.stack_pop().get())>::max() ==
                    maximum,
                "Update the test below");

  model.input_append("9223372036854775807");
  controller.push();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), maximum);
  EXPECT_TRUE(model.input_get().empty());

  model.input_append("9223372036854775808");
  controller.push();
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Value outside of the representable range"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, push_invalid_input) {
  tmodel model;
  tcontroller controller{model};

  model.input_append("a");
  controller.push();
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, push_invalid_input_after_valid_value) {
  tmodel model;
  tcontroller controller{model};

  model.input_append("0a");
  controller.push();
  EXPECT_EQ(model.diagnostics_get(), format_error("Invalid numeric value"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
}

} // namespace calculator
