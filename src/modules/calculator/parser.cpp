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

export module calculator.parser;

import calculator.stack;

export import<string>;
export import<string_view>;
import<vector>;

namespace calculator {

/** Determines whether the character @p c is one of the grouping characters. */
static bool is_grouping(char c) { return c == '_' || c == ','; }

/** Determines whether the character @p c is an input seprarator. */
static bool is_input_separator(char c) { return c == ' '; }

/** The struct containing the results of the parsing. */
export struct tparsed_string {
  enum class ttype {
    /** Returning this state is a bug in the library. */
    internal_error,
    /**
     * A numeric value was detected but contained invalid characters.
     *
     * Invalid characters can be digits invalid for the selected base or other
     * invalid characters.
     */
    invalid_value,
    /**
     * The @ref string contains an signed intgeral value.
     *
     * The value may fail to parse when it's larger than the storage type.
     */
    signed_value,
    /**
     * The @ref string contains an unsigned intgeral value.
     *
     * The value may fail to parse when it's larger than the storage type.
     */
    unsigned_value,
    /**
     * The @ref string contains a floating point value.
     *
     * The value may fail to parse when it's larger than the storage type.
     */
    floating_point_value,
    /**
     * The @ref string contains a string value.
     *
     * @note That at the moment strings are handle the same as @ref
     * invalid_value. However it's intended to add support for strings in the
     * engine, not as value but for operations and constants.
     */
    string_value
  };
  ttype type{ttype::internal_error};

  /** When a valid value is parsed it contains the parsed value. */
  std::string string{};

  bool operator==(const tparsed_string &) const = default;
};

/** The parser base class. */
class tparser_ {
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
   * @pre @c result_.type != @c tparsed_string::ttype::internal_error
   */
  tparsed_string result() {
    if (!is_complete())
      throw std::logic_error("Can't get the result of an incomplete parser");

    if (result_.type == tparsed_string::ttype::internal_error)
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
  tparsed_string result_{};

  /** Complete the current object. */
  virtual tparsed_string make_complete() = 0;
};

/** The parser for @ref ttype::invalid_value. */
class tparser_invalid_value final : public tparser_ {
public:
  std::unique_ptr<tparser_> parse(char) override { return nullptr; }

  bool accept_minus() const noexcept override { return false; }
  bool is_complete() const noexcept override { return complete_; }

private:
  bool complete_{false};

  tparsed_string make_complete() override {
    complete_ = true;
    return tparsed_string{tparsed_string::ttype::invalid_value, std::string{}};
  }
};

/** The parser for @ref ttype::floating_point_value. */
class tparser_floating_point final : public tparser_ {
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

  tparsed_string make_complete() override {
    tparsed_string result =
        in_terminal_state()
            ? tparsed_string{tparsed_string::ttype::floating_point_value,
                             buffer_}
            : tparsed_string{tparsed_string::ttype::invalid_value,
                             std::string{}};
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

/** The parser for @ref ttype::unsigned_value. */
class tparser_unsigned final : public tparser_ {
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
  tbase base_{tbase::decimal};
  std::string buffer_{};

  tparsed_string make_complete() override {
    tparsed_string result =
        in_terminal_state()
            ? tparsed_string{tparsed_string::ttype::unsigned_value, buffer_}
            : tparsed_string{tparsed_string::ttype::invalid_value,
                             std::string{}};
    state_ = tstate::complete;
    return result;
  }

  std::unique_ptr<tparser_> initial(char c) {
    if (!isdigit(c))
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
      base_ = tbase::binary;
      state_ = tstate::required_number;
      break;

    case 'x':
      base_ = tbase::hexadecimal;
      state_ = tstate::required_number;
      break;

    default:
      // The string has an octal 0 prefix, 8 and 9 are invalid octal digits.
      if (c < '0' || c > '7')
        return std::make_unique<tparser_invalid_value>();

      base_ = tbase::octal;
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
    case tbase::binary:
      if (c > '1')
        return std::make_unique<tparser_invalid_value>();
      break;

    case tbase::octal:
      if (c > '7')
        return std::make_unique<tparser_invalid_value>();
      break;
    case tbase::decimal:
      // Non-decimal bases aren't allowed in floating-point values.
      if (c == '.' || c == 'e')
        return create_parser_floating_point();
      if (c > '9')
        return std::make_unique<tparser_invalid_value>();
      break;
    case tbase::hexadecimal:
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

/** The parser for @ref ttype::string_value. */
class tparser_string final : public tparser_ {
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

  tparsed_string make_complete() override {
    complete_ = true;
    return tparsed_string{tparsed_string::ttype::string_value, buffer_};
  }
};

/**
 * The parser for @ref ttype::signed_value.
 *
 * @note Due to the narrow use-case of this value there's no support for
 * different bases.
 *
 * @todo After accepting a minus undoing this is hard and leads to a somewhat
 * funcy state. Need to think about another way to undo things.
 */
class tparser_signed final : public tparser_ {
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

  tparsed_string make_complete() override {
    tparsed_string result =
        in_terminal_state()
            ? tparsed_string{tparsed_string::ttype::signed_value, buffer_}
        : has_minus_ ? tparsed_string{tparsed_string::ttype::invalid_value,
                                      std::string{}}
                     : tparsed_string{tparsed_string::ttype::string_value,
                                      "i" + buffer_};
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

export struct tparser {
public:
  void append(char data) { parse(data); }
  void append(std::string_view data) {
    std::for_each(data.begin(), data.end(), [this](char c) { parse(c); });
  }

  bool accept_minus() const {
    return parser_ ? parser_->accept_minus() : false;
  }

  void reset() {
    parser_.reset();
    result_.clear();
  }

  const std::vector<tparsed_string> &process() {
    finish();

    return result_;
  }

private:
  std::unique_ptr<tparser_> parser_;

  std::vector<tparsed_string> result_;

  void add_result() {
    result_.push_back(parser_->result());
    parser_.reset();
  }

  void parse(char c) {
    if (!parser_) [[unlikely]] {
      switch (c) {
      case '.':
        parser_ = std::make_unique<tparser_floating_point>();
        break;
      case 'i':
        parser_ = std::make_unique<tparser_signed>();
        break;
      default:
        if (isdigit(c))
          parser_ = std::make_unique<tparser_unsigned>();
        else
          parser_ = std::make_unique<tparser_string>();
        break;
      }
    }
    std::unique_ptr<tparser_> replacement = parser_->parse(c);
    if (replacement)
      parser_.reset(replacement.release());

    if (parser_->is_complete())
      add_result();
  }

  void finish() {
    if (!parser_)
      return;

    parser_->finish();
    add_result();
  }
};

} // namespace calculator
