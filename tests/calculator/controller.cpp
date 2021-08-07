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

// *** SMF ***

TEST(controller, constructor) {
  tmodel model;
  const tcontroller controller{model};
  static_assert(noexcept(tcontroller{model}));
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
  static_assert(noexcept(tcontroller{model}.~tcontroller()));
}

TEST(controller, copy_assignment) {
  static_assert(!std::is_copy_assignable_v<tcontroller>,
                "Implement the proper tests.");
}

TEST(controller, move_assignment) {
  static_assert(!std::is_move_assignable_v<tcontroller>,
                "Implement the proper tests.");
}

// *** append ***

TEST(controller, append) {
  tmodel model;
  tcontroller controller{model};
  static_assert(noexcept(controller.append(std::string_view())));

  controller.append("abc");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.input_get(), "abc");

  controller.append("");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.input_get(), "abc");

  controller.append("def");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.input_get(), "abcdef");
}

TEST(controller, append_diagnostics_unchanged) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Unchanged");

  controller.append("abc");
  EXPECT_EQ(model.diagnostics_get(), "Unchanged");
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.input_get(), "abc");
}

// *** push ***

TEST(controller, push) {
  tmodel model;
  tcontroller controller{model};
  static_assert(noexcept(controller.push()));
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

TEST(controller, push_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  model.input_append("42");

  controller.push();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

// *** math_add ***

TEST(controller, math_add_too_few_elements) {
  tmodel model;
  tcontroller controller{model};

  controller.math_add();
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain two elements"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());

  model.stack_push(42);
  controller.math_add();
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain two elements"));
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_add_stack_input) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);
  model.input_append("42");

  controller.math_add();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 84);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_add_stack_stack) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);
  model.stack_push(42);

  controller.math_add();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 84);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_add_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  model.stack_push(42);
  model.stack_push(42);

  controller.math_add();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 84);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_add_input_invalid) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);
  model.input_append("abc");

  controller.math_add();
  EXPECT_EQ(model.diagnostics_get(), "Invalid numeric value");
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

// *** math_sub ***

TEST(controller, math_sub_too_few_elements) {
  tmodel model;
  tcontroller controller{model};

  controller.math_sub();
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain two elements"));
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());

  model.stack_push(42);
  controller.math_sub();
  EXPECT_EQ(model.diagnostics_get(),
            format_error("Stack doesn't contain two elements"));
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_sub_stack_input) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);
  model.input_append("21");

  controller.math_sub();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 21);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_sub_stack_stack) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);
  model.stack_push(21);

  controller.math_sub();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 21);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_sub_diagnostics_cleared) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");
  model.stack_push(42);
  model.stack_push(21);

  controller.math_sub();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 21);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, math_sub_input_invalid) {
  tmodel model;
  tcontroller controller{model};
  model.stack_push(42);
  model.input_append("abc");

  controller.math_sub();
  EXPECT_EQ(model.diagnostics_get(), "Invalid numeric value");
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_EQ(model.stack_pop().get(), 42);
  EXPECT_TRUE(model.input_get().empty());
}

} // namespace calculator
