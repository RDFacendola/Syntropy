
/// \file syntropy.h
/// \brief Common header for Syntropy library.
/// 
/// 
/// 
/// This file has been written considering the following guidelines:
/// 
/// (0) 
/// 
/// 
/// 
/// 
/// (1) No unsigned types.
/// 
///  - Mixing unsigned types with signed integers lead to cluttered code with unnecessary casts.
///  - It's very unlikely that the sign bit is used as a last resort to get extra precision: a larger integer is often a better choice.
///  - Guarding against underflows and overflows is cumbersome and not always straightforward: an array of size "-1" is mathematically wrong (since a set cannot have a negative number of elements),
///      however an array of size static_cast<unsigned int>(-1) ~ 4 billions is only "intuitively" wrong but there's no real check against that condition; unsigned integer subtraction has always to be
///      checked for underflows.
///
/// (2) No double-precision floating point types.
/// 
///  - There's no current plan to support applications that require double-precision.
/// 
/// (3) Strict type semantic.
/// 
///  - Prevent types from mixing with each-other, especially when their underlying type is the same. For example, integers have been separated from fixed-size integer: the former is expected to be
///    used in expression evaluation, the latter as a mean to reduce memory footprint. An explicit conversion is required between the two and even between values of two different fixed-size integers types.
/// 
///  - When evaluating integer expressions, using anything other than the maximum-sized ones has no real performance benefit. Those types are still supported in other parts of the library, where appropriate.
///  - Fixed-size integers are provided to help reduce memory footprint, but shall always be converted to integers when reading their value.
/// 

/// 
/// (4) Everything is immutable unless stated otherwise.
/// 
///  - Inspired from functional languages and other modern languages, constness is enforced everywhere. The naming convention is meant to reduce the effort of writing "immutable" code while moving the saved
///       overhead to the mutable parts. For example: const int readable_ and int writable_ are written as Int readable_ and MutableInt writable_. This sadly cannot be applied to member functions as well.
/// 
/// (5) Conversions between types with different semantic must be explicit. (*)
/// 
///  - Converting from integer to fixed-size integers and between different-sized fixed integers and vice-versa requires explicit conversion. This makes clear when "expanding" the integer into full-width
///       and preventing accidental narrowing conversions.
/// 
/// (6) Be clear but don't be ugly.
/// 
///  - For convenience and aesthetics purposes, casting functions of the form ToXXX(y), where XXX is the target type are provided (ToInt, ToFloat, ToFix8 ...). This reduces
/// 
/// (7)
/// 
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <limits>

namespace Syntropy
{

}