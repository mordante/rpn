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

TEST(parser, valid_signed_value_0) {
  {
    tparser parser;

    parser.append("i0");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::signed_value, "0"}}));
  }
  {
    tparser parser;

    parser.append("i0 ");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::signed_value, "0"}}));
  }
}

TEST(parser, valid_signed_value_positive) {
  {
    {
      tparser parser;

      parser.append("i100");
      EXPECT_EQ(parser.process(),
                (std::vector<ttoken>{{ttoken::ttype::signed_value, "100"}}));
    }

    tparser parser;

    parser.append("i100 ");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::signed_value, "100"}}));
  }
}

TEST(parser, valid_signed_value_negative) {
  {
    tparser parser;

    parser.append("i-100");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::signed_value, "-100"}}));
  }
  {
    tparser parser;

    parser.append("i-100 ");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::signed_value, "-100"}}));
  }
}

TEST(parser, invalid_signed_value_char_less_than_0_as_string) {
  {
    tparser parser;

    parser.append("i0'"); // '\'' < '0' for ASCII
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "i0'"}}));
  }
  {
    tparser parser;

    parser.append("i1'"); // '\'' < '0' for ASCII
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "i1'"}}));
  }
  {
    tparser parser;

    parser.append("i0' "); // '\'' < '0' for ASCII
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "i0'"}}));
  }
  {
    tparser parser;

    parser.append("i1' "); // '\'' < '0' for ASCII
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "i1'"}}));
  }
}

TEST(parser, invalid_value_parsing_as_string) {
  {
    tparser parser;

    parser.append("i1abc");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "i1abc"}}));
  }
  {
    tparser parser;

    parser.append("i1abc ");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "i1abc"}}));
  }
}

TEST(parser, valid_signed_value_grouping) {
  tparser parser;

  // TODO Add more restrictions to where the grouping is allowed.
  parser.append("i-1_,0_,0_,");
  EXPECT_EQ(parser.process(),
            (std::vector<ttoken>{{ttoken::ttype::signed_value, "-100"}}));
}

TEST(parser, invalid_signed_value_grouping) {
  {
    tparser parser;

    parser.append("i-,1");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("i-_1");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
}

TEST(parser, invalid_signed_value_grouping_as_string) {
  {
    tparser parser;

    parser.append("i,-1");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "i,-1"}}));
  }
  {
    tparser parser;

    parser.append("i_-1");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "i_-1"}}));
  }
}

TEST(parser, invalid_signed_value) {
  {
    tparser parser;

    parser.append("i-");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
  {
    tparser parser;

    parser.append("i- ");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::invalid_value, ""}}));
  }
}

TEST(parser, invalid_signed_value_as_string) {
  {
    tparser parser;

    parser.append("i");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "i"}}));
  }
  {
    tparser parser;

    parser.append("i ");
    EXPECT_EQ(parser.process(),
              (std::vector<ttoken>{{ttoken::ttype::string_value, "i"}}));
  }
}
} // namespace parser
