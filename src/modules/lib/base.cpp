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

export module lib.base;

namespace lib {

// TODO This does not seem the best location.
// Need to consider alternatives.

/** The supported bases in the calculator. */
export enum class tbase { binary, octal, decimal, hexadecimal };

} // namespace lib
