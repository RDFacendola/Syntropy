
/// \file record.details.h
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/templates.h"

// ===========================================================================

namespace Syntropy::Records::Details
{
    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Helper function for ElementTypeListOf.
    template <typename TRecord,
              template <Int, typename> typename TElementTypeTrait,
              Int... TIndex>
    auto
    ElementTypeListOfHelper(Templates::Sequence<TIndex...>) noexcept
        -> Templates::TypeList<typename TElementTypeTrait<TIndex,
                                                          TRecord>::Type...>;

    /// \brief List of types of a record's elements.
    template <typename TRecord,
              typename TSequence,
              template <Int, typename> typename TElementTypeTrait>
    using
    ElementTypeListOf = decltype(
        ElementTypeListOfHelper<TRecord, TElementTypeTrait>(TSequence{}));

}

// ===========================================================================
