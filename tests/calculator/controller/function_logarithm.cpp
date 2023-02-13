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

import <string_view>;

#include <gtest/gtest.h>

namespace calculator {

// TODO Make this a generic available test
static void test_require_1_element(std::string_view input) {
  {
    tmodel model;
    tcontroller controller{model};

    handle_input(controller, model, input);
    EXPECT_EQ(model.diagnostics_get(),
              format_error("The stack doesn't contain an element"));
    EXPECT_TRUE(model.stack().empty());
    EXPECT_EQ(model.input_get(), input);
  }
}

/*** *** LG *** ***/

TEST(controller, lg_too_few_elements) { test_require_1_element("lg"); }

TEST(controller, lg_stack) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  handle_input(controller, model, "2");
  handle_input(controller, model, "lg");

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"1"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, lg_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  handle_input(controller, model, "2 lg");

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"1"});
  EXPECT_TRUE(model.input_get().empty());
}

/*** *** LN *** ***/

TEST(controller, ln_too_few_elements) { test_require_1_element("ln"); }

TEST(controller, ln_stack) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  handle_input(controller, model, "2.71828");
  handle_input(controller, model, "ln");

  EXPECT_TRUE(model.diagnostics_get().empty());
  // Note this test is fragile, the exact result is 1.
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"0.999999"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, ln_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  handle_input(controller, model, "2.71828 ln");

  EXPECT_TRUE(model.diagnostics_get().empty());
  // Note this test is fragile, the exact result is 1.
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"0.999999"});
  EXPECT_TRUE(model.input_get().empty());
}

/*** *** LOG *** ***/

TEST(controller, log_too_few_elements) { test_require_1_element("log"); }

TEST(controller, log_stack) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  handle_input(controller, model, "10");
  handle_input(controller, model, "log");

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"1"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(controller, log_input) {
  tmodel model;
  tcontroller controller{model};
  model.diagnostics_set("Cleared");

  handle_input(controller, model, "10 log");

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"1"});
  EXPECT_TRUE(model.input_get().empty());
}

} // namespace calculator
