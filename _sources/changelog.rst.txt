============
Changelog
============

.. contents::
   :local:


Version 0.3.0
=============

Focusses on improving the input parsing and adding more operations.

* Improved the input handling:

  * A space can now be used as separator instead of needing to press return
    after every entry.
  * An underscore or comma can be used as a numeric separator in the input,
    allowing to easier write numbers with many digits.
  * Floating-point values can now be entered.
  * A signed integer values can now be entered, with a special syntax.
  * Added textual constants to put a numeric value on the stack.

* Additional operations:
  * Rounding: round, floor, ceil, trunc.

Version 0.2.0
=============

Focusses on improving the calculations.

* Added better error recovering, undo, and redo.
* Large values are now represented as floating-point values instead of using
  incorrect values.
* Division now properly uses floating-point results instead of truncating.

Version 0.1.0
=============

Initial release.
