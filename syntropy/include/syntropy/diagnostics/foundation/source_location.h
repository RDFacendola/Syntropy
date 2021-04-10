
/// \file source_location.h
///
/// \brief This header is part of the Syntropy diagnostic module.
///        It contains definitions for source-code locations.
///
/// \author Raffaele D. Facendola - April 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/strings/string.h"

// ===========================================================================

namespace Syntropy::Diagnostics
{
    /************************************************************************/
    /* MACROS                                                               */
    /************************************************************************/

    /// \brief Expands to an object representing the location of the current
    ///        line of code.
    /// \remarks This macro is deprecated and soon will be replaced by
    ///          a function to accomodate C++20 features.
    #define SYNTROPY_HERE \
        SYNTROPY_MACRO_DECLARATION(empty)

    /************************************************************************/
    /* SOURCE LOCATION                                                      */
    /************************************************************************/

    /// \brief Represents a collection of information about a particular
    ///        source code location.
    /// \author Raffaele D. Facendola - April 2021
    class SourceLocation
    {
    public:

        /// \brief C-array of characters.
        template <typename TCharacter, Int TSize>
        using StringLiteralType = TCharacter(&)[TSize];

        /// \brief Default constructor.
        SourceLocation() noexcept = default;

        /// \brief Direct constructor.
        template <typename TCharacter, Int TSize, Int USize>
        SourceLocation(StringLiteralType<TCharacter, TSize> file_name,
                       StringLiteralType<TCharacter, USize> function_name,
                       Int line)
        noexcept;

        /// \brief Direct constructor.
        SourceLocation(String file_name, String function_name, Int line)
        noexcept;

        /// \brief Default copy-constructor.
        SourceLocation(Immutable<SourceLocation> other) noexcept = default;

        /// \brief Default move-constructor.
        SourceLocation(Movable<SourceLocation> other) noexcept = default;

        /// \brief Default copy-assignment operator.
        Mutable<SourceLocation>
        operator=(Immutable<SourceLocation> other) noexcept = default;

        /// \brief Default move-assignment operator.
        // Mutable<SourceLocation>
        //operator=(Movable<SourceLocation> other) noexcept = default;

        /// \brief Default destructor.
        ~SourceLocation() noexcept = default;

        /// \brief Get the file name.
        Immutable<String> GetFileName() const noexcept;

        /// \brief Get the function name.
        Immutable<String> GetFunctionName() const noexcept;

        /// \brief Get the code line.
        Int GetLine() const noexcept;

        /// \brief Swap this instance with another one.
        void Swap(Mutable<SourceLocation> other) noexcept;

    private:

        /// \brief File name.
        String file_name_;

        /// \brief Function name.
        String function_name_;

        /// \brief Code line.
        Int line_{ 0 };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/


}

// ===========================================================================

#include "details/source_location.inl"

// ===========================================================================
