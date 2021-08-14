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

  /**
   * Appends data to the input.
   *
   * The function doesn't modify the diagnostics.
   */
  void append(std::string_view data) noexcept;

  /**
   * Pushes the current input to the stack.
   *
   * Upon success the diagnostics are cleared, else they contain the last error.
   */
  void push() noexcept;

  /** Calculates @ref math_binary_operation addition. */
  void math_add() noexcept;

  /** Calculates @ref math_binary_operation subraction. */
  void math_sub() noexcept;

  /** Calculates @ref math_binary_operation multiplication. */
  void math_mul() noexcept;

  /**
   * Calculates @ref math_binary_operation division.
   *
   * @note The result is always an integer value instead of a floating-point
   * value.
   *
   * @todo Guard against division by zero.
   */
  void math_div() noexcept;

  /** Calculates @ref math_binary_operation bitwise and. */
  void math_and() noexcept;

  /** Calculates @ref math_binary_operation bitwise or. */
  void math_or() noexcept;

  /** Calculates @ref math_binary_operation bitwise xor. */
  void math_xor() noexcept;

  /** Calculates @ref math_unary_operation bitwise complement. */
  void math_complement() noexcept;

  /** Calculates @ref math_binary_operation bitwise shl. */
  void math_shl() noexcept;

  /** Calculates @ref math_binary_operation bitwise shr. */
  void math_shr() noexcept;

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
   * Upon success the diagnostics are cleared, else they contain the last error.
   */
  void math_unary_operation(tunary_operation operation) noexcept;

  /**
   * Calculates the binary operation on two values.
   *
   * When the input isn't empty equivalent @c pop() @em op @a input
   * else equivalent @c pop() @em op @c pop()
   *
   * Upon success the diagnostics are cleared, else they contain the last error.
   */
  void math_binary_operation(tbinary_operation operation) noexcept;

  void diagnostics_set(const std::exception &e);
  tmodel &model_;
};

void tcontroller::append(std::string_view data) noexcept {
  try {
    model_.input_append(data);
  } catch (const std::exception &e) {
    diagnostics_set(e);
  }
}

void tcontroller::push() noexcept {
  try {
    push(model_.input_steal());
    model_.diagnostics_clear();
  } catch (const std::exception &e) {
    diagnostics_set(e);
  }
}

void tcontroller::math_binary_operation(tbinary_operation operation) noexcept {
  try {
    if (const std::string input = model_.input_steal(); !input.empty())
      push(input);

    if (model_.stack_size() < 2)
      throw std::out_of_range("Stack doesn't contain two elements");

    const tvalue rhs = model_.stack_pop();
    const tvalue lhs = model_.stack_pop();
    model_.stack_push(operation(lhs.get(), rhs.get()));
    model_.diagnostics_clear();
  } catch (const std::exception &e) {
    diagnostics_set(e);
  }
}

void tcontroller::math_unary_operation(tunary_operation operation) noexcept {
  try {
    if (const std::string input = model_.input_steal(); !input.empty())
      push(input);

    if (model_.stack_empty())
      throw std::out_of_range("Stack doesn't contain an element");

    const tvalue value = model_.stack_pop();
    model_.stack_push(operation(value.get()));
    model_.diagnostics_clear();
  } catch (const std::exception &e) {
    diagnostics_set(e);
  }
}

void tcontroller::math_add() noexcept { math_binary_operation(math::add); }
void tcontroller::math_sub() noexcept { math_binary_operation(math::sub); }
void tcontroller::math_mul() noexcept { math_binary_operation(math::mul); }
void tcontroller::math_div() noexcept { math_binary_operation(math::div); }

void tcontroller::math_and() noexcept { math_binary_operation(math::bit_and); }
void tcontroller::math_or() noexcept { math_binary_operation(math::bit_or); }
void tcontroller::math_xor() noexcept { math_binary_operation(math::bit_xor); }
void tcontroller::math_complement() noexcept {
  math_unary_operation(math::bit_complement);
}
void tcontroller::math_shl() noexcept { math_binary_operation(math::shl); }
void tcontroller::math_shr() noexcept { math_binary_operation(math::shr); }

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

void tcontroller::parse(std::string_view input) {
  int64_t value;
  std::from_chars_result result =
      std::from_chars(input.begin(), input.end(), value);

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
} // namespace calculator
