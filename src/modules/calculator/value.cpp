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

export module calculator.value;

import<cinttypes>;
import<string>;
export import<variant>;

namespace calculator {

using tstorage = std::variant<int64_t, uint64_t>;

/**
 * Basic value class for the entries on the calculator's stack.
 *
 * At the moment class is a simple wrapper for a @c int64_t. For the future
 * some changes are planned:
 * - Allow @c __int128_t as storage type.
 * - Allow @c double as storage type.
 * - Cache the output of @ref to_string.
 *
 * For these reasons the stack uses this class instead of a @c int64_t directly.
 */
export class tvalue final {
public:
  tvalue() = default;

  /** Converting constructor. */
  constexpr tvalue(int64_t value) noexcept : value_(value) {}

  /**
   * A visitor to use the internal values of the stored value.
   *
   * @todo This there are no unit tests for this interface yet.
   */
  template <class Visitor> auto visit(Visitor &&visitor) const {
    return std::visit(std::forward<Visitor>(visitor), value_);
  }

  [[nodiscard]] constexpr int64_t get() const noexcept {
    return std::get<int64_t>(value_);
  }

private:
  tstorage value_{0};
};

} // namespace calculator
