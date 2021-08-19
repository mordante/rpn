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

TEST(model, base_set) {
  tmodel model;
  model.stack_push(tvalue{-42});
  model.stack_push(tvalue{0});
  model.stack_push(tvalue{42});
  model.stack_push(tvalue{100});
  EXPECT_EQ(model.stack(), (std::vector<std::string>{
                               {"@r-42"}, {"@r0"}, {"@r42"}, {"@r100"}}));

  model.base_set(tbase::binary);
  EXPECT_EQ(model.stack(),
            (std::vector<std::string>{
                {"@r-0b101010"}, {"@r0b0"}, {"@r0b101010"}, {"@r0b1100100"}}));

  model.base_set(tbase::octal);
  EXPECT_EQ(model.stack(), (std::vector<std::string>{
                               {"@r-052"}, {"@r0"}, {"@r052"}, {"@r0144"}}));

  model.base_set(tbase::decimal);
  EXPECT_EQ(model.stack(), (std::vector<std::string>{
                               {"@r-42"}, {"@r0"}, {"@r42"}, {"@r100"}}));

  model.base_set(tbase::hexadecimal);
  EXPECT_EQ(model.stack(),
            (std::vector<std::string>{
                {"@r-0x2a"}, {"@r0x0"}, {"@r0x2a"}, {"@r0x64"}}));
}
} // namespace calculator
