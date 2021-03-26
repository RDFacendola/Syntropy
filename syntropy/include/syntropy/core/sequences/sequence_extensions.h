
/// \file sequence_extensions.h
///
/// \brief This header is part of the Syntropy core module.
///        It contains definitions for sequence-related customization-point
///        objects.
///
/// \author Raffaele D. Facendola - March 2021.

#pragma once

// ===========================================================================

namespace Syntropy::Sequences::Extensions
{
    /************************************************************************/
    /* SEQUENCE EXTENSIONS                                                  */
    /************************************************************************/

    /// \brief Access a sequence element by index.
    template <Int TIndex, typename TSequence, typename = void>
    struct Get;

}

// ===========================================================================
