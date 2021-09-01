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

  /**a Is the parsing complete? */
  virtual bool complete() const noexcept = 0;

  /**
   * Completes the parsing.
   *
   * This is used to force the parser to stop when all input is consumed.
   * Depending on the current state this may result in an invalid result. For
   * example @em 0b is an invalid value, but @em 0b1010 is a valid value.
   */
  void finish() {
    if (!complete())
      finish_impl();
  }

  /**
   * Returns the result of the parsing.
   *
   * @pre complete() == @c true
   */
  tparsed_string result() {
    if (!complete())
      throw std::logic_error("Can't get the result of an incomlete parser");

    return result_impl();
  }

private:
  virtual tparsed_string result_impl() = 0;
  virtual void finish_impl() = 0;
};

/** The parser for @ref ttype::invalid_value. */
class tparser_invalid_value final : public tparser_ {
public:
  std::unique_ptr<tparser_> parse(char c) override {
    complete_ |= (c == ' ');
    return nullptr;
  }

  bool accept_minus() const noexcept override { return false; }
  bool complete() const noexcept override { return complete_; }

private:
  bool complete_{false};
  tparsed_string result_impl() override {
    return tparsed_string{tparsed_string::ttype::invalid_value, std::string{}};
  }

  void finish_impl() override { complete_ = true; }
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
  bool complete() const noexcept override { return state_ == tstate::complete; }

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
  tparsed_string result_{};

  void set_complete(bool valid) {
    state_ = tstate::complete;
    if (valid)
      result_ =
          tparsed_string{tparsed_string::ttype::floating_point_value, buffer_};
    else
      result_ =
          tparsed_string{tparsed_string::ttype::invalid_value, std::string{}};
  }

  std::unique_ptr<tparser_> initial(char c) {
    if (c != '.' && (c < '0' || c > '9'))
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
    if (c == ' ') {
      set_complete(true);
      return nullptr;
    }

    buffer_ += c;
    if (c == 'e') {
      state_ = tstate::required_minus_or_number;
      return nullptr;
    }

    if (c < '0' || c > '9')
      return std::make_unique<tparser_invalid_value>();

    return nullptr;
  }

  std::unique_ptr<tparser_> required_minus_or_number(char c) {
    buffer_ += c;
    if (c == '-') {
      state_ = tstate::required_number;
      return nullptr;
    }

    if (c < '0' || c > '9')
      return std::make_unique<tparser_invalid_value>();

    state_ = tstate::optional_number;
    return nullptr;
  }

  std::unique_ptr<tparser_> number(char c) {
    if (state_ == tstate::required_number)
      state_ = tstate::optional_number;
    else if (c == '_' || c == ',')
      return nullptr;

    if (c == ' ') {
      set_complete(true);
      return nullptr;
    }

    if (c < '0' || c > '9')
      return std::make_unique<tparser_invalid_value>();

    buffer_ += c;
    return nullptr;
  }

  tparsed_string result_impl() override { return result_; }

  bool in_terminal_state() const noexcept {
    return state_ == tstate::optional_number_or_e ||
           state_ == tstate::optional_number;
  }

  void finish_impl() override { set_complete(in_terminal_state()); }
};

static std::unique_ptr<tparser_> parse_floating_point(const std::string &data) {
  auto result = std::make_unique<tparser_floating_point>();
  for (char c : data) {
    if (result->parse(c))
      throw std::logic_error(
          "This shouldn't happen, did the caller have a base != 10?");

    if (result->complete())
      throw std::logic_error(
          "A valid unsigned value shouldn't complete a floating point value");
  }
  return result;
}

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
      state_ = tstate::optional_number;
      [[fallthrough]];
    case tstate::optional_number:
      return number(c);
    case tstate::complete:
      throw std::logic_error("Can't parse a complete value");
    }
  }

  bool accept_minus() const noexcept override { return false; }
  bool complete() const noexcept override { return state_ == tstate::complete; }

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
  tparsed_string result_{};

  void set_complete(bool valid) {
    state_ = tstate::complete;
    if (valid)
      result_ = tparsed_string{tparsed_string::ttype::unsigned_value, buffer_};
    else
      result_ =
          tparsed_string{tparsed_string::ttype::invalid_value, std::string{}};
  }

  std::unique_ptr<tparser_> initial(char c) {
    if (c < '0' || c > '9')
      throw std::logic_error(
          "The caller should have validated input is a digit");

    buffer_ += c;
    state_ = c == '0' ? tstate::optional_prefix : tstate::optional_number;
    return nullptr;
  }

  std::unique_ptr<tparser_> optional_prefix(char c) {
    if (c == '_' || c == ',')
      return nullptr;

    buffer_ += c;
    if (c == '.' || c == 'e')
      return parse_floating_point(buffer_);

    state_ = tstate::optional_number;
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
      break;
    }

    return nullptr;
  }

  std::unique_ptr<tparser_> number(char c) {
    if (c == '_' || c == ',')
      return nullptr;

    if (c == ' ') {
      set_complete(true);
      return nullptr;
    }

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
        return parse_floating_point(buffer_);
      if (c > '9')
        return std::make_unique<tparser_invalid_value>();
      break;
    case tbase::hexadecimal:
      if (c > '9' && c != 'a' && c != 'b' && c != 'c' && c != 'd' && c != 'e' &&
          c != 'f')
        return std::make_unique<tparser_invalid_value>();
      break;
    }

    // Expects ASCII
    if (c < '0')
      return std::make_unique<tparser_invalid_value>();

    return nullptr;
  }
  tparsed_string result_impl() override { return result_; }

  void finish_impl() override {
    set_complete(state_ == tstate::optional_prefix ||
                 state_ == tstate::optional_number);
  }
};

/** The parser for @ref ttype::string_value. */
class tparser_string final : public tparser_ {
public:
  std::unique_ptr<tparser_> parse(char c) override {
    if (complete_)
      throw std::logic_error("Can't parse a complete value");

    if (c == ' ')
      set_complete();
    else
      buffer_ += c;

    return nullptr;
  }

  bool accept_minus() const noexcept override { return false; }
  bool complete() const noexcept override { return complete_; }

private:
  bool complete_{false};
  std::string buffer_{};
  tparsed_string result_{};

  void set_complete() {
    complete_ = true;
    result_ = tparsed_string{tparsed_string::ttype::string_value, buffer_};
  }

  tparsed_string result_impl() override { return result_; }

  void finish_impl() override { set_complete(); }
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
      default:
        if (c >= '0' && c <= '9')
          parser_ = std::make_unique<tparser_unsigned>();
        else
          parser_ = std::make_unique<tparser_string>();
        break;
      }
    }
    std::unique_ptr<tparser_> replacement = parser_->parse(c);
    if (replacement) {
      parser_ = std::move(replacement);
    }
    if (parser_->complete())
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
