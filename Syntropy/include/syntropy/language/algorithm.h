
/// \file algorithm.h
/// \brief This header is part of the Syntropy language module. It contains extensions to standard algorithm header.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <initializer_list>
#include <algorithm>

#include "syntropy/memory/memory_span.h"

namespace syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Rotate the order of the element in a range, in such a way that the element pointed by middle becomes the new first element.
    /// If middle doesn't belong to range, the result of this method is undefined.
    void Rotate(const MemorySpan& span, BytePtr middle);

    /// \brief Reduce the source range from the front until the first element is equal to the provided element or the range is exhausted.
    MemorySpan Find(const MemorySpan& source, Byte element);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline void Rotate(const MemorySpan& span, BytePtr middle)
    {
        std::rotate(Begin(span), middle, End(span));
    }

    inline MemorySpan Find(const MemorySpan& span, Byte element)
    {
        auto result = span;

        for (; result && (Front(result) != element); result = PopFront(result));

        return result;
    }

}
