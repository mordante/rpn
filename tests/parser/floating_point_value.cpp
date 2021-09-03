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

import parser;

#include <gtest/gtest.h>

namespace parser {

TEST(parser, valid_floating_point_value_0) {
  {
    tparser parser;

    parser.append("0.");
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, "0."}}));
  }
  {
    tparser parser;

    parser.append("0. ");
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, "0."}}));
  }
  {
    tparser parser;

    parser.append(".0");
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, ".0"}}));
  }
  {
    tparser parser;

    parser.append(".0 ");
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, ".0"}}));
  }
}

TEST(parser, valid_floating_point_value) {
  {
    tparser parser;

    parser.append("123.");
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, "123."}}));
  }
  {
    tparser parser;

    parser.append("123. ");
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, "123."}}));
  }
  {
    tparser parser;

    parser.append(".123");
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, ".123"}}));
  }
  {
    tparser parser;

    parser.append(".123 ");
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, ".123"}}));
  }
}

TEST(parser, valid_floating_point_value_positive_exponent) {
  {
    tparser parser;

    parser.append("123.e123");
    EXPECT_FALSE(parser.accept_minus());
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{
                  {ttoken::ttype::floating_point_value, "123.e123"}}));
  }
  {
    tparser parser;

    parser.append("123.e123 ");
    EXPECT_FALSE(parser.accept_minus());
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{
                  {ttoken::ttype::floating_point_value, "123.e123"}}));
  }
  {
    tparser parser;

    parser.append(".123e123");
    EXPECT_FALSE(parser.accept_minus());
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{
                  {ttoken::ttype::floating_point_value, ".123e123"}}));
  }
}

TEST(parser, valid_floating_point_value_zero_exponent) {
  {
    tparser parser;

    parser.append("123.e0");
    EXPECT_FALSE(parser.accept_minus());
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, "123.e0"}}));
  }
  {
    tparser parser;

    parser.append("123.e0 ");
    EXPECT_FALSE(parser.accept_minus());
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, "123.e0"}}));
  }
  {
    tparser parser;

    parser.append(".123e0");
    EXPECT_FALSE(parser.accept_minus());
    EXPECT_EQ(
        parser.process(),
        (std::vector<ttoken>{{ttoken::ttype::floating_point_value, ".123e0"}}));
  }
}

TEST(parser, valid_floating_point_value_negative_exponent) {
  {
    tparser parser;

    parser.append("123.e");
    EXPECT_TRUE(parser.accept_minus());
    parser.append('-');
    EXPECT_FALSE(parser.accept_minus());
    parser.append("123");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{
                  {ttoken::ttype::floating_point_value, "123.e-123"}}));
  }
  {
    tparser parser;

    parser.append(".123e");
    EXPECT_TRUE(parser.accept_minus());
    parser.append('-');
    EXPECT_FALSE(parser.accept_minus());
    parser.append("123");
    EXPECT_FALSE(parser.accept_minus());
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{
                  {ttoken::ttype::floating_point_value, ".123e-123"}}));
  }
}

TEST(parser, invalid_floating_point_double_dot) {
  {
    tparser parser;

    parser.append("..");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("0..");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append(".0.");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
}

TEST(parser, invalid_floating_point_dot_in_exponent) {
  {
    tparser parser;

    parser.append("1e.");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("1e1.");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("1e-1.");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
}

TEST(parser, invalid_floating_point_only_a_dot) {
  tparser parser;

  parser.append('.');
  EXPECT_EQ(parser.process(),
            (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
}

TEST(parser, only_an_exponent_is_a_string) {
  {
    tparser parser;

    parser.append('e');
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "e"}}));
  }
  {
    tparser parser;

    parser.append("e1");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "e1"}}));
  }
  {
    tparser parser;

    // The parser accepts this but the engine will process the minus as a
    // binary operator.
    parser.append('e');
    EXPECT_FALSE(parser.accept_minus());
    parser.append('-');
    EXPECT_FALSE(parser.accept_minus());
    parser.append('1');
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "e-1"}}));
  }
}

TEST(parser, invalid_floating_point_exponent_no_value) {
  {
    tparser parser;

    parser.append("123.e");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append(".123e");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
}

TEST(parser, invalid_floating_point_exponent_only_minus) {
  {
    tparser parser;

    parser.append("123.e-");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append(".123e-");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
}

TEST(parser, valid_floating_point_value_grouping) {
  tparser parser;

  // TODO Add more restrictions to where the grouping is allowed.
  parser.append("1_,2_,_3,_,.e-1_,0_,");
  EXPECT_EQ(
      parser.process(),
      (std::vector<ttoken>{{ttoken::ttype::floating_point_value, "123.e-10"}}));
}

TEST(parser, invalid_floating_point_value_grouping_after_dot) {
  {
    tparser parser;

    parser.append("123._e-10");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("123.,e-10");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
}

TEST(parser, invalid_floating_point_value_grouping_after_e) {
  {
    tparser parser;

    parser.append("123.e_-10");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("123.e,-10");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
}

TEST(parser, invalid_floating_point_value_grouping_after_minus) {
  {
    tparser parser;

    parser.append("123.e-_10");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("123.e-,10");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
}
} // namespace parser
