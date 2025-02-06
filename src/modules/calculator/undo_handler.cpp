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

#include <stdexcept>
#include <utility>
#include <vector>

export module calculator.undo_handler;

export import calculator.transaction;

namespace calculator {

/**
 * The class contains a buffer with the actions to undo and redo.
 *
 * When an action is undone it can be redone. When a new action is added this
 * is always the new actiont to be undone. This means after:
 * - undoing an action (a)
 * - and adding an action (b)
 * It's no longer possible to redo action (a). So the undo history is lineair
 * and not a tree.
 */
export class tundo_handler {
public:
  void undo() {
    if (active_ == 0)
      throw std::range_error("Undo stack underflow");

    --active_;
    actions_[active_].undo();
  }
  void redo() {
    if (active_ == head_)
      throw std::range_error("Undo stack overflow");

    actions_[active_].redo();
    ++active_;
  }
  void add(taction &&action) {
    if (active_ < actions_.size())
      // Overwrite an action, this action until the end of the buffer can no
      // longer be redone. So it doesn't matter the additional elements in the
      // vector are "corrupt". (Corrupt meaning they no longer are a proper diff
      // to their previous action.)
      actions_[active_] = std::move(action);
    else
      actions_.push_back(std::move(action));

    ++active_;
    head_ = active_;
  }

private:
  /**
   * A buffer containing the actions to undo or redo.
   *
   * When actions are undo @ref active_ points at the last undone action.
   * @ref head_ points to the last action on the action stack. That way it's
   * possible to keep track of what can and what can't be redone. Adding a new
   * action to the list "erases" all possible actions to be redone.
   *
   * @note Since adding elements may relocate the buffer it's being tracked by
   * index instead of by iterator.
   */
  std::vector<taction> actions_;

  /** The head of the buffer @ref actions_. */
  unsigned head_{0};

  /** The active action of the ring-buffer @ref actions_. */
  unsigned active_{0};
};

} // namespace calculator
