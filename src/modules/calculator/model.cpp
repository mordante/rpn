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

export module calculator.model;

export import calculator.value;

import<string>;
import<string_view>;
export import<vector>;

namespace calculator {

/**
 * The model of the calculator.
 *
 * For now a very basic version of this class.
 *
 * The class is non-copyable and non-movable. These operations make little
 * sense for the model and are most likely logic errors.
 */
export class tmodel final {
public:
  // *** SMF ***

  tmodel() = default;
  tmodel(const tmodel &) = delete;
  tmodel(tmodel &&) = delete;
  ~tmodel() = default;

  tmodel &operator=(const tmodel &) = delete;
  tmodel &operator=(tmodel &&) = delete;

  // *** Query ***

  [[nodiscard]] bool stack_empty() const noexcept { return stack_.empty(); }
  [[nodiscard]] size_t stack_size() const noexcept { return stack_.size(); }
  [[nodiscard]] const std::vector<tvalue> &stack() const noexcept {
    return stack_;
  }

  // *** Modifiers ***

  // * Diagnostics *

  void diagnostics_set(std::string &&diagnotics) {
    diagnotics_ = std::move(diagnotics);
  }

  [[nodiscard]] const std::string &diagnostics_get() const noexcept {
    return diagnotics_;
  }

  /** Clears the current diagnostics. */
  void diagnostics_clear() noexcept { diagnotics_.clear(); }

  // * Stack *

  /** Adds the @p value to the back of the stack. */
  void stack_push(tvalue value) { stack_.emplace_back(std::move(value)); }

  /**
   * @returns The last element at the back of the stack.
   * @throws @ref std::out_of_range when the stack is empty.
   */
  [[nodiscard]] tvalue stack_pop();

  // * Input *

  /**
   * Clears the current input.
   *
   * TODO Is this function still needed?
   */
  void input_clear() noexcept { input_.clear(); }

  /** Returns the clears the input and returns the original value. */
  [[nodiscard]] std::string input_steal() {
    std::string result;
    std::swap(input_, result);
    return result;
  }

  /**
   * Appends the @p data to the @ref input_. @note Since the expected usage
   * pattern is that the user only append (and maybe later removes) input
   * there's no setter required.
   */
  void input_append(std::string_view data) { input_.append(data); }

  [[nodiscard]] const std::string &input_get() const noexcept { return input_; }

private:
  /** The execution issues to report to the user. */
  std::string diagnotics_{};

  /**
   * The stack with all values of the applications.
   *
   * The usage pattern is a LIFO. Since @ref std::vector operates faster on the
   * the back new elements are added to the back. So the first item is the
   * oldest item.
   */
  std::vector<tvalue> stack_{};

  /** The input buffer used to store the current editting session. */
  std::string input_{};
};

tvalue tmodel::stack_pop() {
  if (stack_.empty())
    throw std::out_of_range("Stack is empty");

  tvalue result = stack_.back();
  stack_.pop_back();
  return result;
}

} // namespace calculator