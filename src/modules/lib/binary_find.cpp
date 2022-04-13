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
  template <class I, class S, class T, class Pred, class Proj>
  static constexpr I lower_bound(I first, S size, const T &value, Pred &pred,
                                 Proj &proj) {
    while (size > 0) {
      S mid = size / 2;
      I it = std::ranges::next(first, mid);
      if (std::invoke(pred, std::invoke(proj, *it), value)) {
        first = ++it;
        size -= mid + 1;
      } else
        size = mid;
    }

    return first;
  }

  template <std::forward_iterator I, class S, class T, class Proj, class Pred>
  static constexpr I execute(I first, S last, const T &value, Pred &pred,
                             Proj &proj) {
    using size_type = std::iter_difference_t<I>;

    // Observation: When using a std::forward_iterator and !std::same_as<I, S>
    // the end information of the end of the range is lost. So getting the end
    // is another O(N) operation.
    size_type size = std::ranges::distance(first, last);

    // Need to keep first, it's used when !std::same_as<I, S>.
    I result = lower_bound(first, size, value, pred, proj);

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
      return std::ranges::next(first, size);
  }
};

} // namespace detail

inline namespace cpo {
/** Executes a find on a sorted range. */
export inline constexpr auto binary_find = detail::binary_find{};
} // namespace cpo

} // namespace lib
