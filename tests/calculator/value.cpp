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

import calculator.value;

#include <gtest/gtest.h>

namespace calculator {
TEST(value, converting_constructor) {
  static_assert(noexcept(tvalue{1}));
}

TEST(value, copy_constructor) {
  constexpr tvalue v{1};
  static_assert(noexcept(tvalue{v}));
}

TEST(value, move_constructor) {
  constexpr tvalue v{1};
  static_assert(noexcept(tvalue{std::move(v)}));
}

TEST(value, destructor) { static_assert(noexcept(tvalue{1}.~tvalue())); }

} // namespace calculator
