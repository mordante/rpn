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

// TODO Rename siged_value to signed when that becomes valid.
export module parser.detail.signed_value;

export import parser.detail.base;
import parser.detail.invalid_value;
import parser.detail.string;
import std;
import std.compat;

namespace parser {

/**
 * The parser for @ref ttype::signed_value.
 *
 * @note Due to the narrow use-case of this value there's no support for
 * different bases.
 *
 * @todo After accepting a minus undoing this is hard and leads to a somewhat
 * funcy state. Need to think about another way to undo things.
 */
export class tparser_signed final : public tparser_ {
public:
  std::unique_ptr<tparser_> parse(char c) override {
    switch (state_) {
    case tstate::initial:
      return initial(c);
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
    required_minus_or_number,
    required_number,
    optional_number,
    complete
  };
  tstate state_{tstate::initial};
  bool has_minus_{false};
  std::string buffer_{};

  ttoken make_complete() override {
    ttoken result =
        in_terminal_state() ? ttoken{ttoken::ttype::signed_value, buffer_}
        : has_minus_ ? ttoken{ttoken::ttype::invalid_value, std::string{}}
                     : ttoken{ttoken::ttype::string_value, "i" + buffer_};
    state_ = tstate::complete;
    return result;
  }

  std::unique_ptr<tparser_> initial(char c) {
    if (c != 'i')
      throw std::logic_error(
          "The caller should have validated input is an 'i'");

    state_ = tstate::required_minus_or_number;
    return nullptr;
  }

  std::unique_ptr<tparser_> required_minus_or_number(char c) {
    if (handle_input_separator(c))
      return nullptr;

    buffer_ += c;
    if (c == '-') {
      has_minus_ = true;
      state_ = tstate::required_number;
      return nullptr;
    }

    if (!isdigit(c))
      return create_parser_string();

    state_ = tstate::optional_number;
    return nullptr;
  }

  std::unique_ptr<tparser_> number(char c) {
    if (handle_input_separator(c))
      return nullptr;

    if (state_ == tstate::required_number)
      state_ = tstate::optional_number;
    else if (is_grouping(c))
      return nullptr;

    buffer_ += c;
    if (!isdigit(c)) {
      if (has_minus_)
        return std::make_unique<tparser_invalid_value>();
      return create_parser_string();
    }

    return nullptr;
  }

  /** Is the parser in a state it's valid to be completed? */
  bool in_terminal_state() const noexcept {
    return state_ == tstate::optional_number;
  }

  std::unique_ptr<tparser_> create_parser_string() {
    auto result = std::make_unique<tparser_string>();
    for (char c : "i" + buffer_) {
      if (result->parse(c))
        throw std::logic_error("This shouldn't happen");

      if (result->is_complete())
        throw std::logic_error(
            "A valid signed value shouldn't complete a string value");
    }
    return result;
  }
};

} // namespace parser
