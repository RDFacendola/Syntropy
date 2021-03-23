
/// \file tuple.details.inl
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

namespace Syntropy::Sequences::Details
{
    /************************************************************************/
    /* TUPLE                                                                */
    /************************************************************************/

    // Concatenate.
    // =========

    template <Concepts::NTupleReference... TSequences>
    [[nodiscard]] constexpr decltype(auto)
    Concatenate(Forwarding<TSequences>... tuples) noexcept
    {
        auto tuple_cat = [&]<Concepts::NTupleReference TTuple,
                             Int... VTupleIndex,
                             Int... VElementIndex>
             (Forwarding<TTuple> tuple,
              Syntropy::Templates::Sequence<VTupleIndex...>,
              Syntropy::Templates::Sequence<VElementIndex...>)
             {
                 return MakeTuple(
                     Get<VElementIndex>(
                         Get<VTupleIndex>(Forward<TTuple>(tuple)))...);
             };

        return tuple_cat(
            ForwardAsTuple(tuples...),
            EnumerateTupleIndexes<Syntropy::Templates::UnqualifiedOf<TSequences>...>{},
            EnumerateTupleElementIndexes<Syntropy::Templates::UnqualifiedOf<TSequences>...>{});
    }

    // Flatten.
    // ========

    template <Concepts::NTupleReference TTuple>
    [[nodiscard]] constexpr decltype(auto)
    Flatten(Forwarding<TTuple> tuple) noexcept
    {
        // The argument is a tuple: flatten each element recursively and
        // eturn their concatenation.

        using Syntropy::Sequences::Concatenate;

        auto flat = [&]<Int... VTupleIndex>(
            Forwarding<TTuple> tuple,
            Syntropy::Templates::Sequence<VTupleIndex...>)
        {
            return Concatenate(
                Details::Flatten(Get<VTupleIndex>(Forward<TTuple>(tuple)))...);
        };

        return flat(Forward<TTuple>(tuple),
                    Templates::TupleSequenceFor<
                        Syntropy::Templates::UnqualifiedOf<TTuple>>{});
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
