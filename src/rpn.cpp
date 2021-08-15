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

import calculator;

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Window.H>
#include <iostream>

class twindow final : public Fl_Window {
public:
  twindow() : Fl_Window(310, 265, "RPN") {
    diagnostics_.align(FL_ALIGN_INSIDE | FL_ALIGN_RIGHT);
    diagnostics_.box(FL_DOWN_BOX);

    input_.align(FL_ALIGN_INSIDE | FL_ALIGN_RIGHT);
    input_.box(FL_DOWN_BOX);
  }

private:
  int handle(int event) override {
    switch (event) {
    case FL_KEYDOWN:
    case FL_PASTE:
      process_input_event();
      update_ui();
      return 1;
    }
    return 0;
  }

  /** Process the user provided input. */
  void process_input_event();

  /** Updates the UI after changes need to be processed. */
  void update_ui();

  // *** Widgets ***

  Fl_Box diagnostics_{5, 5, 300, 20};
  Fl_Browser stack_{5, 30, 300, 200};
  Fl_Box input_{5, 235, 300, 20};

  // *** Model & controller ***

  calculator::tmodel model_;
  calculator::tcontroller controller_{model_};
};

static int convert_key_pad(int key) {
  if (key >= FL_KP && key <= FL_KP + 9)
    return '0' + key - FL_KP;

  return key;
}

// Note this expects ASCII.
static bool is_printable(int key) { return key >= 32 && key <= 127; }

void twindow::process_input_event() {
  // *** Handle special keys ***
  int key = Fl::event_key();
  switch (Fl::event_key()) {
  case FL_BackSpace:
    controller_.handle_keyboard_input(calculator::tkey::backspace);
    return;

  case FL_Enter:
  case FL_KP_Enter:
    controller_.handle_keyboard_input(calculator::tkey::enter);
    return;
  }

  // *** Handle control pressed ***
  const bool control =
      Fl::event_key(FL_Control_L) || Fl::event_key(FL_Control_R);
  if (control) {
    key = convert_key_pad(key);
    if (is_printable(key))
      controller_.handle_keyboard_input(calculator::tmodifiers::control, key);
    return;
  }

  const std::string text = Fl::event_text();
  switch (text.size()) {
  case 0:
    /* Do nothing */
    break;

  case 1:
    // *** The normal characters ***
    controller_.handle_keyboard_input(calculator::tmodifiers::none, text[0]);
    break;

  default:
    // *** Special strings ***

    // TODO Validate whether we should add them at all.
    controller_.append(text);
  }
}

void twindow::update_ui() {
  diagnostics_.label(model_.diagnostics_get().c_str());

  stack_.clear();
  for (const auto &value : model_.stack())
    stack_.insert(std::numeric_limits<int>::max(), value.c_str());
  stack_.bottomline(stack_.size());

  input_.label(model_.input_get().c_str());
}

int main(int argc, char **argv) {
  twindow window;
  window.end();
  window.show(argc, argv);
  return Fl::run();
}
