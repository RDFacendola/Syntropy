
/// \file to_string_extensions.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for string-related customization point
///        objects.
///
/// \author Raffaele D. Facendola - April 2021

// ===========================================================================

#pragma once

// ===========================================================================

namespace Syntropy::Strings::Extensions
{
    /************************************************************************/
    /* STRING EXTENSIONS                                                    */
    /************************************************************************/

    /// \brief Convert an object to a string.
    template <typename TType, typename = void>
    struct ToString;

}

// ===========================================================================
