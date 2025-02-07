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

#include <string_view>

export module tests.handle_input;

import calculator.controller;
import calculator.model;

namespace calculator {

/**
 * Helper function to process an input string in the model.
 *
 * Using this function makes it possible to add values to the model's stack
 * without directly pushing values. Using this function lets the unit tests
 * resemble the normal application's usage pattern.
 */
export void handle_input(tcontroller &controller, tmodel &model,
                         std::string_view input) {
  model.input_append(input);
  controller.handle_keyboard_input(tkey::enter);
}
} // namespace calculator
