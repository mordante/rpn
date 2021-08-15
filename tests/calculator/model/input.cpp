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

import calculator.model;

#include <gtest/gtest.h>

namespace calculator {

TEST(model, input_pop_back) {
  tmodel model;
  EXPECT_FALSE(model.input_pop_back());

  model.input_append("abc");
  EXPECT_TRUE(model.input_pop_back());
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.input_get(), "ab");
  EXPECT_TRUE(model.stack_empty());

  EXPECT_TRUE(model.input_pop_back());
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_EQ(model.input_get(), "a");
  EXPECT_TRUE(model.stack_empty());

  EXPECT_TRUE(model.input_pop_back());
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.input_get().empty());
  EXPECT_TRUE(model.stack_empty());

  EXPECT_FALSE(model.input_pop_back());
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.input_get().empty());
  EXPECT_TRUE(model.stack_empty());
}
} // namespace calculator
