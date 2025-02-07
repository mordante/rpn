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

// TODO Rename unsiged_value to unsigned when that becomes valid.
export module parser.detail.unsigned_value;

import lib.base;

import parser.detail.floating_point;
import parser.detail.invalid_value;
import std;

namespace parser {

/** The parser for @ref ttype::unsigned_value. */
export class tparser_unsigned final : public tparser_ {
public:
  std::unique_ptr<tparser_> parse(char c) override {
    switch (state_) {
    case tstate::initial:
      return initial(c);
    case tstate::optional_prefix:
      return optional_prefix(c);
    case tstate::required_number:
    case tstate::optional_number:
      return number(c);
    case tstate::complete:
      throw std::logic_error("Can't parse a complete value");
    }
  }

  bool accept_minus() const noexcept override { return false; }
  bool is_complete() const noexcept override {
    return state_ == tstate::complete;
  }

private:
  enum class tstate {
    initial,
    optional_prefix,
    required_number,
    optional_number,
    complete
  };
  tstate state_{tstate::initial};
  lib::tbase base_{lib::tbase::decimal};
  std::string buffer_{};

  ttoken make_complete() override {
    ttoken result = in_terminal_state()
                        ? ttoken{ttoken::ttype::unsigned_value, buffer_}
                        : ttoken{ttoken::ttype::invalid_value, std::string{}};
    state_ = tstate::complete;
    return result;
  }

  std::unique_ptr<tparser_> initial(char c) {
    if (!std::isdigit(c))
      throw std::logic_error(
          "The caller should have validated input is a digit");

    buffer_ += c;
    state_ = c == '0' ? tstate::optional_prefix : tstate::optional_number;
    return nullptr;
  }

  std::unique_ptr<tparser_> optional_prefix(char c) {
    if (is_grouping(c))
      return nullptr;

    buffer_ += c;
    if (c == '.' || c == 'e')
      return create_parser_floating_point();

    switch (c) {
    case 'b':
      base_ = lib::tbase::binary;
      state_ = tstate::required_number;
      break;

    case 'x':
      base_ = lib::tbase::hexadecimal;
      state_ = tstate::required_number;
      break;

    default:
      // The string has an octal 0 prefix, 8 and 9 are invalid octal digits.
      if (c < '0' || c > '7')
        return std::make_unique<tparser_invalid_value>();

      base_ = lib::tbase::octal;
      state_ = tstate::optional_number;
      break;
    }

    return nullptr;
  }

  std::unique_ptr<tparser_> number(char c) {
    if (is_grouping(c))
      return nullptr;

    if (handle_input_separator(c))
      return nullptr;

    if (state_ == tstate::required_number)
      state_ = tstate::optional_number;

    buffer_ += c;
    switch (base_) {
    case lib::tbase::binary:
      if (c > '1')
        return std::make_unique<tparser_invalid_value>();
      break;

    case lib::tbase::octal:
      if (c > '7')
        return std::make_unique<tparser_invalid_value>();
      break;
    case lib::tbase::decimal:
      // Non-decimal bases aren't allowed in floating-point values.
      if (c == '.' || c == 'e')
        return create_parser_floating_point();
      if (c > '9')
        return std::make_unique<tparser_invalid_value>();
      break;
    case lib::tbase::hexadecimal:
      if (c > '9' && c > 'a' && c > 'f')
        return std::make_unique<tparser_invalid_value>();
      break;
    }
    // Expects ASCII
    if (c < '0')
      return std::make_unique<tparser_invalid_value>();

    return nullptr;
  }

  /** Is the parser in a state it's valid to be completed? */
  bool in_terminal_state() const noexcept {
    return state_ == tstate::optional_prefix ||
           state_ == tstate::optional_number;
  }

  std::unique_ptr<tparser_> create_parser_floating_point() {
    auto result = std::make_unique<tparser_floating_point>();
    for (char c : buffer_) {
      if (result->parse(c))
        throw std::logic_error(
            "This shouldn't happen, did the caller have a base != 10?");

      if (result->is_complete())
        throw std::logic_error(
            "A valid unsigned value shouldn't complete a floating point value");
    }
    return result;
  }
};

} // namespace parser
