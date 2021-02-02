
/// \file enumerations.h
/// \brief This header is part of Syntropy language module.
///        It contains definitions for enumeration types.
///
/// \author Raffaele D. Facendola - December 2020

#pragma once

#include <cstdint>
#include <cstddef>

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* ENUMERATION TYPES                                                    */
    /************************************************************************/

    // Fixed underlying types for enumeration classes.
    // For fixed-size integers see Fix8, Fix16, Fix32, Fix64.

    /// \brief 8-bit enum class type.
    /// \usage enum class MyEnum : Enum8 { ... };
    using Enum8 = std::int8_t;

    /// \brief 16-bit enum class type.
    /// \usage enum class MyEnum : Enum16 { ... };
    using Enum16 = std::int16_t;

    /// \brief 32-bit enum class type.
    /// \usage enum class MyEnum : Enum32 { ... };
    using Enum32 = std::int32_t;

    /// \brief 64-bit enum class type.
    /// \usage enum class MyEnum : Enum64 { ... };
    using Enum64 = std::int64_t;

}

// ===========================================================================
