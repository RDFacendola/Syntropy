
/// \file new.h
/// \brief This header is part of the Syntropy memory module. It contains custom variants of both new\delete operators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/byte_span.h"

/************************************************************************/
/* OPERATOR NEW                                                         */
/************************************************************************/

/// \brief Allocate requested number of bytes inside byte span.
/// If the byte span could not fit the requested size, returns nullptr.
Syntropy::RWTypelessPtr operator new(std::size_t size, const Syntropy::RWByteSpan& byte_span);

/************************************************************************/
/* IMPLEMENTATION                                                       */
/************************************************************************/

// Operator New.
// =============

inline Syntropy::RWTypelessPtr operator new(std::size_t size, const Syntropy::RWByteSpan& byte_span)
{
    if (Syntropy::ToInt(size) <= Syntropy::ToInt(Syntropy::Memory::Size(byte_span)))
    {
        return Begin(byte_span);
    }

    return nullptr;
}

