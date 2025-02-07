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

import lib.binary_find;

import tests.test_constexpr;

#include <array>
#include <concepts>
#include <forward_list>
#include <ranges>
#include <string_view>

#include <gtest/gtest.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

namespace lib {

/*
 * Array tests use the random access iterator path.
 */
TEST(binary_find, iter_iter) {
  test_constexpr([] {
    std::array arr{1, 2, 3, 5};
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), 1);
      assert(it == arr.data());
    }
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), 2);
      assert(it == arr.data() + 1);
    }
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), 3);
      assert(it == arr.data() + 2);
    }
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), 4);
      assert(it == arr.data() + arr.size());
    }
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), 5);
      assert(it == arr.data() + 3);
    }
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), 6);
      assert(it == arr.data() + arr.size());
    }
    return true;
  });
}

TEST(binary_find, iter_iter_proj) {
  test_constexpr([] {
    struct kv {
      std::string_view key;
      int value;
    };
    std::array<kv, 4> arr{{{"1", 1}, {"2", 2}, {"3", 3}, {"5", 5}}};
    {
      auto it =
          binary_find(arr.data(), arr.data() + arr.size(), "1", {}, &kv::key);
      assert(it == arr.data());
      assert(it->value == 1);
    }
    {
      auto it =
          binary_find(arr.data(), arr.data() + arr.size(), "2", {}, &kv::key);
      assert(it == arr.data() + 1);
      assert(it->value == 2);
    }
    {
      auto it =
          binary_find(arr.data(), arr.data() + arr.size(), "3", {}, &kv::key);
      assert(it == arr.data() + 2);
      assert(it->value == 3);
    }
    {
      auto it =
          binary_find(arr.data(), arr.data() + arr.size(), "4", {}, &kv::key);
      assert(it == arr.data() + arr.size());
    }
    {
      auto it =
          binary_find(arr.data(), arr.data() + arr.size(), "5", {}, &kv::key);
      assert(it == arr.data() + 3);
      assert(it->value == 5);
    }
    {
      auto it =
          binary_find(arr.data(), arr.data() + arr.size(), "6", {}, &kv::key);
      assert(it == arr.data() + arr.size());
    }
    return true;
  });
}

TEST(binary_find, iter_iter_pred_proj) {
  test_constexpr([] {
    struct kv {
      std::string_view key;
      int value;
    };
    std::array<kv, 4> arr{{{"5", 5}, {"3", 3}, {"2", 2}, {"1", 1}}};
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), "1",
                            std::ranges::greater{}, &kv::key);
      assert(it == arr.data() + 3);
      assert(it->value == 1);
    }
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), "2",
                            std::ranges::greater{}, &kv::key);
      assert(it == arr.data() + 2);
      assert(it->value == 2);
    }
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), "3",
                            std::ranges::greater{}, &kv::key);
      assert(it == arr.data() + 1);
      assert(it->value == 3);
    }
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), "4",
                            std::ranges::greater{}, &kv::key);
      assert(it == arr.data() + arr.size());
    }
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), "5",
                            std::ranges::greater{}, &kv::key);
      assert(it == arr.data());
      assert(it->value == 5);
    }
    {
      auto it = binary_find(arr.data(), arr.data() + arr.size(), "6",
                            std::ranges::greater{}, &kv::key);
      assert(it == arr.data() + arr.size());
    }
    return true;
  });
}

struct nul_terminator {
  friend constexpr bool operator==(const char *c, nul_terminator) {
    return *c == '\0';
  }
};

TEST(binary_find, iter_sentinel) {
  test_constexpr([] {
    const char *data = "1235";
    {
      auto it = binary_find(data, nul_terminator{}, '1');
      assert(it == &data[0]);
    }
    {
      auto it = binary_find(data, nul_terminator{}, '2');
      assert(it == &data[1]);
    }
    {
      auto it = binary_find(data, nul_terminator{}, '3');
      assert(it == &data[2]);
    }
    {
      auto it = binary_find(data, nul_terminator{}, '4');
      assert(it == &data[4]);
    }
    {
      auto it = binary_find(data, nul_terminator{}, '5');
      assert(it == &data[3]);
    }
    {
      auto it = binary_find(data, nul_terminator{}, '6');
      assert(it == &data[4]);
    }
    return true;
  });
}

TEST(binary_find, range) {
  test_constexpr([] {
    std::array arr{1, 2, 3, 5};
    {
      auto it = binary_find(arr, 1);
      assert(it == arr.data());
    }
    {
      auto it = binary_find(arr, 2);
      assert(it == arr.data() + 1);
    }
    {
      auto it = binary_find(arr, 3);
      assert(it == arr.data() + 2);
    }
    {
      auto it = binary_find(arr, 4);
      assert(it == arr.data() + arr.size());
    }
    {
      auto it = binary_find(arr, 5);
      assert(it == arr.data() + 3);
    }
    {
      auto it = binary_find(arr, 6);
      assert(it == arr.data() + arr.size());
    }
    return true;
  });
}

TEST(binary_find, range_dangling) {
  test_constexpr([] {
    {
      {
        auto it = binary_find(std::array{1, 2, 3, 5}, 1);
        static_assert(std::is_same_v<std::ranges::dangling, decltype(it)>);
      }
      {
        auto it = binary_find(std::array{1, 2, 3, 5}, 6);
        static_assert(std::is_same_v<std::ranges::dangling, decltype(it)>);
      }
      return true;
    }
  });
}

TEST(binary_find, range_proj) {
  test_constexpr([] {
    struct kv {
      std::string_view key;
      int value;
    };
    std::array<kv, 4> arr{{{"1", 1}, {"2", 2}, {"3", 3}, {"5", 5}}};
    {
      auto it = binary_find(arr, "1", {}, &kv::key);
      assert(it == arr.data());
      assert(it->value == 1);
    }
    {
      auto it = binary_find(arr, "2", {}, &kv::key);
      assert(it == arr.data() + 1);
      assert(it->value == 2);
    }
    {
      auto it = binary_find(arr, "3", {}, &kv::key);
      assert(it == arr.data() + 2);
      assert(it->value == 3);
    }
    {
      auto it = binary_find(arr, "4", {}, &kv::key);
      assert(it == arr.data() + arr.size());
    }
    {
      auto it = binary_find(arr, "5", {}, &kv::key);
      assert(it == arr.data() + 3);
      assert(it->value == 5);
    }
    {
      auto it = binary_find(arr, "6", {}, &kv::key);
      assert(it == arr.data() + arr.size());
    }
    return true;
  });
}

TEST(binary_find, range_pred_proj) {
  test_constexpr([] {
    struct kv {
      std::string_view key;
      int value;
    };
    std::array<kv, 4> arr{{{"5", 5}, {"3", 3}, {"2", 2}, {"1", 1}}};
    {
      auto it = binary_find(arr, "1", std::ranges::greater{}, &kv::key);
      assert(it == arr.data() + 3);
      assert(it->value == 1);
    }
    {
      auto it = binary_find(arr, "2", std::ranges::greater{}, &kv::key);
      assert(it == arr.data() + 2);
      assert(it->value == 2);
    }
    {
      auto it = binary_find(arr, "3", std::ranges::greater{}, &kv::key);
      assert(it == arr.data() + 1);
      assert(it->value == 3);
    }
    {
      auto it = binary_find(arr, "4", std::ranges::greater{}, &kv::key);
      assert(it == arr.data() + arr.size());
    }
    {
      auto it = binary_find(arr, "5", std::ranges::greater{}, &kv::key);
      assert(it == arr.data());
      assert(it->value == 5);
    }
    {
      auto it = binary_find(arr, "6", std::ranges::greater{}, &kv::key);
      assert(it == arr.data() + arr.size());
    }
    return true;
  });
}

/*
 * Forward_list tests use the random access iterator path.
 * These test can't be done constexpr, not really worth the effort to use a
 * custom constexpr forward_iterator.
 */
TEST(binary_find, forward_iterator_iter_iter) {
  std::forward_list list{1, 2, 3, 5};
  {
    auto it = binary_find(list.begin(), list.end(), 1);
    assert(it == list.begin());
  }
  {
    auto it = binary_find(list.begin(), list.end(), 2);
    assert(it == std::next(list.begin(), 1));
  }
  {
    auto it = binary_find(list.begin(), list.end(), 3);
    assert(it == std::next(list.begin(), 2));
  }
  {
    auto it = binary_find(list.begin(), list.end(), 4);
    assert(it == list.end());
  }
  {
    auto it = binary_find(list.begin(), list.end(), 5);
    assert(it == std::next(list.begin(), 3));
  }
  {
    auto it = binary_find(list.begin(), list.end(), 6);
    assert(it == list.end());
  }
}

struct kv {
  char key;
  int value;
};

struct terminator {
  friend constexpr bool operator==(auto it, terminator) {
    return it->key == '\0';
  }
};

TEST(binary_find, forward_iteratoriter_sentinel_pred_proj) {
  std::forward_list<kv> list{
      {{'5', 5}, {'3', 3}, {'2', 2}, {'1', 1}, {'\0', -1}}};
  {
    auto it = binary_find(list.begin(), terminator{}, '1',
                          std::ranges::greater{}, &kv::key);
    assert(it == std::next(list.begin(), 3));
    assert(it->value == 1);
  }
  {
    auto it = binary_find(list.begin(), terminator{}, '2',
                          std::ranges::greater{}, &kv::key);
    assert(it == std::next(list.begin(), 2));
    assert(it->value == 2);
  }
  {
    auto it = binary_find(list.begin(), terminator{}, '3',
                          std::ranges::greater{}, &kv::key);
    assert(it == std::next(list.begin(), 1));
    assert(it->value == 3);
  }
  {
    auto it = binary_find(list.begin(), terminator{}, '4',
                          std::ranges::greater{}, &kv::key);
    assert(it == std::next(list.begin(), 4));
    assert(it->key == '\0');
    assert(it->value == -1);
  }
  {
    auto it = binary_find(list.begin(), terminator{}, '5',
                          std::ranges::greater{}, &kv::key);
    assert(it == list.begin());
    assert(it->value == 5);
  }
  {
    auto it = binary_find(list.begin(), terminator{}, '6',
                          std::ranges::greater{}, &kv::key);
    assert(it == std::next(list.begin(), 4));
    assert(it->key == '\0');
    assert(it->value == -1);
  }
}

TEST(binary_find, forward_iterator_range) {
  std::forward_list list{1, 2, 3, 5};
  {
    auto it = binary_find(list, 1);
    assert(it == list.begin());
  }
  {
    auto it = binary_find(list, 2);
    assert(it == std::next(list.begin(), 1));
  }
  {
    auto it = binary_find(list, 3);
    assert(it == std::next(list.begin(), 2));
  }
  {
    auto it = binary_find(list, 4);
    assert(it == list.end());
  }
  {
    auto it = binary_find(list, 5);
    assert(it == std::next(list.begin(), 3));
  }
  {
    auto it = binary_find(list, 6);
    assert(it == list.end());
  }
}
} // namespace lib
