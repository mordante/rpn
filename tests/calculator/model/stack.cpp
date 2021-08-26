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

import<type_traits>;

#include <gtest/gtest.h>

namespace calculator {

TEST(model, stack_drop) {
  tmodel model;
  model.stack_push(tvalue(0));

  model.stack_drop();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.input_get().empty());
  EXPECT_TRUE(model.stack_empty());
}

TEST(model, stack_display_double) {
  tmodel model;
  model.stack_push(tvalue{-42.0});
  model.stack_push(tvalue{0.1});
  model.stack_push(tvalue{42.23});
  model.stack_push(tvalue{100.456});
  // Note these results are expected to change when std::format will be used.
  EXPECT_EQ(model.stack(), (std::vector<std::string>{
                               {"-42"}, {"0.1"}, {"42.23"}, {"100.456"}}));
}
} // namespace calculator
