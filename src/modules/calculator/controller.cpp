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
import math;

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
  void push(std::string_view input);

  void duplicate_last_entry();
  void parse(std::string_view input);

  using tunary_operation = int64_t (*)(int64_t) noexcept;
  using tbinary_operation = int64_t (*)(int64_t, int64_t) noexcept;

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

  tmodel &model_;
};

void tcontroller::handle_keyboard_input(tkey key) noexcept {
  try {
    switch (key) {
    case tkey::enter:
      push();
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
    math_binary_operation(math::add);
    break;

  case '-':
    math_binary_operation(math::sub);
    break;

  case '*':
    math_binary_operation(math::mul);
    break;

  case '/':
    math_binary_operation(math::div);
    break;

    /*** Bitwise operations ***/
  case '&':
    math_binary_operation(math::bit_and);
    break;

  case '|':
    math_binary_operation(math::bit_or);
    break;

  case '^':
    math_binary_operation(math::bit_xor);
    break;

  case '~':
    math_unary_operation(math::bit_complement);
    break;

    /*** Bitwise shifts ***/
  case '<':
    math_binary_operation(math::shl);
    break;

  case '>':
    math_binary_operation(math::shr);
    break;

    /*** Others ***/
  default:
    model_.input_append(key);
  }
}

void tcontroller::handle_keyboard_input_control(char key) { (void)key; }

void tcontroller::append(std::string_view data) noexcept {
  try {
    model_.input_append(data);
  } catch (const std::exception &e) {
    diagnostics_set(e);
  }
}

void tcontroller::math_binary_operation(tbinary_operation operation) {
  if (const std::string input = model_.input_steal(); !input.empty())
    push(input);

  if (model_.stack_size() < 2)
    throw std::out_of_range("Stack doesn't contain two elements");

  const tvalue rhs = model_.stack_pop();
  const tvalue lhs = model_.stack_pop();
  model_.stack_push(operation(lhs.get(), rhs.get()));
  model_.diagnostics_clear();
}

void tcontroller::math_unary_operation(tunary_operation operation) {
  if (const std::string input = model_.input_steal(); !input.empty())
    push(input);

  if (model_.stack_empty())
    throw std::out_of_range("Stack doesn't contain an element");

  const tvalue value = model_.stack_pop();
  model_.stack_push(operation(value.get()));
  model_.diagnostics_clear();
}

void tcontroller::push(std::string_view input) {
  if (input.empty())
    duplicate_last_entry();
  else
    parse(input);
}

void tcontroller::duplicate_last_entry() {
  tvalue value = model_.stack_pop();
  model_.stack_push(value);
  model_.stack_push(value);
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

int determine_base(std::string_view &input) {
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

void tcontroller::parse(std::string_view input) {
  int base = determine_base(input);
  int64_t value;
  std::from_chars_result result =
      std::from_chars(input.begin(), input.end(), value, base);

  validate(result.ec);
  if (result.ptr != input.end())
    throw std::domain_error("Invalid numeric value");

  model_.stack_push(value);
}

void tcontroller::diagnostics_set(const std::exception &e) {
#if defined(__cpp_lib_format)
  model_.diagnostics_set(std::format("[ERR]{:>80.79}", e.what()));
#else
  model_.diagnostics_set(e.what());
#endif
}

void tcontroller::push() {
  push(model_.input_steal());
  model_.diagnostics_clear();
}

} // namespace calculator
