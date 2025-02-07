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

#include <cerrno>

export module calculator.controller;

import calculator.math.arithmetic;
import calculator.math.bitwise;
import calculator.math.core;
import calculator.math.logarithm;
import calculator.math.round;
import calculator.model;
import calculator.transaction;
import calculator.undo_handler;
import lib.base;
import lib.dictionary;
import std;
import std.compat;

namespace calculator {

/** Functor for a nullary operation. */
template <class F>
concept nullary_operation =
    std::same_as<std::invoke_result_t<F, ttransaction>, void>;

/** Functor for an unary math operation. */
template <class F>
concept unary_operation =
    (std::same_as<std::invoke_result_t<F, math::tstorage>, math::tstorage>);

/** Functor for a binary math operation. */
template <class F>
concept binary_operation =
    (std::same_as<std::invoke_result_t<F, math::tstorage, math::tstorage>,
                  math::tstorage>);

/**
 * The pressed keyboard modifiers.
 *
 * @note Alt will be added when there's a use-case.
 * @note The shift won't be processed since it gives a different character.
 */
export enum class tmodifiers { none = 0, control = 1 };

/**
 * The special keys on the keyboard.
 *
 * Most keys are processed using their ASCII value. This makes it easier to
 * handle the two ways to enter a + character on the keyboard.
 *
 * @note This set only contains the keys the controller is interested in. For
 * example:
 * - there's no control key, that is already part of @ref tmodifier,
 * - there's only on enter, since both enters on the keyboard are treated the
 *   same.
 * This means adding functionality may add new keys.
 */
export enum class tkey {
  /* Sorted list */
  backspace,
  enter
};

/**
 * The controller of the calculator.
 *
 * For now a very basic version of this class.
 *
 * The class is non-copyable and non-movable. These operations make little
 * sense for the model and are most likely logic errors.
 */
export class tcontroller final {
public:
  // *** SMF ***

  explicit tcontroller(tmodel &model) noexcept : model_(model) {}
  tcontroller(const tcontroller &) = delete;
  tcontroller(tcontroller &&) = delete;
  ~tcontroller() = default;

  tcontroller &operator=(const tcontroller &) = delete;
  tcontroller &operator=(tcontroller &&) = delete;

  // *** Operations ***

  /** Handles the keyboard input for special keys. */
  void handle_keyboard_input(tkey key) noexcept;

  /** Handles the keyboard input for normal keys. */
  void handle_keyboard_input(tmodifiers modifiers, char key) noexcept;

  /**
   * Appends data to the input.
   *
   * The function doesn't modify the diagnostics.
   */
  void append(std::string_view data) noexcept;

private:
  /**
   * Calculates the unary operation on a value.
   *
   * When the input isn't empty equivalent @em op @a input
   * else equivalent @em op @c pop()
   *
   * Upon success the diagnostics are cleared, else passes the exception thrown
   * to its parent.
   */
  void math_unary_operation(unary_operation auto operation);

  /**
   * Calculates the binary operation on two values.
   *
   * When the input isn't empty equivalent @c pop() @em op @a input
   * else equivalent @c pop() @em op @c pop()
   *
   * Upon success the diagnostics are cleared, else passes the exception thrown
   * to its parent.
   */
  void math_binary_operation(binary_operation auto operation);

  void diagnostics_set(const std::exception &e);

  /**
   * Pushes the current input to the stack.
   *
   * Upon success the diagnostics are cleared, else passes the exception thrown
   * to its parent.
   */
  void push();

  /** Implementation of the keyboard per modifier. */
  void handle_keyboard_input_no_modifiers(char key);
  void handle_keyboard_input_control(char key);

  /**
   * Removes an element.
   *
   * If the input isn't empty removes the last element from the buffer.
   * Else drops an item of the stack.
   */
  void remove();

  tmodel &model_;

  tundo_handler undo_handler_;
};

void tcontroller::handle_keyboard_input(tkey key) noexcept {
  try {
    switch (key) {
    case tkey::backspace:
      remove();
      break;

    case tkey::enter:
      push();
      break;
    }
  } catch (const std::exception &e) {
    diagnostics_set(e);
  }
}

void tcontroller::handle_keyboard_input(tmodifiers modifiers,
                                        char key) noexcept {
  try {
    switch (modifiers) {
    case tmodifiers::none:
      handle_keyboard_input_no_modifiers(key);
      break;

    case tmodifiers::control:
      handle_keyboard_input_control(key);
      break;
    }
  } catch (const std::exception &e) {
    diagnostics_set(e);
  }
}

void tcontroller::handle_keyboard_input_no_modifiers(char key) {
  switch (key) {
    /*** Basic arithmetic operations ***/
  case '+':
    return math_binary_operation(&math::add);

  case '-':
    if (model_.input_accept_minus())
      model_.input_append(key);
    else
      math_binary_operation(&math::sub);
    break;

  case '*':
    return math_binary_operation(&math::mul);

  case '/':
    return math_binary_operation(&math::div);

  case '%':
    return math_binary_operation(&math::mod);

  case '\\':
    return math_binary_operation(&math::quotient);

    /*** Bitwise operations ***/
  case '&':
    return math_binary_operation(&math::bit_and);

  case '|':
    return math_binary_operation(&math::bit_or);

  case '^':
    return math_binary_operation(&math::bit_xor);

  case '~':
    return math_unary_operation(&math::complement);

    /*** Bitwise shifts ***/
  case '<':
    return math_binary_operation(&math::shl);

  case '>':
    return math_binary_operation(&math::shr);

    /*** Others ***/
  default:
    model_.input_append(key);
  }
}

static void validate(std::errc ec) {
  if (ec == std::errc())
    return;

  switch (ec) {
  case std::errc::invalid_argument:
    throw std::domain_error("Invalid numeric value");

  case std::errc::result_out_of_range:
    throw std::out_of_range("Value outside of the representable range");

  default:
    // This happens when the implementation behaves outside the specifications.
    throw std::domain_error("Unexpected error");
  }
}

static int determine_base(std::string_view &input) {
  if (input.size() < 2 || input[0] != '0')
    return 10;

  switch (input[1]) {
  case 'b':
    input.remove_prefix(2);
    return 2;
  default:
    input.remove_prefix(1);
    return 8;
  case 'x':
    input.remove_prefix(2);
    return 16;
  }
}

static void parse_signed(ttransaction &transaction, std::string_view input) {
  int64_t value;
  std::from_chars_result result =
      std::from_chars(input.begin(), input.end(), value);

  validate(result.ec);
  if (result.ptr != input.end())
    throw std::domain_error("Invalid numeric value");

  transaction.push(tvalue{value});
}

static void parse_unsigned(ttransaction &transaction, std::string_view input) {
  int base = determine_base(input);
  uint64_t value;
  std::from_chars_result result =
      std::from_chars(input.begin(), input.end(), value, base);

  validate(result.ec);
  if (result.ptr != input.end())
    throw std::domain_error("Invalid numeric value");

  transaction.push(tvalue{value});
}

static void parse_float(ttransaction &transaction, std::string_view input) {
  // TODO Use std::from_chars once it becomes available.
  std::string str{input};
  const char *s = str.c_str();
  char *ptr = nullptr;
  double value = strtod(s, &ptr);

  if (!ptr || *ptr != '\0' || errno == ERANGE)
    throw std::domain_error("Invalid numeric value");

  transaction.push(tvalue{value});
}

static std::optional<tvalue> get_constant(std::string_view input) {
  static constexpr std::array constants = lib::make_dictionary(
      /*** Signed int minimum ***/
      "int8_min", tvalue{int64_t(std::numeric_limits<int8_t>::min())},   //
      "int16_min", tvalue{int64_t(std::numeric_limits<int16_t>::min())}, //
      "int32_min", tvalue{int64_t(std::numeric_limits<int32_t>::min())}, //
      "int64_min", tvalue{int64_t(std::numeric_limits<int64_t>::min())}, //
      /*** Signed int maximum ***/
      "int8_max", tvalue{int64_t(std::numeric_limits<int8_t>::max())},   //
      "int16_max", tvalue{int64_t(std::numeric_limits<int16_t>::max())}, //
      "int32_max", tvalue{int64_t(std::numeric_limits<int32_t>::max())}, //
      "int64_max", tvalue{int64_t(std::numeric_limits<int64_t>::max())}, //
      /*** Unsigned int maximum ***/
      "uint8_max", tvalue{uint64_t(std::numeric_limits<uint8_t>::max())},   //
      "uint16_max", tvalue{uint64_t(std::numeric_limits<uint16_t>::max())}, //
      "uint32_max", tvalue{uint64_t(std::numeric_limits<uint32_t>::max())}, //
      "uint64_max", tvalue{uint64_t(std::numeric_limits<uint64_t>::max())}, //
      /*** Floating point minimum ***/
      "float_min", tvalue{double(std::numeric_limits<float>::min())},   //
      "double_min", tvalue{double(std::numeric_limits<double>::min())}, //
      /*** Floating point maximum ***/
      "float_max", tvalue{double(std::numeric_limits<float>::max())},   //
      "double_max", tvalue{double(std::numeric_limits<double>::max())}, //
      /** Double constants ***/
      "pi", tvalue{std::numbers::pi}, //
      "e", tvalue{std::numbers::e}    //
  );
  if (auto iter = lib::find(constants, input); iter != constants.end())
    return iter->second;

  return {};
}

static void exectute_operation(ttransaction &transaction,
                               nullary_operation auto operation) {
  std::invoke(operation, transaction);
}

static void exectute_operation(ttransaction &transaction,
                               unary_operation auto operation) {
  auto [value] = transaction.pop();
  value = std::invoke(operation, value);
  transaction.push(value);
}

static void exectute_operation(ttransaction &transaction,
                               binary_operation auto operation) {
  auto [rhs, lhs] = transaction.pop<2>();
  transaction.push(std::invoke(operation, lhs, rhs));
}

static void execute_command(ttransaction &transaction, std::string_view input) {
  /*** Nullary ***/
  static constexpr std::array nullary_commands =
      lib::make_dictionary("debug", &ttransaction::debug_mode_toggle);

  if (auto iter = lib::find(nullary_commands, input);
      iter != nullary_commands.end())
    return exectute_operation(transaction, iter->second);

  /*** Unary ***/
  static constexpr std::array unary_commands = lib::make_dictionary(
      /*** Logarithm ***/
      "lg", &math::lg,   //
      "ln", &math::ln,   //
      "log", &math::log, //
      /*** Rounding ***/
      "round", &math::round, //
      "floor", &math::floor, //
      "ceil", &math::ceil,   //
      "trunc", &math::trunc);

  if (auto iter = lib::find(unary_commands, input);
      iter != unary_commands.end())
    return exectute_operation(transaction, iter->second);

  /*** Binary ***/
  static constexpr std::array binary_commands = lib::make_dictionary(
      /*** Powers ***/
      "pow", static_cast<math::tstorage (*)(math::tstorage, math::tstorage)>(
                 math::pow) // cast needed to specify non-templated function.
  );

  if (auto iter = lib::find(binary_commands, input);
      iter != binary_commands.end())
    return exectute_operation(transaction, iter->second);

  /*** Error ***/
  throw std::domain_error("Invalid numeric value or command");
}

static void parse_string(ttransaction &transaction, std::string_view input) {
  const std::optional<tvalue> constant = get_constant(input);
  if (constant)
    transaction.push(*constant);
  else
    execute_command(transaction, input);
}

void tcontroller::handle_keyboard_input_control(char key) {
  switch (key) {
    /*** Modify selected base ***/
  case 'b':
    model_.base_set(lib::tbase::binary);
    break;

  case 'o':
    model_.base_set(lib::tbase::octal);
    break;

  case 'd':
    model_.base_set(lib::tbase::decimal);
    break;

  case 'h':
    model_.base_set(lib::tbase::hexadecimal);
    break;

    /*** Undo / redo ***/
  case 'z':
    undo_handler_.undo();
    break;

  case 'Z':
    undo_handler_.redo();
    break;

    /*** Miscellaneous ***/
  case 'g':
    model_.grouping_toggle();
    break;

  case 'n':
    return math_unary_operation(&math::negate);

    /*** Powers ***/
  case '2':
    return math_unary_operation(&math::pow<2>);
  case '3':
    return math_unary_operation(&math::pow<3>);
  case '4':
    return math_unary_operation(&math::pow<4>);
  case '5':
    return math_unary_operation(&math::pow<5>);
  case '6':
    return math_unary_operation(&math::pow<6>);
  case '7':
    return math_unary_operation(&math::pow<7>);
  case '8':
    return math_unary_operation(&math::pow<8>);
  case '9':
    return math_unary_operation(&math::pow<9>);
  }
}

void tcontroller::append(std::string_view data) noexcept {
  try {
    model_.input_append(data);
  } catch (const std::exception &e) {
    diagnostics_set(e);
  }
}

static void parse(ttransaction &transaction, const parser::ttoken &input) {
  switch (input.type) {
  case parser::ttoken::ttype::internal_error:
    throw std::logic_error("Invalid parsed string");

  case parser::ttoken::ttype::invalid_value:
    throw std::domain_error("Invalid numeric value");

  case parser::ttoken::ttype::signed_value:
    parse_signed(transaction, input.string);
    break;

  case parser::ttoken::ttype::unsigned_value:
    parse_unsigned(transaction, input.string);
    break;

  case parser::ttoken::ttype::floating_point_value:
    parse_float(transaction, input.string);
    break;

  case parser::ttoken::ttype::string_value:
    parse_string(transaction, input.string);
    break;
  }
}

static void parse(ttransaction &transaction,
                  const std::vector<parser::ttoken> &input) {
  std::ranges::for_each(input, [&transaction](const parser::ttoken &i) {
    parse(transaction, i);
  });
}

void tcontroller::math_unary_operation(unary_operation auto operation) {
  ttransaction transaction(model_);
  parse(transaction, model_.input_process());
  transaction.input_reset();

  exectute_operation(transaction, operation);

  model_.diagnostics_clear();
  undo_handler_.add(std::move(transaction).release());
}

void tcontroller::math_binary_operation(binary_operation auto operation) {
  ttransaction transaction(model_);
  parse(transaction, model_.input_process());
  transaction.input_reset();

  auto [rhs, lhs] = transaction.pop<2>();
  transaction.push(std::invoke(operation, lhs, rhs));

  model_.diagnostics_clear();
  undo_handler_.add(std::move(transaction).release());
}

void tcontroller::diagnostics_set(const std::exception &e) {
  model_.diagnostics_set(std::format("{:7} {:>50.50}", "[ERR]", e.what()));
}

void tcontroller::push() {
  ttransaction transaction(model_);
  if (model_.input_get().empty())
    transaction.duplicate();
  else {
    parse(transaction, model_.input_process());
    transaction.input_reset();
  }

  model_.diagnostics_clear();
  undo_handler_.add(std::move(transaction).release());
}

void tcontroller::remove() {
  if (model_.input_pop_back())
    return;

  ttransaction transaction(model_);
  transaction.drop();
  model_.diagnostics_clear();
  undo_handler_.add(std::move(transaction).release());
}

} // namespace calculator
