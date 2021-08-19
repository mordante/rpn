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

// *** SMF ***

TEST(model, default_constructor) {
  const tmodel model;
  static_assert(noexcept(tmodel{}));
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(model, copy_constructor) {
  static_assert(!std ::is_copy_constructible_v<tmodel>,
                "Implement the proper tests.");
}

TEST(model, move_constructor) {
  static_assert(!std ::is_move_constructible_v<tmodel>,
                "Implement the proper tests.");
}

TEST(model, destructor) { static_assert(noexcept(tmodel{}.~tmodel())); }

TEST(model, copy_assignment) {
  static_assert(!std ::is_copy_assignable_v<tmodel>,
                "Implement the proper tests.");
}

TEST(model, move_assignment) {
  static_assert(!std ::is_move_assignable_v<tmodel>,
                "Implement the proper tests.");
}

// *** Stack operations ***

TEST(model, stack_empty) {
  const tmodel model;
  static_assert(noexcept(model.stack_empty()));
}

TEST(model, stack_size) {
  const tmodel model;
  static_assert(noexcept(model.stack_size()));
}

TEST(model, stack_base_default) {
  {
    const tmodel model;
    static_assert(noexcept(model.stack()));
  }

  tmodel model;
  model.stack_push(tvalue{42});
  model.stack_push(tvalue{100});
  EXPECT_EQ(model.stack(), (std::vector<std::string>{{"@r42"}, {"@r100"}}));

  model.stack_drop();
  model.stack_drop();
  EXPECT_EQ(model.stack(), std::vector<std::string>{});
}

TEST(model, stack_base_2) {
  tmodel model;
  model.base_set(tbase::binary);
  model.stack_push(tvalue{42});
  EXPECT_EQ(model.stack(), std::vector<std::string>{"@r0b101010"});

  model.stack_drop();
  EXPECT_EQ(model.stack(), std::vector<std::string>{});
}

TEST(model, stack_base_8) {
  tmodel model;
  model.base_set(tbase::octal);
  model.stack_push(tvalue{42});
  EXPECT_EQ(model.stack(), std::vector<std::string>{"@r052"});

  model.stack_drop();
  EXPECT_EQ(model.stack(), std::vector<std::string>{});
}

TEST(model, stack_base_10) {
  tmodel model;
  model.base_set(tbase::decimal);
  model.stack_push(tvalue{42});
  EXPECT_EQ(model.stack(), std::vector<std::string>{"@r42"});

  model.stack_drop();
  EXPECT_EQ(model.stack(), std::vector<std::string>{});
}

TEST(model, stack_base_16) {
  tmodel model;
  model.base_set(tbase::hexadecimal);
  model.stack_push(tvalue{42});
  EXPECT_EQ(model.stack(), std::vector<std::string>{"@r0x2a"});

  model.stack_drop();
  EXPECT_EQ(model.stack(), std::vector<std::string>{});
}

TEST(model, stack_push) {
  tmodel model;

  model.stack_push(tvalue{42});
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_FALSE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_TRUE(model.input_get().empty());

  model.stack_push(tvalue{42});
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_FALSE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 2);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(model, stack_pop) {
  tmodel model;
  tvalue value;

  model.stack_push(tvalue{42});
  model.stack_push(tvalue{1});

  value = model.stack_pop();
  EXPECT_EQ(value, 1);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_FALSE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 1);
  EXPECT_TRUE(model.input_get().empty());

  value = model.stack_pop();
  EXPECT_EQ(value, 42);
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
  EXPECT_TRUE(model.input_get().empty());

  EXPECT_THROW(model.stack_drop(), std::out_of_range);
}

// *** Diagnostic operations ***

TEST(model, diagnostics_get) {
  const tmodel model;
  static_assert(noexcept(model.diagnostics_get()));
}

TEST(model, diagnostics_set) {
  tmodel model;

  model.diagnostics_set("abc");
  EXPECT_EQ(model.diagnostics_get(), "abc");
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
  EXPECT_TRUE(model.input_get().empty());

  model.diagnostics_set("");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_TRUE(model.input_get().empty());
  EXPECT_EQ(model.stack_size(), 0);
}

TEST(model, diagnostics_clear) {
  tmodel model;
  static_assert(noexcept(model.diagnostics_clear()));

  model.diagnostics_set("abc");

  model.diagnostics_clear();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
  EXPECT_TRUE(model.input_get().empty());
}

// *** Input operations ***

TEST(model, input_get) {
  const tmodel model;
  static_assert(noexcept(model.input_get()));
}

TEST(model, input_append_char) {
  tmodel model;

  model.input_append('a');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
  EXPECT_EQ(model.input_get(), "a");

  model.input_append('b');
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
  EXPECT_EQ(model.input_get(), "ab");
}

TEST(model, input_append_string) {
  tmodel model;

  model.input_append("abc");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
  EXPECT_EQ(model.input_get(), "abc");

  model.input_append("def");
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
  EXPECT_EQ(model.input_get(), "abcdef");
}

TEST(model, input_clear) {
  tmodel model;
  static_assert(noexcept(model.input_clear()));

  model.input_append("abc");

  model.input_clear();
  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
  EXPECT_TRUE(model.input_get().empty());
}

TEST(model, input_steal) {
  tmodel model;
  model.input_append("abc");

  const std::string input = model.input_steal();
  EXPECT_EQ(input, "abc");

  EXPECT_TRUE(model.diagnostics_get().empty());
  EXPECT_TRUE(model.stack_empty());
  EXPECT_EQ(model.stack_size(), 0);
  EXPECT_TRUE(model.input_get().empty());
}

} // namespace calculator
