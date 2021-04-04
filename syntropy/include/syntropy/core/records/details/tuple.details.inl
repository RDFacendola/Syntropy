
/// \file tuple.details.inl
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

namespace Syntropy::Details
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // Concatenate.
    // =========

    template <Records::RecordReference... TRecords>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TRecords>... tuples) noexcept
    {
        auto tuple_cat = [&]<Records::RecordReference TTuple,
                             Int... VTupleIndex,
                             Int... VElementIndex>
             (Forwarding<TTuple> tuple,
              Templates::Sequence<VTupleIndex...>,
              Templates::Sequence<VElementIndex...>)
             {
                 return MakeTuple(
                     Get<VElementIndex>(
                         Get<VTupleIndex>(Forward<TTuple>(tuple)))...);
             };

        return tuple_cat(
            ForwardAsTuple(tuples...),
            EnumerateTupleIndexes<Templates::UnqualifiedOf<TRecords>...>{},
            EnumerateTupleElementIndexes<Templates::UnqualifiedOf<TRecords>...>{});
    }

    // Flatten.
    // ========

    template <Records::RecordReference TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TTuple> tuple) noexcept
    {
        // Flatten each tuple recursively and concatenate the result.

        auto flat = [&]<Int... TIndex>(Forwarding<TTuple> tuple, Templates::Sequence<TIndex...>)
        {
            return Details::Concatenate(Details::Flatten(Get<TIndex>(Forward<TTuple>(tuple)))...);
        };

        return flat(Forward<TTuple>(tuple), Records::SequenceOf<Templates::UnqualifiedOf<TTuple>>{});
    }

    template <typename TElement>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TElement> element) noexcept
    {
        // Wrap the element in a 1-tuple and end recursion.

        return MakeTuple(Forward<TElement>(element));
    }

}

// ===========================================================================
