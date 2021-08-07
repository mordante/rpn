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
  twindow(int w, int h, const char *t) : Fl_Window(w, h, t) {
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
  void stack_push();

  void update_stack();

  Fl_Browser stack_{5, 5, 300, 200};
  Fl_Box input_{5, 210, 300, 20};

  //  std::string buffer_{};
  calculator::tmodel model_;
  calculator::tcontroller controller_{model_};
};

void twindow::process_input_event() noexcept {
  try {

    // *** Handle special keys ***
    switch (Fl::event_key()) {
    case FL_Enter:
    case FL_KP_Enter:
      stack_push();
      update_stack();
      return;
    }

    const std::string text = Fl::event_text();
    /*    if (text.empty())
          return;
    */
    // *** Handle special values ***
    if (text.size() == 1) {
      switch (text[0]) {
      case '+':
        controller_.math_add();
        update_stack();
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
  //  buffer_ += data;
  input_.label(model_.input_get().c_str());
}

void twindow::stack_push() {
  /*
    if (buffer_.empty())
      return;

    stack_.insert(std::numeric_limits<int>::max(), buffer_.c_str());

    buffer_.clear();
    input_.label(buffer_.c_str());
    */
  //  stack_.insert(std::numeric_limits<int>::max(),
  //  model_.input_get().c_str());
  input_.label("");
  controller_.push();
}

void twindow::update_stack() {
  stack_.clear();

  for (const auto &value: model_.stack())
    stack_.insert(std::numeric_limits<int>::max(), value.format().c_str());
}

int main(int argc, char **argv) {
  twindow w{310, 240, "RPN"};
  w.end();
  w.show(argc, argv);
  return Fl::run();

  /*  Fl_Window *window = new Fl_Window(340, 180);
    Fl_Box *box = new Fl_Box(20, 40, 300, 100, "Hello, World!");
    box->box(FL_UP_BOX);
    box->labelfont(FL_BOLD + FL_ITALIC);
    box->labelsize(36);
    box->labeltype(FL_SHADOW_LABEL);
    window->end();
    window->show(argc, argv);
    return Fl::run(); */
}
