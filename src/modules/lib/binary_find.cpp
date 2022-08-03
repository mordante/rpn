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

export module lib.binary_find;

import <functional>;
import <algorithm>;
import <ranges>;

namespace lib {
namespace detail {

export struct binary_find {
  template <std::forward_iterator I, std::sentinel_for<I> S, class T,
            class Proj = std::identity,
            std::indirect_strict_weak_order<const T *, std::projected<I, Proj>>
                Pred = std::ranges::less>
  constexpr I operator()(I first, S last, const T &value, Pred pred = {},
                         Proj proj = {}) const {
    return execute(first, last, value, pred, proj);
  }

  template <std::ranges::forward_range R, class T, class Proj = std::identity,
            std::indirect_strict_weak_order<
                const T *, std::projected<std::ranges::iterator_t<R>, Proj>>
                Pred = std::ranges::less>
  constexpr std::ranges::borrowed_iterator_t<R>
  operator()(R &&r, const T &value, Pred pred = {}, Proj proj = {}) const {
    return execute(std::ranges::begin(r), std::ranges::end(r), value, pred,
                   proj);
  }

private:
  template <std::forward_iterator I, class S, class T, class Proj, class Pred>
  static constexpr I execute(I first, S last, const T &value, Pred &pred,
                             Proj &proj) {
    I result = std::ranges::lower_bound(first, last, value, pred, proj);

    // Typically code looks like
    //   result != last && !std::invoke(pred, value, std::invoke(proj, *result)
    // If true the *result == value and the result is returned.
    // Else last is returned.
    // However calculating last might be expensive, and if the calculation is
    // already done, no need to redo it. Therefore it compares equal to last.
    if (result == last || !std::invoke(pred, value, std::invoke(proj, *result)))
      return result;

    if constexpr (std::same_as<I, S>)
      return last;
    else
      // Note this can be quite inefficient, but it's not expected to be used in
      // the application.
      return std::ranges::next(first, std::ranges::distance(first, last));
  }
};

} // namespace detail

inline namespace cpo {
/** Executes a find on a sorted range. */
export inline constexpr auto binary_find = detail::binary_find{};
} // namespace cpo

} // namespace lib
