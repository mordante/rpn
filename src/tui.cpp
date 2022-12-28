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

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

export module tui;

import calculator;

namespace {

// When not in an anonymous namespace the twindow classes in tui and gui give
// issues. This seems like a bug in Clang.
class tinput final : public ftxui::ComponentBase {
public:
  explicit tinput(calculator::tmodel &model)
      : model_(model), controller_(model) {
    TakeFocus();
  }

  ftxui::Element Render() override {
    return ftxui::hbox({ftxui::filler(), ftxui::text(model_.input_get())}) //
           | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1)                   //
           | ftxui::border;
  }
  bool OnEvent(ftxui::Event event) override {
    if (event == ftxui::Event::Backspace) {
      controller_.handle_keyboard_input(calculator::tkey::backspace);
      control_ = false;
    } else if (event == ftxui::Event::Tab) {
      control_ = true;
    } else if (event == ftxui::Event::Return) {
      controller_.handle_keyboard_input(calculator::tkey::enter);
      control_ = false;
    } else if (event.is_character()) {

      const std::string text = event.character();
      switch (text.size()) {
      case 0:
        /* Do nothing */
        break;

      case 1:
        // *** The normal characters ***
        controller_.handle_keyboard_input(control_
                                              ? calculator::tmodifiers::control
                                              : calculator::tmodifiers::none,
                                          text[0]);
        break;

      default:
        // *** Special strings ***

        // TODO Validate whether we should add them at all.
        controller_.append(text);
        break;
      }
      control_ = false;
    }

    // Swallow all events
    return true;
  }

  bool Focusable() const override { return true; }

private:
  // The special keys using control don't work properly in the terminal. Using
  // alt is iffy too. Instead we set control pressed after a tab key.
  bool control_{false};
  calculator::tmodel &model_;
  calculator::tcontroller controller_;
};

class twindow {
public:
  twindow() : input_{std::make_shared<tinput>(model_)} {}

  ftxui::Component screen() {
    return ftxui::Container::Vertical({
               ftxui::Renderer(input_,
                               [&] {
                                 return ftxui::vbox({diagnostics(), //
                                                     stack(),       //
                                                     input_->Render()});
                               }),
           }) |
           ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 61);
  }

private:
  ftxui::Element diagnostics() const {
    return ftxui::text(model_.diagnostics_get())         //
           | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1) //
           | ftxui::border;
  }
  ftxui::Element stack() const {
    ftxui::Elements stack;
    for (auto value : model_.stack().strings())
      stack.emplace_back(
          ftxui::hbox({ftxui::filler(), ftxui::text(std::move(value))}));

    return ftxui::vbox(stack)                                   //
           | ftxui::vscroll_indicator                           //
           | ftxui::focusPositionRelative(0.0, 1.0)             //
           | ftxui::yframe                                      //
           | ftxui::size(ftxui::HEIGHT, ftxui::GREATER_THAN, 4) //
           | ftxui::border;
  }

  calculator::tmodel model_;
  ftxui::Component input_;
};

} // namespace

export namespace tui {

int run() {

  // window clashes with window in gui module, which shouldn't happen
  twindow window;
  auto screen = ftxui::ScreenInteractive::FitComponent();
  screen.Loop(window.screen());
  return 0;
}

} // namespace tui
