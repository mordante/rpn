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
      return 1;
    }
    return 0;
  }

  void process_input_event() noexcept;

  void input_append(std::string_view data);

  void update_ui();

  Fl_Box diagnostics_{5, 5, 300, 20};
  Fl_Browser stack_{5, 30, 300, 200};
  Fl_Box input_{5, 235, 300, 20};

  calculator::tmodel model_;
  calculator::tcontroller controller_{model_};
};

void twindow::process_input_event() noexcept {
  try {

    // *** Handle special keys ***
    switch (Fl::event_key()) {
    case FL_Enter:
    case FL_KP_Enter:
      controller_.push();
      update_ui();
      return;
    }

    const std::string text = Fl::event_text();
    // *** Handle special values ***
    if (text.size() == 1) {
      switch (text[0]) {
      case '+':
        controller_.math_add();
        update_ui();
        return;
      }
    }

    // *** Add the text to the input ***
    input_append(text);
  } catch (...) {
  }
}

void twindow::input_append(std::string_view data) {
  controller_.append(data);
  update_ui();
}

void twindow::update_ui() {
  diagnostics_.label(model_.diagnostics_get().c_str());

  stack_.clear();
  for (const auto &value : model_.stack())
    stack_.insert(std::numeric_limits<int>::max(), value.format().c_str());

  input_.label(model_.input_get().c_str());
}

int main(int argc, char **argv) {
  twindow window;
  window.end();
  window.show(argc, argv);
  return Fl::run();
}
