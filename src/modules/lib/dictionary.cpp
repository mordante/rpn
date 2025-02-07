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

export module lib.dictionary;

import lib.binary_find;
import std;

namespace lib {
/**
 * Creates a dictionary.
 *
 * A dictionary is a set of key value pairs stored in a sorted array.
 *
 * This function converts all input @p args to a dictionary.
 * - key = arg[even]
 * - value = arg[odd]
 *
 * The supplied keys don't need to be sorted.
 *
 * @pre All keys have a unique value.
 * @pre A std::string_view can be constructed from a key.
 *
 * @pre All values are the same type.
 */
export template <class... Args>
  requires(sizeof...(Args) != 0 && sizeof...(Args) % 2 == 0)
consteval auto make_dictionary(Args... args) {
  // TODO Guard against temporary std::string.
  std::tuple all = std::make_tuple(args...);

  auto result = [&all]<std::size_t... I>(std::index_sequence<I...>) {
    return std::array{[&all] {
      // Make sure all values have the same type.
      static_assert(
          std::same_as<
              typename std::tuple_element<1, decltype(all)>::type,
              typename std::tuple_element<2 * I + 1, decltype(all)>::type>);

      // Creates the array elements.
      return std::make_pair(std::string_view(std::get<2 * I>(all)),
                            std::get<2 * I + 1>(all));
    }()...};
  }(std::make_index_sequence<sizeof...(Args) / 2>());

  std::ranges::sort(result, {}, &decltype(result)::value_type::first);

  return result;
}

/** Executes a find operation on a dictionary. */
export template <class K, class V, std::size_t N>
constexpr auto find(const std::array<std::pair<std::string_view, V>, N> &lut,
                    K &&key) {
  using E = std::pair<std::string_view, V>;
  return binary_find(lut, std::forward<K>(key), {}, &E::first);
}
} // namespace lib
