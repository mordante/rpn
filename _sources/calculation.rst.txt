============
Calculations
============

.. contents::
   :local:


Introduction
============

The elements on the stack have the following underlying types:

* ``int64_t`` a 64-bit signed integral.
* ``uint64_t`` a 64-bit unsigned integral.
* ``double`` a double precision floating-point value.

The engine mandates ``sizeof(int64_t) == sizeof(uint64_t) == sizeof(double)``.
For most types this isn't a real issue, but some bitwise operations execute
bit-casts.

Depending on the operation used the types used have different requirements.
This page describes these requirements. First an generic set of named
requirements. After that the requirements per type. When a requirement isn't
met the integral algorithm will throw an exception. This is shown as a
diagnostic in the user interface.

The conventions uses some other terms:

* ``lhs`` the first element popped from the stack.
* ``rhs`` the second element popped from the stack.
* ``value`` the first element popped from the stack, for operations using only
  one element.
* ``result`` the result of the operation. The type of the result described is
  what's expected of the engine. How it handles this internally isn't
  specified. That's especially important for overflows. In that case the engine
  might need to do the conversion on ``lhs`` or ``rhs`` prior to the operation.

Named requirements/conversions
==============================

.. _conversion-unmodified:

Unmodified
----------

``int64_t``
  * Returns: unmodified ``int64_t`` value.
``uint64_t``
  * Returns: unmodified ``uint64_t`` value.
``double``
  * Returns: unmodified ``double`` value.

Integral
--------

``int64_t``
  * Returns: unmodified ``int64_t`` value.
``uint64_t``
  * Returns: unmodified ``uint64_t`` value.
``double``
  * Requires: The number has no fractional part.
  * If ``value < 0``

    * Requires: ``value >= INT64_MIN``
    * Returns: ``int64_t`` equivalent of the value.

  * Else

    * Requires: ``value <= UINT64_MAX``
    * Returns: ``uint64_t`` equivalent of the value.

.. _conversion-positive:

Positive integral
-----------------

``int64_t``
  * Requires: ``value > 0``.
  * Returns: ``uint64_t`` equivalent of the value.
``uint64_t``
  * Requires: ``value != 0``.
  * Returns: unmodified ``uint64_t`` value.
``double``
  * Requires: The number has no fractional part.
  * Requires: ``value > 0``
  * Requires: ``value <= UINT64_MAX``
  * Returns: ``uint64_t`` equivalent of the value.

.. _conversion-double:

Double
------

``int64_t``
  * Returns: the value, possible lossy, converted to a ``double``.
``uint64_t``
  * Returns: the value, possible lossy, converted to a ``double``.
``double``
  * Returns: unmodified ``double`` value.


.. _conversion-bitwise:

Bitwise operations cast
-----------------------

``int64_t``
  * Returns: the value converted to an ``uint64_t`` using modulo arithmetic.
``uint64_t``
  * Returns: unmodified ``uint64_t`` value.
``double``
  * Returns: the value bit_casted to an ``uint64_t``. The exact value depends
    on the system's ``double`` representation.

.. _to-storage-int64_t:

Store prefer ``int64_t``
------------------------

Stores the ``result``, when a value can be stored in the range of both an
``int64_t`` and an ``uint64_t`` this version uses the ``int64_t``.

* If ``result >= INT64_MIN && result <= INT64_MAX``:

  * Returns: unmodified ``int64_t`` result.

* Else if ``result <= UINT64_MAX``:

  * Returns: unmodified ``uint64_t`` result.

* Else:

  * Returns: the result, possible lossy, converted to a ``double``.

.. _to-storage-uint64_t:

Store prefer ``uint64_t``
-------------------------

* If ``result >= INT64_MIN && result < 0``:

  * Returns: unmodified ``int64_t`` result.

* Else if ``result <= UINT64_MAX``:

  * Returns: unmodified ``uint64_t`` result.

* Else:

  * Returns: the result, possible lossy, converted to a ``double``.


Arithmetic operations
=====================

Add
---

* If either ``lhs`` or ``rhs`` is a double:

  * ``lhs`` is :ref:`double converted<conversion-double>`.
  * ``rhs`` is :ref:`double converted<conversion-double>`.
  * Returns: a ``double``.

* Else if both ``lhs`` and ``rhs`` are an ``int64_t``:

  * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.
  * ``rhs`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: :ref:`store_prefer_int64_t<to-storage-int64_t>`.

* Else:

  * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.
  * ``rhs`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: :ref:`store_prefer_uint64_t<to-storage-uint64_t>`.

Subtract
--------

* If either ``lhs`` or ``rhs`` is a double:

  * ``lhs`` is :ref:`double converted<conversion-double>`.
  * ``rhs`` is :ref:`double converted<conversion-double>`.
  * Returns: a ``double``.

* Else if both ``lhs`` and ``rhs`` are an ``int64_t``:

  * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.
  * ``rhs`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: :ref:`store_prefer_uint64_t<to-storage-int64_t>`.

* Else:

  * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.
  * ``rhs`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: :ref:`store_prefer_uint64_t<to-storage-uint64_t>`.


Multiply
--------

* If either ``lhs`` or ``rhs`` is a double:

  * ``lhs`` is :ref:`double converted<conversion-double>`.
  * ``rhs`` is :ref:`double converted<conversion-double>`.
  * Returns: a ``double``.

* Else if both ``lhs`` and ``rhs`` are an ``int64_t``:

  * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.
  * ``rhs`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: :ref:`store_prefer_uint64_t<to-storage-int64_t>`.

* Else:

  * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.
  * ``rhs`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: :ref:`store_prefer_uint64_t<to-storage-uint64_t>`.


Divide
------

The division algorithm can be optimized, using the integral results if there's
no fraction. This might be improved later.

* ``lhs`` is :ref:`double converted<conversion-double>`.
* ``rhs`` is :ref:`double converted<conversion-double>`.
* Returns: a ``double``.

Negate
------

* ``value`` is :ref:`unmodified<conversion-unmodified>`.
* Returns: :ref:`store_prefer_uint64_t<to-storage-uint64_t>`.


Modulo
------

* If either ``lhs`` or ``rhs`` is a double:

  * ``lhs`` is :ref:`double converted<conversion-double>`.
  * ``rhs`` is :ref:`double converted<conversion-double>`.
  * Returns: a ``double``.

* Else if both ``lhs`` and ``rhs`` are an ``int64_t``:

  * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.
  * ``rhs`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: :ref:`store_prefer_uint64_t<to-storage-int64_t>`.

* Else if both ``lhs`` and ``rhs`` are an ``unt64_t``:

  * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.
  * ``rhs`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: :ref:`store_prefer_uint64_t<to-storage-int64_t>`.

* Else:

  * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.
  * ``rhs`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: :ref:`store_prefer_uint64_t<to-storage-uint64_t>`.

Quotient
--------

Bitwise logical operations
==========================

Generic
-------

The bitwise operations ``and``, ``or``, and ``xor`` all have the same
conversion behaviour.

* If both ``lhs`` and ``rhs`` are an ``int64_t``:

  * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.
  * ``rhs`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: an ``int64_t``.

Else:

  * ``lhs`` is :ref:`bitwise uint64_t casted<conversion-bitwise>`.
  * ``rhs`` is :ref:`bitwise uint64_t casted<conversion-bitwise>`.
  * Returns: an ``uint64_t``.

Complement
----------

* If ``value`` is an ``int64_t``:

  * ``value`` is :ref:`unmodified<conversion-unmodified>`.
  * Returns: an ``int64_t``.

Else:

  * ``value`` is :ref:`bitwise uint64_t casted<conversion-bitwise>`.
  * Returns: an ``uint64_t``.


Bitwise shifts
==============

The bitwise shift left and shift right have the same conversion behaviour.

* ``lhs``:

  * If an ``int64_t``:

    * ``lhs`` is :ref:`unmodified<conversion-unmodified>`.

  * Else:

    * ``lhs`` is :ref:`bitwise uint64_t casted<conversion-bitwise>`.

* ``rhs``:

  * is :ref:`a positive integral<conversion-positive>`.
  * Requires: ``rhs <= 64``.

* ``result`` the type used for ``lhs``.


Rounding functions
==================

Round
-----

Rounds the value to the nearest integer value. Rounding halfway rounds away
from zero.

* ``value``:

  * is a ``double``

* Returns: a ``double``.

Floor
-----

Returns the value with an integral representation less than or equal to the
original value.


* ``value``:

  * is a ``double``

* Returns: a ``double``.

Ceil
----

Returns the value with an integral representation greater than or equal to the
original value.


* ``value``:

  * is a ``double``

* Returns: a ``double``.

Trunc
----

Returns the value with an integral representation greater where the fractional
part is truncated.


* ``value``:

  * is a ``double``

* Returns: a ``double``.

Power functions
===============

Pow
---

Raises a value to a certain power.


* If either ``lhs`` or ``rhs`` is an integral:

  * ``lhs`` is :ref:`double converted<conversion-double>`.
  * ``rhs`` is :ref:`double converted<conversion-double>`.
  * Returns: a ``double``.

.. note: This in the future the return type may change.
