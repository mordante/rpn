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

import <type_traits>;

#include <gtest/gtest.h>

namespace calculator {

TEST(model, base_set) {
  tmodel model;
  model.grouping_toggle();
  model.stack().push(tvalue{uint64_t(0)});
  model.stack().push(tvalue{uint64_t(42)});
  model.stack().push(tvalue{uint64_t(100)});
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0"}, {"42"}, {"100"}}));

  model.base_set(tbase::binary);
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0b0"}, {"0b101010"}, {"0b1100100"}}));

  model.base_set(tbase::octal);
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0"}, {"052"}, {"0144"}}));

  model.base_set(tbase::decimal);
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0"}, {"42"}, {"100"}}));

  model.base_set(tbase::hexadecimal);
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0x0"}, {"0x2a"}, {"0x64"}}));
}
} // namespace calculator
