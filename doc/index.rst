==============
RPN Calculator
==============

.. contents::
   :local:

This is yet another
`RPN <https://en.wikipedia.org/wiki/Reverse_Polish_notation>`_ calculator. It's
still in its infancy and the goal of the application is to have better support
for different bases and make it easy to use bitwise operations.

Next to its practical usage the application is used to experiment with:

 * The latest C++ features available in `Clang <clang.llvm.org/>`_ and
   `libc++ <libcxx.llvm.org/>`_.
 * The possible CI [#note-CD]_ features of GitHub actions.

The sources of the project are at `GitHub <https://github.com/mordante/rpn>`_.

Building
========

In order to build the application the following dependencies are required:

* `CMake <https://cmake.org/>`_ 3.13.0 or later. Once CMake adds proper module
  support the requirement will be incremented to that version.
* A recent `Clang <clang.llvm.org/>`_ main build. Clang 13.0.0 is expected to
  work, but when module support improves on Clang a more recent build will be
  required.
* A recent `libc++ <libcxx.llvm.org/>`_ main build. In libc++ both ``<ranges>``
  and ``<format>`` are actively being developed. When features land in the main
  branch the application will use some of these features.
* A working Internet connection. The application depends on
  `GoogleTest <http://google.github.io/googletest/>`_ and
  `FLTK <https://cmake.org/>`_. The proper dependencies are downloaded during
  the build process.
* Optional when enabling coverage ``llvm-cov`` and ``llvm-profdata`` are
  required. This should be the same version as Clang, but normally they are
  installed together with Clang.

The build itself is a normal CMake build. It has the following options:

Usage
=====

GUI
---

The application has a simple GUI with the following elements:

Diagnostics
  Contains the diagnostics issued by the application. Usually an error message
  regarding the last operation.

Stack
  The stack with the values to operate on.

Input
  The input buffer where new text is entered.

At the moment the GUI has no real interaction with the user and can't be
controlled. All keyboard input is automatically processed by the input buffer.

Operations
----------

Most operations will require one or two stack elements. The basic operation is:

* take an element from the stack ``arg 1``,
* take an element from the stack ``arg 2``,
* execute the requested postfix operation,
* push the result of the operation on the stack.

To do a simple addition like ``1 + 2`` would be:

#. type ``1``
#. press ``return``, the input is empty and ``1`` is stored on the stack
#. type ``2``
#. press ``return``, the input is empty and ``1`` and ``2`` are stored on the
   stack  
#. press ``+``, the input is empty and the ``3`` is stored on the stack

In these operations step 4 is optional. When the operation is performed with a
non-empty input this input will be pushed on the stack before the
operation [#note-stack]_.

This is a short summary of the operations, but behind the scenes the
`calculations <calculation.html>`_ have more complexity.

* Arithmetic:

  ``+``
    Adds two elements on the stack.
  ``-``
    Subtracts two elements on the stack. This can't be used for negative
    values. Entering negative values is done with ``Ctrl + n``.
  ``*``
    Multiplies two elements on the stack.
  ``/``
    Divides two elements on the stack.
  ``Ctrl + n``
    Negates one element on the stack. This is the way to get negative values.

* Bitwise:

  ``&``
    Calculates the bitwise and of two elements on the stack.
  ``|``
    Calculates the bitwise or of two elements on the stack.
  ``^``
    Calculates the bitwise xor of two elements on the stack.
  ``~``
    Calculates the bitwise complement of one element on the stack.
  ``<``
    Calculates the bitwise shift left of one element on the stack.
  ``>``
    Calculates the bitwise shift right of one element on the stack.

* Base:

  The application allows the user to switch the base used to display values.
  The based used for input and output are separate.

  * Input:

    * Binary (base 2), prefix the input with ``0b``.
    * Octal (base 8), prefix the input with ``0``. A single ``0`` is considered
      a decimal zero. But since zero is identical in all bases that's not an
      issue.
    * Decimal (base 10), this is the default and requires no prefix.
    * Hexadecimal (base 16), prefix the input with ``0x``. All hexadecimal
      characters should be lowercase.

  * Output:

    By default the output is decimal. This can be changed. The change modifies
    all elements on the stack. The input is unaffected.

    * Binary (base 2), use ``Ctrl + b``.
    * Octal (base 8), use ``Ctrl + o``.
    * Decimal (base 10), use ``Ctrl + d``.
    * Hexadecimal (base 16), use ``Ctrl + h``.

* Miscellaneous:

  ``Backspace``
    As long as the input buffer isn't empty it removes the last character from
    the input buffer. When the input buffer is empty it discards on element
    from the stack.

  ``Ctrl + z``
    Undo the last operation. This only affects the input buffer and the stack.
    Multiple actions can be undo until reaching the initial state of the
    application.

  ``Ctrl + Z``
    Redo the last undone operation. Multiple actions can be redone until

    * reaching the final undo action,
    * doing a regular operation.

Known limitations
=================

Building requires a git build of Clang and libc++
-------------------------------------------------

The goal of the application was to experiment with modules. Since module support
in various compilers is in various stages of development I want to focus on one
toolchain. Once modules becomes production quality in both compilers and CMake
other compiler might be supported.

The GUI isn't great
-------------------

Basically the GUI is only used for output nothing else at the moment. The user
interaction is low on the priority list. The initial goal is to get something
usable for me.

The GUI is inefficient
----------------------

The application is quite happy to redraw too many elements. An attempt has been
made to improve this. An observer was created, however it seems that Clang
modules and vtables don't play along nicely with the linker. These linker errors
prevented me from using this pattern. Since the application is fast enough this
improvement has dropped low on the priority list.

Links
=====

.. toctree::
   :maxdepth: 1

   calculation
   changelog

* `Coverage report <coverage/index.html>`_

.. note::

  .. [#note-CD] There's a CD part too, but since the application isn't in a
     deployable state there's no need to have a real CD part. Still this page is
     deployed by Github actions.

  .. [#note-stack] The current implementation really pushes the element on the
     stack. However future implementations might optimize this by using an
     register. But that's an implementation detail which doesn't affect the
     observed behaviour for the user.
