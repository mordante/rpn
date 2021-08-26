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
  EXPECT_EQ(model.stack(),
            (std::vector<std::string>{{"-42"}, {"0"}, {"42"}, {"100"}}));

  model.base_set(tbase::binary);
  EXPECT_EQ(model.stack(),
            (std::vector<std::string>{
                {"-0b101010"}, {"0b0"}, {"0b101010"}, {"0b1100100"}}));

  model.base_set(tbase::octal);
  EXPECT_EQ(model.stack(),
            (std::vector<std::string>{{"-052"}, {"0"}, {"052"}, {"0144"}}));

  model.base_set(tbase::decimal);
  EXPECT_EQ(model.stack(),
            (std::vector<std::string>{{"-42"}, {"0"}, {"42"}, {"100"}}));

  model.base_set(tbase::hexadecimal);
  EXPECT_EQ(model.stack(),
            (std::vector<std::string>{{"-0x2a"}, {"0x0"}, {"0x2a"}, {"0x64"}}));
}
} // namespace calculator
