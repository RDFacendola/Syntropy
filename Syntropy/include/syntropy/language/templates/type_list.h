
/// \file type_list.h
/// \brief This header is part of Syntropy language module. It contains definitions for type lists.
/// 
/// \author Raffaele D. Facendola - August  2020

#pragma once

namespace Syntropy
{
    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief An ordered sequence of types.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename... TTyes>
    struct TypeList {};
}