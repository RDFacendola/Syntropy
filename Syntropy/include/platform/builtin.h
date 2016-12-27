
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

            /// \brief Get the number of leading zeroes in a 64 bit unsigned number.
            /// \param number Number to count the leading zeroes of.
            /// \return Returns the number of leading zeroes in the provided number.
            virtual size_t GetLeadingZeroes(uint64_t number) const = 0;

        };

        /// \brief Get the current system instance.
        /// \brief Returns the current system instance.
        BuiltIn& GetBuiltIn();
    }
}
