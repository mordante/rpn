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

export module parser.detail.base;

export import parser.token;

export import <memory>;

namespace parser {

/** Determines whether the character @p c is one of the grouping characters. */
export bool is_grouping(char c) { return c == '_' || c == ','; }

/** Determines whether the character @p c is an input seprarator. */
static bool is_input_separator(char c) { return c == ' '; }

/** The parser base class. */
export class tparser_ {
public:
  virtual ~tparser_() = default;

  /**
   * Parses the character @p c.
   *
   * During parsing the parser may detect another parser is required to parse
   * the contents of the input. When that happens the function returns a new
   * parser that processed all input including @p c of the original parser.
   *
   * Otherwise it returns a @c nullptr.
   */
  virtual std::unique_ptr<tparser_> parse(char c) = 0;

  /**
   * Does the parser in its current state accept the minus sign?
   *
   * Depending on the state of parsing a minus sign should be parsed instead of
   * execute the binary minus operation. For example @em 12e will accept a
   * minus sign in its current state.
   */
  virtual bool accept_minus() const noexcept = 0;

  /** Is the parsing complete? */
  virtual bool is_complete() const noexcept = 0;

  /**
   * Completes the parsing.
   *
   * This is used to force the parser to stop when all input is consumed.
   * Depending on the current state this may result in an invalid result. For
   * example @em 0b is an invalid value, but @em 0b1010 is a valid value.
   */
  void finish() {
    if (!is_complete())
      result_ = make_complete();
  }

  /**
   * Returns the result of the parsing.
   *
   * @pre @ref is_complete() == @c true
   * @pre @c result_.type != @c ttoken::ttype::internal_error
   */
  ttoken result() {
    if (!is_complete())
      throw std::logic_error("Can't get the result of an incomplete parser");

    if (result_.type == ttoken::ttype::internal_error)
      throw std::logic_error("The result isn't properly set");

    return result_;
  }

protected:
  /** Handles the input separator. */
  bool handle_input_separator(char c) {
    if (!is_input_separator(c))
      return false;

    result_ = make_complete();
    return true;
  }

private:
  /** The result of the parsing. */
  ttoken result_{};

  /** Complete the current object. */
  virtual ttoken make_complete() = 0;
};

} // namespace parser
