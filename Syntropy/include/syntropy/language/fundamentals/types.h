
/// \file types.h
/// \brief This header is part of Syntropy language module. It contains fundamental definitions of types.
///
/// This header must define only types and shall not depend on any other header other than standards'.
/// 
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <cstdint>
#include <cstddef>

namespace Syntropy
{
    /************************************************************************/
    /* FUNDAMENTAL TYPES                                                    */
    /************************************************************************/

    /// \brief Boolean value.
    /// \remarks Standard booleans bind also to numeric values.
    using Bool = bool;

    /// \brief 64-bit integer value.
    using Int = std::int64_t;

    /// \brief 32-bit floating point value.
    using Float = float;

    /// \brief Type alias for a non-owning pointer which doesn't participate to pointee life-time.
    template <typename TType>
    using PointerT = TType*;

    /// \brief Type alias for a lvalue reference to an object.
    template <typename TType>
    using ReferenceT = TType&;

    /// \brief Type of the null pointer literal (nullptr).
    using Null = std::nullptr_t;

    /************************************************************************/
    /* FIXED-SIZE TYPES                                                     */
    /************************************************************************/

    /// \brief 8-bit integer value.
    enum class Fix8 : std::int8_t {};

    /// \brief 16-bit integer value.
    enum class Fix16 : std::int16_t {};

    /// \brief 32-bit integer value.
    enum class Fix32 : std::int32_t {};

    /// \brief 64-bit integer value.
    enum class Fix64 : std::int64_t {};

}