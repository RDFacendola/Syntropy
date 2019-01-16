
/// \file tuple.h
/// \brief This header is part of the syntropy patterns. It contains generic functionalities used to handle tuples.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <type_traits>
#include <utility>

namespace syntropy
{
    /// \brief Apply a function on each element of all the provided tuples at once from the first element to the last. Tuples must have the same rank.
    template <typename TOp, typename... TTuples>
    constexpr void LockstepApply(TOp op, TTuples&&... tuples);

    /// \brief Apply a function on the I-th elements of all the provided tuples at once.
    /// Uses the definition of a generic function "get<I>" to access the I-th element of each tuple.
    template <size_t I, typename TOp, typename... TTuples>
    inline constexpr void LockstepApplyAt(TOp&& op, TTuples&&... tuples);

    /// \brief Provides a member value containing the rank of the elements in TTuples if the rank is the same for each one of them.
    template <typename... TTuples>
    struct LockstepRank {};

    /// \brief Helper constant for LockstepRank.
    template <typename... TTuples>
    constexpr size_t LockstepRankV = LockstepRank<TTuples...>::Value;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    namespace details
    {
        template <typename TOp, typename... TTuples, size_t... Is>
        inline constexpr void LockstepApply(TOp&& op, TTuples&&... tuples, std::index_sequence<Is...>)
        {
            (LockstepApplyAt<Is>(std::forward<TOp>(op), std::forward<TTuples>(tuples)...), ...);
        }
    }

    template <typename TOp, typename... TTuples>
    inline constexpr void LockstepApply(TOp op, TTuples&&... tuples)
    {
        details::LockstepApply<TOp, TTuples...>(std::forward<TOp>(op), std::forward<TTuples>(tuples)..., std::make_index_sequence<LockstepRankV<std::decay_t<TTuples>...>>{});
    }

    template <size_t I, typename TOp, typename... TTuples>
    inline constexpr void LockstepApplyAt(TOp&& op, TTuples&&... tuples)
    {
        using std::get;

        op(get<I>(tuples)...);
    }
}