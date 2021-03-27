
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

    template <Records::ForwardingRecord... TRecords>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TRecords>... tuples) noexcept
    {
        auto tuple_cat = [&]<Records::ForwardingRecord TTuple,
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

    template <Records::ForwardingRecord TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TTuple> tuple) noexcept
    {
        // The argument is a tuple: flatten each element recursively and
        // eturn their concatenation.

        auto flat = [&]<Int... VTupleIndex>(
            Forwarding<TTuple> tuple,
            Templates::Sequence<VTupleIndex...>)
        {
            return Details::Concatenate(
                Details::Flatten(Get<VTupleIndex>(Forward<TTuple>(tuple)))...);
        };

        return flat(Forward<TTuple>(tuple),
                    Records::SequenceOf<Templates::UnqualifiedOf<TTuple>>{});
    }

    template <typename TElement>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TElement> element) noexcept
    {
        // The argument is not a tuple: wrap it in a 1-tuple and end recursion.

        return MakeTuple(Forward<TElement>(element));
    }

}

// ===========================================================================
