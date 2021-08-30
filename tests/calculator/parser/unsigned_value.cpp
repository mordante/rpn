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

TEST(parser, valid_unsigned_value_0) {
  tparser parser;

  parser.append('0');
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::unsigned_value, "0"}}));
}

TEST(parser, invalid_unsigned_value_char_less_than_0) {
  {
    tparser parser;

    parser.append("0'"); // '\'' < '0' for ASCII
    EXPECT_EQ(parser.process(),
              (std::vector<tparsed_string>{
                  {tparsed_string::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("1'"); // '\'' < '0' for ASCII
    EXPECT_EQ(parser.process(),
              (std::vector<tparsed_string>{
                  {tparsed_string::ttype::invalid_value, ""}}));
  }
}

TEST(parser, invalid_value_parsing) {
  tparser parser;

  parser.append("1abc");
  EXPECT_EQ(parser.process(), (std::vector<tparsed_string>{
                                  {tparsed_string::ttype::invalid_value, ""}}));
}

TEST(parser, valid_unsigned_value_invalid_prefix_0z) {
  tparser parser;

  parser.append("0z");
  EXPECT_EQ(parser.process(), (std::vector<tparsed_string>{
                                  {tparsed_string::ttype::invalid_value, ""}}));
}

TEST(parser, valid_unsigned_value_invalid_prefix_0b_only) {
  tparser parser;

  parser.append("0b");
  EXPECT_EQ(parser.process(), (std::vector<tparsed_string>{
                                  {tparsed_string::ttype::invalid_value, ""}}));
}

TEST(parser, valid_unsigned_value_invalid_prefix_0x_only) {
  tparser parser;

  parser.append("0x");
  EXPECT_EQ(parser.process(), (std::vector<tparsed_string>{
                                  {tparsed_string::ttype::invalid_value, ""}}));
}

TEST(parser, valid_unsigned_value_base_2) {
  tparser parser;

  parser.append("0b100");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::unsigned_value, "0b100"}}));
}

TEST(parser, invalid_unsigned_value_base_2) {
  tparser parser;

  parser.append("0b1002");
  EXPECT_EQ(parser.process(), (std::vector<tparsed_string>{
                                  {tparsed_string::ttype::invalid_value, ""}}));
}

TEST(parser, valid_unsigned_value_grouping_base_2) {
  tparser parser;

  // TODO Add more restrictions to where the grouping is allowed.
  parser.append("0_,b_,1,_0__0,,0_,_,");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::unsigned_value, "0b1000"}}));
}

TEST(parser, valid_unsigned_value_base_8) {
  tparser parser;

  parser.append("012345670");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::unsigned_value, "012345670"}}));
}

TEST(parser, invalid_unsigned_value_base_8) {
  {
    tparser parser;

    parser.append("08");
    EXPECT_EQ(parser.process(),
              (std::vector<tparsed_string>{
                  {tparsed_string::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("018");
    EXPECT_EQ(parser.process(),
              (std::vector<tparsed_string>{
                  {tparsed_string::ttype::invalid_value, ""}}));
  }
}

TEST(parser, valid_unsigned_value_grouping_base_8) {
  tparser parser;

  // TODO Add more restrictions to where the grouping is allowed.
  parser.append("0_,1,_2__3,,4_,_,");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::unsigned_value, "01234"}}));
}

TEST(parser, valid_unsigned_value_base_10) {
  tparser parser;

  parser.append("100");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::unsigned_value, "100"}}));
}

TEST(parser, invalid_unsigned_value_base_10) {
  {
    tparser parser;

    parser.append("1a");
    EXPECT_EQ(parser.process(),
              (std::vector<tparsed_string>{
                  {tparsed_string::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("10a");
    EXPECT_EQ(parser.process(),
              (std::vector<tparsed_string>{
                  {tparsed_string::ttype::invalid_value, ""}}));
  }
}

TEST(parser, valid_unsigned_value_grouping_base_10) {
  tparser parser;

  // TODO Add more restrictions to where the grouping is allowed.
  parser.append("1,_2__3,,4_,_,");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::unsigned_value, "1234"}}));
}

TEST(parser, valid_unsigned_value_base_16) {
  tparser parser;

  parser.append("0x1234567890abcdef");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{{tparsed_string::ttype::unsigned_value,
                                          "0x1234567890abcdef"}}));
}

TEST(parser, invalid_unsigned_value_base_16) {
  {
    tparser parser;

    parser.append("0x1g");
    EXPECT_EQ(parser.process(),
              (std::vector<tparsed_string>{
                  {tparsed_string::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("0xg");
    EXPECT_EQ(parser.process(),
              (std::vector<tparsed_string>{
                  {tparsed_string::ttype::invalid_value, ""}}));
  }
}

TEST(parser, valid_unsigned_value_grouping_base_16) {
  tparser parser;

  // TODO Add more restrictions to where the grouping is allowed.
  parser.append("0_,x_,1,_2__3,,4_,_,");
  EXPECT_EQ(parser.process(),
            (std::vector<tparsed_string>{
                {tparsed_string::ttype::unsigned_value, "0x1234"}}));
}

} // namespace calculator
