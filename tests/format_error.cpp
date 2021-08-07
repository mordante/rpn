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

export module tests.format_error;

import<string>;
import<format>;

export std::string format_error(const char *message) {
#if defined(__cpp_lib_format)
  return std::format("[ERR]{:>80.79}", message);
#else
  return message;
#endif
}
