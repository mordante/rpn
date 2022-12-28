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

#include <string_view>

import gui;
import tui;

int main(int argc, char **argv) {
  if (argc > 1 && argv[1] == std::string_view{"-t"})
    return tui::run();
  return gui::run(argc, argv);
}
