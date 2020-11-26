# Syntropy Language - DRAFT

Syntropy language module is meant to provide definitions for fundamental types, language machinery and easy-to-integrate paradigms.

## Fundamental types

C++ offers a lot of fundamental types: signed integer values, unsigned integer values, single-precision floats and so on.
Restricting the application of C++ to interactive simulations, most of those types are unnecessary ad may cause confusion.

Syntropy pins down the most common types without ambiguities:

1. Bool: A boolean value, either true or false.
2. Int: A signed-integer value. This type is the largest integer on a platform.
3. Float: A signed-real value represented by 32-bit floating point format.

## Read-only values and Read-write values.

Unlike other higher-level languages, C++ has the concept of "const" and "non-const" values, objects and methods.
Those are meant to reduce what can be performed on them and how they can be propagated.

In Syntropy, read-only values are used basically everywhere: only a few of the interfaces provided deal with read-write values.
To "shield" a value from being modifies, the utility function ReadOnly will convert any value to its read-only counterpart.
It is also possible to convert a read-only value to a read-write couterpart, however note that in C++, writing to a read-only memory
location results in undefined behavior. That, of course, is valid also for Syntropy. The ReadWrite method is intended to avoid
code duplication when writing a method for both the read-only and the read-write version of the object. Only in this case
ReadWrite can be used to write the non-const implementation in terms of the read-only one and casting away the constness of the result.

Many template types come in different aliases: one for its generic implementation, one for the read-only version and the last for the read-write one.

Usually the generic implementation starts with the suffix "T", the read-write version has the "RW" prefix and read-only has no prefix at all
(since they are considered the standard for Syntropy).

For pointers, for example

1. PointerT<T> is the generic alias.
2. RWPointer<T> is the read-write alias.
3. Pointer<T> is the read-only alias.

### Rationale

One of the prerogatives of Syntropy is to provide a clear path when writing interactive applications, therefore it supports only
the minimal set of types in the language. This has also reduce the amount of code in the code-base by simpling ignoring them.
Removing all types deemed superfluous is not really possible, especially when working with standard features, therefore from time to time
it may happen that those types are used directly. In these cases, the usage of those types is transparent and they are never propagated.

Mixing signed integers and unsigned integer results either in compiler warnings (if lucky) or unexpected values as negative values
wrap around, producing huge numbers.
For the most, unsigned types, are naturally fit for those quantities whose negative values have no real meaning (a distance, for example).
However, performing arithmetic operations on those values and mixing them with signed integer results in cumbersome expressions using
casts and branches, reducing readability and maintenance cost. For this reason unsigned values have been discarded by Syntropy.

Booleans in C++ are considered arithmetic types so they will bind to integers. This may cause ambiguities between
different overloads where a boolean and an integer are expected. In this case there's no real way of solving the problem without
introducing custom types (such an enum class), but that requires turning the language upside-down, which seems a little too much.

Floating values, other than single-precision, are really not needed in the context of interactive applications, therefore no other type
rather than float is supported.

C++ has literally dozen of char-like types, but those require a chapter on their own therefore we'll skip that for now.

### Rationals

Wrapping <ratio> won't save much work.

### Naming convention

Constants kBlah
Template typenames T[N]Whatever after first (T if single, after T0 T1 T2 ...)
Template values VWhatever after first (V if single, after V0 V1 V2 ...)
