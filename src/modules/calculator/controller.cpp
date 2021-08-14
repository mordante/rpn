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

  /**
   * Calculates the sum of two values.
   *
   * When the input isn't empty equivalent @c pop() @em op @a input
   * else equivalent @c pop() @em op @c pop()
   *
   * Upon success the diagnostics are cleared, else they contain the last error.
   */
  void math_add() noexcept;

private:
  void push(std::string_view input);

  void duplicate_last_entry();
  void parse(std::string_view input);

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

void tcontroller::math_add() noexcept {
  try {
    if (const std::string input = model_.input_steal(); !input.empty())
      push(input);

    const tvalue lhs = model_.stack_pop();
    const tvalue rhs = model_.stack_pop();
    model_.stack_push(math::add(lhs.get(), rhs.get()));
    model_.diagnostics_clear();
  } catch (const std::exception &e) {
    diagnostics_set(e);
  }
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