
/// \file array.h
/// \brief This header is part of the syntropy containers. It contains wrappers for arrays.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <array>

namespace syntropy
{
    /************************************************************************/
    /* ARRAY <TTYPE, KSIZE>                                                 */
    /************************************************************************/

        /// \brief Alias type for std::array.
    template <typename TType, std::size_t kSize>
    using Array = std::array<TType, kSize>;

}
