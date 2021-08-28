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

import calculator.transaction;

import calculator.model;

#include <gtest/gtest.h>

namespace calculator {

static_assert(!std::default_initializable<ttransaction>);
static_assert(!std::copy_constructible<ttransaction>);
static_assert(!std::move_constructible<ttransaction>);

TEST(transaction, no_exception_thrown) {
  tmodel model;
  model.stack().push(tvalue(uint64_t(42)));
  model.stack().push(tvalue(uint64_t(42)));
  model.input_append("abc");

  {
    ttransaction transaction{model};
    (void)transaction.input_steal();
    (void)transaction.pop();
  }
  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_TRUE(model.input_get().empty());
}

TEST(transaction, exception_thrown) {
  tmodel model;
  model.stack().push(tvalue(uint64_t(42)));
  model.stack().push(tvalue(uint64_t(42)));
  model.input_append("abc");

  try {
    ttransaction transaction{model};

    // The diagnostics and base aren't part of the transaction.
    model.diagnostics_set("Unchanged");
    model.base_set(tbase::binary);

    (void)transaction.input_steal();
    (void)transaction.pop();

    throw 42;
  } catch (...) {
  }
  EXPECT_EQ(model.diagnostics_get(), "Unchanged");
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0b101010"}, {"0b101010"}}));
  EXPECT_EQ(model.input_get(), "abc");
}

TEST(transaction, exception_thrown_after_release) {
  tmodel model;
  model.stack().push(tvalue(uint64_t(42)));
  model.stack().push(tvalue(uint64_t(42)));
  model.input_append("abc");

  try {
    ttransaction transaction{model};
    (void)transaction.input_steal();
    (void)transaction.pop();
    (void)std::move(transaction).release();

    throw 42;
  } catch (...) {
  }

  EXPECT_EQ(model.stack().strings(), std::vector<std::string>{"42"});
  EXPECT_TRUE(model.input_get().empty());
}

static_assert(!std::default_initializable<taction>);
static_assert(!std::copy_constructible<taction>);
static_assert(std::move_constructible<taction>);

TEST(action, input_steal) {
  tmodel model;
  model.input_append("abc");

  ttransaction transaction{model};
  (void)transaction.input_steal();
  taction action = std::move(transaction).release();

  model.input_append("zzz"); // Make sure the current content is replaced.
  action.undo();
  EXPECT_EQ(model.input_get(), "abc");
  action.redo();
  EXPECT_TRUE(model.input_get().empty());
}

TEST(action, push) {
  tmodel model;
  model.stack().push(tvalue(uint64_t(42)));

  ttransaction transaction{model};
  transaction.push(tvalue{uint64_t(0)});
  taction action = std::move(transaction).release();

  action.undo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{"42"}));
  action.redo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{{"42"}, {"0"}}));
}

TEST(action, pop) {
  tmodel model;
  model.stack().push(tvalue(uint64_t(42)));
  model.stack().push(tvalue(uint64_t(0)));

  ttransaction transaction{model};
  (void)transaction.pop();
  taction action = std::move(transaction).release();

  action.undo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{{"42"}, {"0"}}));
  action.redo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{"42"}));
}

TEST(action, drop) {
  tmodel model;
  model.stack().push(tvalue(uint64_t(42)));
  model.stack().push(tvalue(uint64_t(0)));

  ttransaction transaction{model};
  transaction.drop();
  taction action = std::move(transaction).release();

  action.undo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{{"42"}, {"0"}}));
  action.redo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{"42"}));
}

TEST(action, duplicate) {
  tmodel model;
  model.stack().push(tvalue(uint64_t(42)));

  ttransaction transaction{model};
  transaction.duplicate();
  taction action = std::move(transaction).release();

  action.undo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{"42"}));
  action.redo();
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"42"}, {"42"}}));
}

} // namespace calculator
