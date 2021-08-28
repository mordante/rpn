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

export module calculator.transaction;

import calculator.model;

import<algorithm>;
import<string>;
import<vector>;

namespace calculator {

/** The interface of an step of an action. */
class tstep_ {
public:
  tstep_() = default;
  tstep_(const tstep_ &) = delete;
  tstep_(tstep_ &&) = delete;
  virtual ~tstep_() = default;
  tstep_ &operator=(const tstep_ &) = delete;
  tstep_ &operator=(tstep_ &&) = delete;

  /** This implements the way to undo the changes done in this step. */
  virtual void undo(tmodel &model) = 0;

  /** This implements the way to redo the changes done in this step. */
  virtual void redo(tmodel &model) = 0;
};

class tinput final : public tstep_ {
public:
  /** Handles the stealing of @p input from the model's input. */
  explicit tinput(const std::string &input) : input_(input) {}

  void undo(tmodel &model) override {
    // There's no input setter, so first need to remove the current contents.
    (void)model.input_steal();
    model.input_append(input_);
  }
  void redo(tmodel &model) override { (void)model.input_steal(); }

private:
  std::string input_;
};

class tpop final : public tstep_ {
public:
  /** Handles the popping or dropping of @p value from the model's stack. */
  explicit tpop(tvalue value) : value_(value) {}

  void undo(tmodel &model) override { model.stack().push(value_); }
  void redo(tmodel &model) override { model.stack().drop(); }

private:
  tvalue value_;
};

class tpush final : public tstep_ {
public:
  /** Handles the pushing of @p value from the model's stack. */
  explicit tpush(tvalue value) : value_(value) {}

  void undo(tmodel &model) override { model.stack().drop(); }
  void redo(tmodel &model) override { model.stack().push(value_); }

private:
  tvalue value_;
};

class tduplicate final : public tstep_ {
public:
  /** Handles the duplicating model's stack last entry. */
  void undo(tmodel &model) override { model.stack().drop(); }
  void redo(tmodel &model) override { model.stack().duplicate(); }
};

/**
 * The action class to undo and redo actions.
 *
 * This class helps to implement:
 * - an undo and redo system,
 * - improved error handling.
 *
 * One action may consist of several steps, for example:
 * - stack contains 2
 * - the input contains 1
 * - the users types +
 * Then:
 * 1 is pushed as value on the stack
 * 1 is popped from the stack
 * 2 is popped from the stack
 * 3 is pushed on the stack
 *
 * Undoing should undo these 4 steps.
 *
 * This class is created by a @ref ttransaction object. This object records the
 * steps associated with one action.
 */
export class taction {
protected:
  explicit taction(tmodel &model) : model_(model) {}

public:
  taction(const taction &) = delete;
  taction(taction &&) = default;
  ~taction() = default;
  taction &operator=(const taction &) = delete;
  taction &operator=(taction &&action) {
    this->~taction();
    new (this) taction(std::move(action));
    return *this;
  }

  /**
   * Undo the action.
   *
   * @pre This function can be called once before a call to @ref undo or once
   * after every concecutive call to @ref redo.
   */
  void undo() {
    std::for_each(steps_.rbegin(), steps_.rend(),
                  [this](auto &step) { step->undo(model_); });
  }
  /**
   * Redo the action.
   *
   * @pre The function can only be called once after @ref undo is called.
   */
  void redo() {
    std::for_each(steps_.begin(), steps_.end(),
                  [this](auto &step) { step->redo(model_); });
  }

protected:
  tmodel &model_;

  std::vector<std::unique_ptr<tstep_>> steps_;
};

/**
 * The transaction step to build a @ref taction.
 *
 * @warning All actions in this class first need modify the model. After this
 * has succeeded the undo information is stored. This avoids recording failed
 * changes, which may cause undo operations to fail.
 */
export class ttransaction : public taction {
public:
  explicit ttransaction(tmodel &model) : taction(model) {}

  /**
   * When a roll back throws we just die.
   *
   * Throwing should be impossible:
   * - The string appended to the input buffer was taken from it, so that
   *   string should have a buffer large enough.
   * - Popping elements from the stack should always be safe.
   * - Pushing elements to the stack should be safe since these elements were
   *   taken from the stack, so the buffer should be large enough.
   */
  ~ttransaction() noexcept {
    // When this object is released it's a safe to test whether the container
    // is empty.
    if (!steps_.empty() && std::uncaught_exceptions())
      undo();
  }

  /** Handles the stealing of @p input from the model's input. */
  [[nodiscard]] std::string input_steal() {
    std::string result = model_.input_steal();
    steps_.push_back(std::make_unique<tinput>(result));
    return result;
  };

  /** Handles the popping of @p value from the model's stack. */
  [[nodiscard]] tvalue pop() {
    tvalue result = model_.stack().pop();
    steps_.push_back(std::make_unique<tpop>(result));
    return result;
  }

  /** Handles the dropping of @p value from the model's stack. */
  void drop() {
    tvalue result = model_.stack().pop();
    steps_.push_back(std::make_unique<tpop>(result));
  }

  /** Handles the pushing of @p value from the model's stack. */
  void push(tvalue value) {
    model_.stack().push(value);
    steps_.push_back(std::make_unique<tpush>(value));
  }

  /** Handles the duplicating model's stack last entry. */
  void duplicate() {
    model_.stack().duplicate();
    steps_.push_back(std::make_unique<tduplicate>());
  };

  /**
   * Finalises the transaction as succesful.
   *
   * It's not required to call this function, when the class is destroyed
   * without an active exception the transaction is considered a success.
   *
   * The intention of this function is to store the transaction as an action
   * which can be used to implement the undo/redo feature.
   */
  [[nodiscard]] taction release() && { return std::move(*this); }
};

} // namespace calculator
