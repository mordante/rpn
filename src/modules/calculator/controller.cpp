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

export module calculator.controller;

import calculator.model;
import calculator.transaction;
import calculator.undo_handler;

import<charconv>;
import<format>;
import<string>;
import<string_view>;

namespace calculator {

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
  using tunary_operation = void (tvalue::*)();
  using tbinary_operation = void (tvalue::*)(const tvalue &);

  /**
   * Calculates the unary operation on a value.
   *
   * When the input isn't empty equivalent @em op @a input
   * else equivalent @em op @c pop()
   *
   * Upon success the diagnostics are cleared, else passes the exception thrown
   * to its parent.
   */
  void math_unary_operation(tunary_operation operation);

  /**
   * Calculates the binary operation on two values.
   *
   * When the input isn't empty equivalent @c pop() @em op @a input
   * else equivalent @c pop() @em op @c pop()
   *
   * Upon success the diagnostics are cleared, else passes the exception thrown
   * to its parent.
   */
  void math_binary_operation(tbinary_operation operation);

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
    math_binary_operation(&tvalue::add);
    break;

  case '-':
    if (model_.input_accept_minus())
      model_.input_append(key);
    else
      math_binary_operation(&tvalue::sub);
    break;

  case '*':
    math_binary_operation(&tvalue::mul);
    break;

  case '/':
    math_binary_operation(&tvalue::div);
    break;

    /*** Bitwise operations ***/
  case '&':
    math_binary_operation(&tvalue::bit_and);
    break;

  case '|':
    math_binary_operation(&tvalue::bit_or);
    break;

  case '^':
    math_binary_operation(&tvalue::bit_xor);
    break;

  case '~':
    math_unary_operation(&tvalue::complement);
    break;

    /*** Bitwise shifts ***/
  case '<':
    math_binary_operation(&tvalue::shl);
    break;

  case '>':
    math_binary_operation(&tvalue::shr);
    break;

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

static void parse(ttransaction &transaction, std::string_view input) {
  int base = determine_base(input);
  uint64_t value;
  std::from_chars_result result =
      std::from_chars(input.begin(), input.end(), value, base);

  validate(result.ec);
  if (result.ptr != input.end())
    throw std::domain_error("Invalid numeric value");

  transaction.push(tvalue{value});
}

// TODO This function can be removed since it now only forwards the arguments.
static void push(ttransaction &transaction, std::string_view input) {
  if (input.empty())
    throw std::logic_error(
        "The parser should only be called with a non-empty string");

  parse(transaction, input);
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

void tcontroller::handle_keyboard_input_control(char key) {
  switch (key) {
    /*** Modify selected base ***/
  case 'b':
    model_.base_set(tbase::binary);
    break;

  case 'o':
    model_.base_set(tbase::octal);
    break;

  case 'd':
    model_.base_set(tbase::decimal);
    break;

  case 'h':
    model_.base_set(tbase::hexadecimal);
    break;

    /*** Undo / redo ***/
  case 'z':
    undo_handler_.undo();
    break;

  case 'Z':
    undo_handler_.redo();
    break;

    /*** Miscellaneous ***/
  case 'n':
    math_unary_operation(&tvalue::negate);
    break;
  }
}

void tcontroller::append(std::string_view data) noexcept {
  try {
    model_.input_append(data);
  } catch (const std::exception &e) {
    diagnostics_set(e);
  }
}

static void parse(ttransaction &transaction, const tparsed_string &input) {
  switch (input.type) {
  case tparsed_string::ttype::internal_error:
    throw std::logic_error("Invalid parsed string");

  case tparsed_string::ttype::invalid_value:
    throw std::domain_error("Invalid numeric value");

  case tparsed_string::ttype::unsigned_value:
    push(transaction, input.string);
    break;

  case tparsed_string::ttype::floating_point_value:
    parse_float(transaction, input.string);
    break;

  case tparsed_string::ttype::string_value:
    throw std::domain_error("Invalid numeric value");
  }
}

static void parse(ttransaction &transaction,
                  const std::vector<tparsed_string> &input) {
  std::for_each(
      input.begin(), input.end(),
      [&transaction](const tparsed_string &i) { parse(transaction, i); });
}

void tcontroller::math_binary_operation(tbinary_operation operation) {
  ttransaction transaction(model_);
  parse(transaction, model_.input_process());
  transaction.input_reset();

  if (model_.stack().size() < 2)
    throw std::out_of_range("Stack doesn't contain two elements");

  const tvalue rhs = transaction.pop();
  tvalue lhs = transaction.pop();
  (lhs.*operation)(rhs);
  transaction.push(lhs);
  model_.diagnostics_clear();
  undo_handler_.add(std::move(transaction).release());
}

void tcontroller::math_unary_operation(tunary_operation operation) {
  ttransaction transaction(model_);
  parse(transaction, model_.input_process());
  transaction.input_reset();

  if (model_.stack().empty()) {
    throw std::out_of_range("Stack doesn't contain an element");
  }

  tvalue value = transaction.pop();
  (value.*operation)();
  transaction.push(value);
  model_.diagnostics_clear();
  undo_handler_.add(std::move(transaction).release());
}

void tcontroller::diagnostics_set(const std::exception &e) {
#if defined(__cpp_lib_format)
  model_.diagnostics_set(std::format("[ERR]{:>80.79}", e.what()));
#else
  model_.diagnostics_set(e.what());
#endif
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
