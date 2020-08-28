
/// \file initializer_list.h
/// \brief This header is part of the Syntropy language module. It contains type aliases for initializer list.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <initializer_list>

namespace Syntropy
{
    /************************************************************************/
    /* TYPE ALIAS                                                           */
    /************************************************************************/

    // Type alias for a lightweight proxy object that provides access to an array of objects of type const TElement.
    template <typename TElement>
    using InitializerList = std::initializer_list<TElement>;

}
