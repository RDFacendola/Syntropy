
/// \file memory.h
///
/// \brief This header is part of the Syntropy memory module.
///        It contains low-level memory functionalities.
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Copy a source memory region to a destination memory region.
    ///        Neither span is exceed during the process.
    ///
    /// \return Returns the bytes copied as a result of this call.
    Bytes
    Copy(Immutable<RWByteSpan> destination, Immutable<ByteSpan> source);

    /// \brief Copy a source memory region to a destination memory region
    ///        repeating the source until destination span is exhausted.
    ///        Neither span is exceed during the process.
    void
    Repeat(Immutable<RWByteSpan> destination, Immutable<ByteSpan> source);

    /// \brief Set a value to each byte in a destination span.
    void
    Set(Immutable<RWByteSpan> destination, Byte value);

    /// \brief Zero-out a memory region.
    void
    Zero(Immutable<RWByteSpan> destination);
    
}

// ===========================================================================

#include "memory.inl"

// ===========================================================================
