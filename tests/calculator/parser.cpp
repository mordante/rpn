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

import calculator.parser;

#include <gtest/gtest.h>

namespace calculator {

TEST(parser, no_value) {
  tparser parser;

  EXPECT_EQ(parser.process(), std::vector<tparsed_string>{});
}

TEST(parser, multiple_values) {
  tparser parser;

  parser.append("1 10 abc 1. 1e1 1.e1 i42 i-42 100a");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::unsigned_value, "1"},
                {tparsed_string::ttype::unsigned_value, "10"},
                {tparsed_string::ttype::string_value, "abc"},
                {tparsed_string::ttype::floating_point_value, "1."},
                {tparsed_string::ttype::floating_point_value, "1e1"},
                {tparsed_string::ttype::floating_point_value, "1.e1"},
                {tparsed_string::ttype::signed_value, "42"},
                {tparsed_string::ttype::signed_value, "-42"},
                {tparsed_string::ttype::invalid_value, ""}}));
}

TEST(parser, reset) {
  tparser parser;

  parser.append("0x");
  EXPECT_EQ(parser.process(), (std::vector<tparsed_string>{
                                  {tparsed_string::ttype::invalid_value, ""}}));

  parser.reset();
  parser.append("42");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::unsigned_value, "42"}}));
}

TEST(parser, valid_string_value) {
  tparser parser;

  parser.append("abc");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::string_value, "abc"}}));
}

TEST(parser, do_not_accept_minus) {
  tparser parser;

  EXPECT_FALSE(parser.accept_minus());

  parser.append('1');
  EXPECT_FALSE(parser.accept_minus());

  parser.append('a');
  EXPECT_FALSE(parser.accept_minus());

  parser.reset();
  parser.append('a');
  EXPECT_FALSE(parser.accept_minus());
}
} // namespace calculator
