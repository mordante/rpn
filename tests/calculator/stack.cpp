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

import calculator.stack;

import lib.base;

#include <type_traits>

#include <gtest/gtest.h>

namespace calculator {

TEST(stack, drop) {
  tstack stack;

  EXPECT_THROW(stack.drop(), std::out_of_range);

  stack.push(tvalue{uint64_t(42)});

  stack.drop();
  EXPECT_TRUE(stack.empty());
}

TEST(stack, display_double) {
  tstack stack;
  stack.push(tvalue{-42.0});
  stack.push(tvalue{0.1});
  stack.push(tvalue{42.23});
  stack.push(tvalue{100.456});
  // Note these results are expected to change when std::format will be used.
  EXPECT_EQ(stack.strings(), (std::vector<std::string>{
                                 {"-42"}, {"0.1"}, {"42.23"}, {"100.456"}}));
}

TEST(stack, empty) {
  const tstack stack;
  static_assert(noexcept(stack.empty()));
}

TEST(stack, size) {
  const tstack stack;
  static_assert(noexcept(stack.size()));
}

TEST(stack, base_default) {
  {
    const tstack stack;
    static_assert(noexcept(stack.strings()));
  }

  tstack stack;
  stack.push(tvalue{uint64_t(42)});
  stack.push(tvalue{uint64_t(100)});
  EXPECT_EQ(stack.strings(), (std::vector<std::string>{{"42"}, {"100"}}));

  stack.drop();
  stack.drop();
  EXPECT_EQ(stack.strings(), std::vector<std::string>{});
}

TEST(stack, base_2) {
  tstack stack;
  stack.grouping_toggle();
  stack.base_set(lib::tbase::binary);
  stack.push(tvalue{uint64_t(42)});
  EXPECT_EQ(stack.strings(), std::vector<std::string>{"0b101010"});

  stack.drop();
  EXPECT_EQ(stack.strings(), std::vector<std::string>{});
}

TEST(stack, base_8) {
  tstack stack;
  stack.base_set(lib::tbase::octal);
  stack.push(tvalue{uint64_t(42)});
  EXPECT_EQ(stack.strings(), std::vector<std::string>{"052"});

  stack.drop();
  EXPECT_EQ(stack.strings(), std::vector<std::string>{});
}

TEST(stack, base_10) {
  tstack stack;
  stack.base_set(lib::tbase::decimal);
  stack.push(tvalue{uint64_t(42)});
  EXPECT_EQ(stack.strings(), std::vector<std::string>{"42"});

  stack.drop();
  EXPECT_EQ(stack.strings(), std::vector<std::string>{});
}

TEST(stack, base_16) {
  tstack stack;
  stack.base_set(lib::tbase::hexadecimal);
  stack.push(tvalue{uint64_t(42)});
  EXPECT_EQ(stack.strings(), std::vector<std::string>{"0x2a"});

  stack.drop();
  EXPECT_EQ(stack.strings(), std::vector<std::string>{});
}

TEST(stack, push) {
  tstack stack;

  stack.push(tvalue{uint64_t(42)});
  EXPECT_FALSE(stack.empty());
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.strings(), std::vector<std::string>{"42"});

  stack.push(tvalue{uint64_t(42)});
  EXPECT_FALSE(stack.empty());
  EXPECT_EQ(stack.size(), 2);
  EXPECT_EQ(stack.strings(), (std::vector<std::string>{{"42"}, {"42"}}));
}

TEST(stack, duplicate) {
  tstack stack;

  EXPECT_THROW(stack.duplicate(), std::out_of_range);

  stack.push(tvalue{uint64_t(42)});
  stack.duplicate();

  EXPECT_EQ(stack.strings(), (std::vector<std::string>{{"42"}, {"42"}}));
}

TEST(stack, pop) {
  tstack stack;

  EXPECT_THROW((void)stack.pop(), std::out_of_range);

  stack.push(tvalue{uint64_t(42)});
  stack.push(tvalue{uint64_t(1)});

  (void)stack.pop();
  EXPECT_FALSE(stack.empty());
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.strings(), std::vector<std::string>{"42"});

  (void)stack.pop();
  EXPECT_TRUE(stack.empty());
  EXPECT_EQ(stack.size(), 0);

  EXPECT_THROW(stack.drop(), std::out_of_range);
}

} // namespace calculator
