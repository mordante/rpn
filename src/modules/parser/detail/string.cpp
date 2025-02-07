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

export module parser.detail.string;

export import parser.detail.base;
import std;

namespace parser {

/** The parser for @ref ttype::string_value. */
export class tparser_string final : public tparser_ {
public:
  std::unique_ptr<tparser_> parse(char c) override {
    if (complete_)
      throw std::logic_error("Can't parse a complete value");

    if (!handle_input_separator(c))
      buffer_ += c;

    return nullptr;
  }

  bool accept_minus() const noexcept override { return false; }
  bool is_complete() const noexcept override { return complete_; }

private:
  bool complete_{false};
  std::string buffer_{};

  ttoken make_complete() override {
    complete_ = true;
    return ttoken{ttoken::ttype::string_value, buffer_};
  }
};

} // namespace parser
