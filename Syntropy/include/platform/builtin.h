
/// \file builtin.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains classes and definitions used to access platform builtin functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>

namespace syntropy
{
    namespace platform
    {
        /// \brief Exposes platform built-in functionalities.
        /// \author Raffaele D. Facendola - December 2016
        class BuiltIn
        {
        public:

            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance;
            static BuiltIn& GetInstance();

            /// \brief Get the index of the most significant bit set.
            /// \return Returns the index of the most significant bit set. Undefined behavior if 0.
            virtual size_t GetMostSignificantBit(uint64_t number) const = 0;

        };

        /// \brief Get the current system instance.
        /// \brief Returns the current system instance.
        BuiltIn& GetBuiltIn();
    }
}
