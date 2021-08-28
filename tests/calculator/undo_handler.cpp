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

import calculator.undo_handler;

import calculator.model;

#include <gtest/gtest.h>

namespace calculator {

TEST(undo, basic_undo_redo) {
  tmodel model;
  tundo_handler undo_handler;

  for (int i = 0; i < 4; ++i) {
    ttransaction transaction{model};
    transaction.push(tvalue(uint64_t(i)));
    undo_handler.add(std::move(transaction).release());
  }

  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0"}, {"1"}, {"2"}, {"3"}}));
  undo_handler.undo();
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0"}, {"1"}, {"2"}}));
  undo_handler.undo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{{"0"}, {"1"}}));
  undo_handler.undo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{{"0"}}));
  undo_handler.undo();
  EXPECT_TRUE(model.stack().empty());

  undo_handler.redo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{{"0"}}));
  undo_handler.redo();
  EXPECT_EQ(model.stack().strings(), (std::vector<std::string>{{"0"}, {"1"}}));
  undo_handler.redo();
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0"}, {"1"}, {"2"}}));
  undo_handler.redo();
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0"}, {"1"}, {"2"}, {"3"}}));
}

TEST(undo, discard_history) {
  tmodel model;
  tundo_handler undo_handler;

  EXPECT_THROW(undo_handler.undo(), std::range_error);
  for (int i = 0; i < 4; ++i) {
    ttransaction transaction{model};
    transaction.push(tvalue(uint64_t(i)));
    undo_handler.add(std::move(transaction).release());
  }

  undo_handler.undo();
  undo_handler.undo();
  {
    ttransaction transaction{model};
    transaction.push(tvalue(uint64_t(42)));
    undo_handler.add(std::move(transaction).release());
  }
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0"}, {"1"}, {"42"}}));
  EXPECT_THROW(undo_handler.redo(), std::range_error);

  undo_handler.undo();
  undo_handler.undo();
  undo_handler.undo();
  EXPECT_TRUE(model.stack().empty());
  undo_handler.redo();
  undo_handler.redo();
  undo_handler.redo();
  EXPECT_EQ(model.stack().strings(),
            (std::vector<std::string>{{"0"}, {"1"}, {"42"}}));
}

TEST(undo, underflow) {
  tmodel model;
  tundo_handler undo_handler;

  EXPECT_THROW(undo_handler.undo(), std::range_error);
  for (int i = 0; i < 4; ++i) {
    ttransaction transaction{model};
    transaction.push(tvalue(uint64_t(i)));
    undo_handler.add(std::move(transaction).release());
  }
  undo_handler.undo();
  undo_handler.undo();
  undo_handler.undo();
  undo_handler.undo();
  EXPECT_THROW(undo_handler.undo(), std::range_error);
}

TEST(undo, overflow) {
  tmodel model;
  tundo_handler undo_handler;

  EXPECT_THROW(undo_handler.redo(), std::range_error);
  for (int i = 0; i < 4; ++i) {
    ttransaction transaction{model};
    transaction.push(tvalue(uint64_t(i)));
    undo_handler.add(std::move(transaction).release());
  }
  undo_handler.undo();
  undo_handler.undo();
  undo_handler.undo();
  undo_handler.undo();
  undo_handler.redo();
  undo_handler.redo();
  undo_handler.redo();
  undo_handler.redo();
  EXPECT_THROW(undo_handler.redo(), std::range_error);
}
} // namespace calculator
