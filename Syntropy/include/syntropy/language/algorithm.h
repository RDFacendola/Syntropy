
/// \file algorithm.h
/// \brief This header is part of the Syntropy language module. It contains extensions to standard algorithm header.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <initializer_list>
#include <algorithm>

#include "syntropy/memory/byte_span.h"

namespace syntropy
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Rotate the order of the element in a range, in such a way that the element pointed by middle becomes the new first element.
    /// If middle doesn't belong to range, the result of this method is undefined.
    template <typename TElement>
    void Rotate(const RWSpan<TElement>& span, Pointer<TElement> middle);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TElement>
    inline void Rotate(const RWSpan<TElement>& span, Pointer<TElement> middle)
    {
        std::rotate(Begin(span), middle, End(span));
    }

}
