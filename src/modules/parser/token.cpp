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

#include <string>

export module parser.token;

namespace parser {

/** The struct containing the results of the parsing. */
export struct ttoken {
  enum class ttype {
    /** Returning this state is a bug in the library. */
    internal_error,
    /**
     * A numeric value was detected but contained invalid characters.
     *
     * Invalid characters can be digits invalid for the selected base or other
     * invalid characters.
     */
    invalid_value,
    /**
     * The @ref string contains an signed intgeral value.
     *
     * The value may fail to parse when it's larger than the storage type.
     */
    signed_value,
    /**
     * The @ref string contains an unsigned intgeral value.
     *
     * The value may fail to parse when it's larger than the storage type.
     */
    unsigned_value,
    /**
     * The @ref string contains a floating point value.
     *
     * The value may fail to parse when it's larger than the storage type.
     */
    floating_point_value,
    /**
     * The @ref string contains a string value.
     *
     * @note That at the moment strings are handle the same as @ref
     * invalid_value. However it's intended to add support for strings in the
     * engine, not as value but for operations and constants.
     */
    string_value
  };
  ttype type{ttype::internal_error};

  /** When a valid value is parsed it contains the parsed value. */
  std::string string{};

  bool operator==(const ttoken &) const = default;
};
} // namespace parser
