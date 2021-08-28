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

export import calculator.stack;

import<string>;
import<string_view>;

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

  // *** Diagnostics ****

  void diagnostics_set(std::string &&diagnotics) {
    diagnotics_ = std::move(diagnotics);
  }

  [[nodiscard]] const std::string &diagnostics_get() const noexcept {
    return diagnotics_;
  }

  /** Clears the current diagnostics. */
  void diagnostics_clear() noexcept { diagnotics_.clear(); }

  // *** Stack ***

  [[nodiscard]] const tstack &stack() const noexcept { return stack_; }
  [[nodiscard]] tstack &stack() noexcept { return stack_; }

  // *** Input ***

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
  void input_append(char data) { input_.push_back(data); }
  void input_append(std::string_view data) { input_.append(data); }

  /**
   * Removes the last character of the @ref input_.
   *
   * @returns Whether an element was removed.
   */
  [[nodiscard]] bool input_pop_back() {
    if (input_.empty())
      return false;
    input_.pop_back();
    return true;
  }

  [[nodiscard]] const std::string &input_get() const noexcept { return input_; }

  // *** Base ***

  /**
   * @note The base is part of the stack, but it makes more sense to have the
   * settings accessed via the model instead of the stack. So keep the function
   * here.
   */
  void base_set(tbase base) { stack_.base_set(base); }

private:
  /** The execution issues to report to the user. */
  std::string diagnotics_{};

  /** The stack containing the values for the calculation. */
  tstack stack_{};

  /** The input buffer used to store the current editting session. */
  std::string input_{};
};

} // namespace calculator