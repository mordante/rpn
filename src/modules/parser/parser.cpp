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

module;

#include <__config>

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER >= 17
template <class _Tp>
[[nodiscard]] _LIBCPP_HIDE_FROM_ABI constexpr const _Tp &
as_const(_Tp &__t) noexcept {
  return __t;
}

template <class _Tp> void as_const(const _Tp &&) = delete;
#endif

_LIBCPP_END_NAMESPACE_STD

#include <algorithm>
#include <cctype>
#include <string_view>
#include <vector>

export module parser;

export import parser.token;

import parser.detail.base;
import parser.detail.invalid_value;
import parser.detail.floating_point;
import parser.detail.unsigned_value;
import parser.detail.string;
import parser.detail.signed_value;

namespace parser {

export struct tparser {
public:
  void append(char data) { parse(data); }
  void append(std::string_view data) {
    std::ranges::for_each(data, [this](char c) { parse(c); });
  }

  bool accept_minus() const {
    return parser_ ? parser_->accept_minus() : false;
  }

  void reset() {
    parser_.reset();
    result_.clear();
  }

  const std::vector<ttoken> &process() {
    finish();

    return result_;
  }

private:
  std::unique_ptr<tparser_> parser_;

  std::vector<ttoken> result_;

  void add_result() {
    result_.push_back(parser_->result());
    parser_.reset();
  }

  void parse(char c) {
    if (!parser_) [[unlikely]] {
      switch (c) {
      case '.':
        parser_ = std::make_unique<tparser_floating_point>();
        break;
      case 'i':
        parser_ = std::make_unique<tparser_signed>();
        break;
      default:
        if (isdigit(c))
          parser_ = std::make_unique<tparser_unsigned>();
        else
          parser_ = std::make_unique<tparser_string>();
        break;
      }
    }
    std::unique_ptr<tparser_> replacement = parser_->parse(c);
    if (replacement)
      parser_.reset(replacement.release());

    if (parser_->is_complete())
      add_result();
  }

  void finish() {
    if (!parser_)
      return;

    parser_->finish();
    add_result();
  }
};

} // namespace parser
