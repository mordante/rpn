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

module;

#include <memory>
#include <string>

export module parser.detail.invalid_value;

export import parser.detail.base;

namespace parser {

/** The parser for @ref ttype::invalid_value. */
export class tparser_invalid_value final : public tparser_ {
public:
  std::unique_ptr<tparser_> parse(char) override { return nullptr; }

  bool accept_minus() const noexcept override { return false; }
  bool is_complete() const noexcept override { return complete_; }

private:
  bool complete_{false};

  ttoken make_complete() override {
    complete_ = true;
    return ttoken{ttoken::ttype::invalid_value, std::string{}};
  }
};

} // namespace parser
