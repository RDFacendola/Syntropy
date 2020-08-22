
/// \file language.h
/// \brief This header is part of Syntropy language module. It contains fundamental definitions of types.
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

}