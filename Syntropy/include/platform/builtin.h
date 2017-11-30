
/// \file builtin.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains classes and definitions used to access platform builtin functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>

namespace syntropy::platform
{
    /// \brief Exposes platform built-in functionalities.
    /// \author Raffaele D. Facendola - December 2016
    class BuiltIn
    {
    public:

        /// \brief Get the index of the most significant bit set.
        /// \return Returns the index of the most significant bit set. Undefined behavior if number is 0.
        static uint64_t GetMostSignificantBit(uint64_t number);

        /// \brief Get the index of the least significant bit set.
        /// \return Returns the index of the least significant bit set. Undefined behavior if number is 0.
        static uint64_t GetLeastSignificantBit(uint64_t number);

    };
}
