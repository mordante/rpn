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

export module parser.detail.floating_point;

export import parser.detail.base;
import parser.detail.invalid_value;
import std;
import std.compat;

namespace parser {

/** The parser for @ref ttype::floating_point_value. */
export class tparser_floating_point final : public tparser_ {
public:
  std::unique_ptr<tparser_> parse(char c) override {
    switch (state_) {
    case tstate::initial:
      return initial(c);
    case tstate::required_number_or_dot_or_e:
      return required_number_or_dot_or_e(c);
    case tstate::required_number_or_e:
      state_ = tstate::optional_number_or_e;
      [[fallthrough]];
    case tstate::optional_number_or_e:
      return optional_number_or_e(c);
    case tstate::required_minus_or_number:
      return required_minus_or_number(c);
    case tstate::required_number:
    case tstate::optional_number:
      return number(c);
    case tstate::complete:
      throw std::logic_error("Can't parse a complete value");
    }
  }

  bool accept_minus() const noexcept override {
    return state_ == tstate::required_minus_or_number;
  }
  bool is_complete() const noexcept override {
    return state_ == tstate::complete;
  }

private:
  enum class tstate {
    initial,
    required_number_or_dot_or_e,
    required_number_or_e,
    optional_number_or_e,
    required_minus_or_number,
    required_number,
    optional_number,
    complete
  };
  tstate state_{tstate::initial};
  std::string buffer_{};

  ttoken make_complete() override {
    ttoken result = in_terminal_state()
                        ? ttoken{ttoken::ttype::floating_point_value, buffer_}
                        : ttoken{ttoken::ttype::invalid_value, std::string{}};
    state_ = tstate::complete;
    return result;
  }

  std::unique_ptr<tparser_> initial(char c) {
    if (!isdigit(c) && c != '.')
      throw std::logic_error(
          "The caller should have validated input is a digit or .");

    buffer_ += c;
    state_ = c == '.' ? tstate::required_number_or_e
                      : tstate::required_number_or_dot_or_e;
    return nullptr;
  }

  std::unique_ptr<tparser_> required_number_or_dot_or_e(char c) {
    if (c != '.')
      return optional_number_or_e(c);

    buffer_ += c;
    state_ = tstate::optional_number_or_e;
    return nullptr;
  }

  std::unique_ptr<tparser_> optional_number_or_e(char c) {
    if (handle_input_separator(c))
      return nullptr;

    buffer_ += c;
    if (c == 'e') {
      state_ = tstate::required_minus_or_number;
      return nullptr;
    }

    if (!isdigit(c))
      return std::make_unique<tparser_invalid_value>();

    return nullptr;
  }

  std::unique_ptr<tparser_> required_minus_or_number(char c) {
    buffer_ += c;
    if (c == '-') {
      state_ = tstate::required_number;
      return nullptr;
    }

    if (!isdigit(c))
      return std::make_unique<tparser_invalid_value>();

    state_ = tstate::optional_number;
    return nullptr;
  }

  std::unique_ptr<tparser_> number(char c) {
    if (state_ == tstate::required_number)
      state_ = tstate::optional_number;
    else if (is_grouping(c))
      return nullptr;

    if (handle_input_separator(c))
      return nullptr;

    if (!isdigit(c))
      return std::make_unique<tparser_invalid_value>();

    buffer_ += c;
    return nullptr;
  }

  /** Is the parser in a state it's valid to be completed? */
  bool in_terminal_state() const noexcept {
    return state_ == tstate::optional_number_or_e ||
           state_ == tstate::optional_number;
  }
};

} // namespace parser
