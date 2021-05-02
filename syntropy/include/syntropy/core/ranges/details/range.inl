
/// \file range.inl
///
/// \author Raffaele D. Facendola - March 2021.

// ===========================================================================

namespace Syntropy::RangesADL
{
    /************************************************************************/
    /* RANGE EXTENSIONS                                                     */
    /************************************************************************/

    // Type aliases.
    // =============

    /// \brief Type alias for the ViewOf extension functor.
    template <typename TRange>
    using ViewOfExtension
        = Ranges::Extensions::ViewOf<TRange>;

    /// \brief Type alias for the IsEmpty extension functor.
    template <typename TRange>
    using IsEmptyExtension
        = Ranges::Extensions::IsEmpty<TRange>;

    /// \brief Type alias for the Count extension functor.
    template <typename TRange>
    using CountExtension
        = Ranges::Extensions::Count<TRange>;

    /// \brief Type alias for the Select extension functor.
    template <typename TRange>
    using SelectExtension
        = Ranges::Extensions::Select<TRange>;

    /// \brief Type alias for the Front extension functor.
    template <typename TRange>
    using FrontExtension
        = Ranges::Extensions::Front<TRange>;

    /// \brief Type alias for the Back extension functor.
    template <typename TRange>
    using BackExtension
        = Ranges::Extensions::Back<TRange>;

    /// \brief Type alias for the PopFront extension functor.
    template <typename TRange>
    using PopFrontExtension
        = Ranges::Extensions::PopFront<TRange>;

    /// \brief Type alias for the PopBack extension functor.
    template <typename TRange>
    using PopBackExtension
        = Ranges::Extensions::PopBack<TRange>;

    /// \brief Type alias for the At extension functor.
    template <typename TRange>
    using AtExtension
        = Ranges::Extensions::At<TRange>;

    /// \brief Type alias for the Data extension functor.
    template <typename TRange>
    using DataExtension
        = Ranges::Extensions::Data<TRange>;

    // InvokeExtension.
    // ================

    /// \brief Invoke the ViewOf function via extension functor.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    InvokeViewOfExtension(Forwarding<TRange> rhs)
        noexcept -> decltype(ViewOfExtension<TRange>{}(Forward<TRange>(rhs)))
    {
        return ViewOfExtension<TRange>{}(Forward<TRange>(rhs));
    }

    /// \brief Invoke the IsEmpty function via extension functor.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    InvokeIsEmptyExtension(Immutable<TRange> rhs)
        noexcept -> decltype(IsEmptyExtension<TRange>{}(rhs))
    {
        return IsEmptyExtension<TRange>{}(rhs);
    }

    /// \brief Invoke the Count function via extension functor.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    InvokeCountExtension(Immutable<TRange> rhs)
        noexcept -> decltype(CountExtension<TRange>{}(rhs))
    {
        return CountExtension<TRange>{}(rhs);
    }

    /// \brief Invoke the Select function via extension functor.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    InvokeSelectExtension(Immutable<TRange> rhs, Int offset, Int count)
        noexcept -> decltype(SelectExtension<TRange>{}(rhs, offset, count))
    {
        return SelectExtension<TRange>{}(rhs, offset, count);
    }

    /// \brief Invoke the Front function via extension functor.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    InvokeFrontExtension(Immutable<TRange> rhs)
        noexcept -> decltype(FrontExtension<TRange>{}(rhs))
    {
        return FrontExtension<TRange>{}(rhs);
    }

    /// \brief Invoke the Back function via extension functor.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    InvokeBackExtension(Immutable<TRange> rhs)
        noexcept -> decltype(BackExtension<TRange>{}(rhs))
    {
        return BackExtension<TRange>{}(rhs);
    }

    /// \brief Invoke the PopFront function via extension functor.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    InvokePopFrontExtension(Immutable<TRange> rhs)
        noexcept -> decltype(PopFrontExtension<TRange>{}(rhs))
    {
        return PopFrontExtension<TRange>{}(rhs);
    }

    /// \brief Invoke the PopBack function via extension functor.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    InvokePopBackExtension(Immutable<TRange> rhs)
        noexcept -> decltype(PopBackExtension<TRange>{}(rhs))
    {
        return PopBackExtension<TRange>{}(rhs);
    }

    /// \brief Invoke the At function via extension functor.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    InvokeAtExtension(Immutable<TRange> rhs, Int index)
        noexcept -> decltype(AtExtension<TRange>{}(rhs, index))
    {
        return AtExtension<TRange>{}(rhs);
    }

    /// \brief Invoke the Data function via extension functor.
    template <typename TRange>
    [[nodiscard]] constexpr auto
    InvokeDataExtension(Immutable<TRange> rhs)
        noexcept -> decltype(DataExtension<TRange>{}(rhs))
    {
        return DataExtension<TRange>{}(rhs);
    }

    /************************************************************************/
    /* RANGES                                                               */
    /************************************************************************/

    // ViewOf.
    // =======

    /// \brief Invoke the ViewOf function, trying different implementations.
    template <typename TRange>
    constexpr auto
    InvokeViewOf(Forwarding<TRange> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeViewOfExtension(Forward<TRange>(rhs)))
        {
            return InvokeViewOfExtension(Forward<TRange>(rhs));
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.GetView())
        {
            return rhs.GetView();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(ViewOf(Forward<TRange>(rhs)))
        {
            return ViewOf(Forward<TRange>(rhs));
        };

        auto fallback = [](auto&& rhs)
        {
            return rhs;
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function,
                                  fallback)(Forward<TRange>(rhs));
    }

    // IsEmpty.
    // ========

    /// \brief Invoke the IsEmpty function, trying different implementations.
    template <typename TRange>
    constexpr decltype(auto)
    InvokeIsEmpty(Immutable<TRange> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeIsEmptyExtension(rhs))
        {
            return InvokeIsEmptyExtension(rhs);
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.IsEmpty())
        {
            return rhs.IsEmpty();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(IsEmpty(rhs))
        {
            return IsEmpty(rhs);
        };

        auto fallback = [](auto&& rhs)
            -> decltype(Ranges::Count(rhs) == ToInt(0))
        {
            return Ranges::Count(rhs) == ToInt(0);
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function,
                                  fallback)(rhs);
    }

    // Count.
    // ======

    /// \brief Invoke the Count function, trying different implementations.
    template <typename TRange>
    constexpr decltype(auto)
    InvokeCount(Immutable<TRange> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeCountExtension(rhs))
        {
            return InvokeCountExtension(rhs);
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.GetCount())
        {
            return rhs.GetCount();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(Count(rhs))
        {
            return Count(rhs);
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function)(rhs);
    }

    // Select.
    // =======

    /// \brief Invoke the Select function, trying different implementations.
    template <typename TRange>
    constexpr decltype(auto)
    InvokeSelect(Immutable<TRange> rhs, Int offset, Int count) noexcept
    {
        auto extension = [](auto&& rhs, auto&& offset, auto&& count)
            -> decltype(InvokeSelectExtension(rhs, offset, count))
        {
            return InvokeSelectExtension(rhs);
        };

        auto member_function = [](auto&& rhs, auto&& offset, auto&& count)
            -> decltype(rhs.Select(offset, count))
        {
            return rhs.Select(offset, count);
        };

        auto non_member_function = [](auto&& rhs, auto&& offset, auto&& count)
            -> decltype(Select(rhs, offset, count))
        {
            return Select(rhs, offset, count);
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function)(rhs, offset, count);
    }

    // Front.
    // ======

    /// \brief Invoke the Front function, trying different implementations.
    template <typename TRange>
    constexpr decltype(auto)
    InvokeFront(Immutable<TRange> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeFrontExtension(rhs))
        {
            return InvokeFrontExtension(rhs);
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.GetFront())
        {
            return rhs.GetFront();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(Front(rhs))
        {
            return Front(rhs);
        };

        auto fallback = [](auto&& rhs)
            -> decltype(Ranges::At(rhs, ToInt(0)))
        {
            return Ranges::At(rhs, ToInt(0));
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function,
                                  fallback)(rhs);
    }

    // Back.
    // =====

    /// \brief Invoke the Back function, trying different implementations.
    template <typename TRange>
    constexpr decltype(auto)
    InvokeBack(Immutable<TRange> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeBackExtension(rhs))
        {
            return InvokeBackExtension(rhs);
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.GetBack())
        {
            return rhs.GetBack();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(Back(rhs))
        {
            return Back(rhs);
        };

        auto fallback = [](auto&& rhs)
            -> decltype(Ranges::At(rhs, Ranges::Count(rhs) - ToInt(1)))
        {
            return Ranges::At(rhs, Ranges::Count(rhs) - ToInt(1));
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function,
                                  fallback)(rhs);
    }

    // PopFront.
    // =========

    /// \brief Invoke the PopFront function, trying different implementations.
    template <typename TRange>
    constexpr decltype(auto)
    InvokePopFront(Immutable<TRange> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokePopFrontExtension(rhs))
        {
            return InvokePopFrontExtension(rhs);
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.PopFront())
        {
            return rhs.PopFront();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(PopFront(rhs))
        {
            return PopFront(rhs);
        };

        auto fallback = [](auto&& rhs)
            -> decltype(Ranges::Select(rhs,
                                       ToInt(1),
                                       Ranges::Count(rhs) - ToInt(1)))
        {
            return Ranges::Select(rhs,
                                  ToInt(1),
                                  Ranges::Count(rhs) - ToInt(1));
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function,
                                  fallback)(rhs);
    }

    // PopBack.
    // ========

    /// \brief Invoke the PopBack function, trying different implementations.
    template <typename TRange>
    constexpr decltype(auto)
    InvokePopBack(Immutable<TRange> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokePopBackExtension(rhs))
        {
            return InvokePopBackExtension(rhs);
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.PopBack())
        {
            return rhs.PopBack();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(PopBack(rhs))
        {
            return PopBack(rhs);
        };

        auto fallback = [](auto&& rhs)
            -> decltype(Ranges::Select(rhs,
                                       ToInt(0),
                                       Ranges::Count(rhs) - ToInt(1)))
        {
            return Ranges::Select(rhs,
                                  ToInt(0),
                                  Ranges::Count(rhs) - ToInt(1));
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function,
                                  fallback)(rhs);
    }

    // At.
    // ==

    /// \brief Invoke the At function, trying different implementations.
    template <typename TRange>
    constexpr decltype(auto)
    InvokeAt(Immutable<TRange> rhs, Int index) noexcept
    {
        auto extension = [](auto&& rhs, auto&& index)
            -> decltype(InvokeAtExtension(rhs, index))
        {
            return InvokeAtExtension(rhs, index);
        };

        auto member_function = [](auto&& rhs, auto&& index)
            -> decltype(rhs.At(index))
        {
            return rhs.At(index);
        };

        auto member_operator = [](auto&& rhs, auto&& index)
            -> decltype(rhs[index])
        {
            return rhs[index];
        };

        auto non_member_function = [](auto&& rhs, auto&& index)
            -> decltype(At(rhs, index))
        {
            return At(rhs, index);
        };

        auto fallback = [](auto&& rhs, auto&& index)
            -> decltype(Ranges::Data(rhs)[index])
        {
            return Ranges::Data(rhs)[index];
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  member_operator,
                                  non_member_function,
                                  fallback)(rhs, index);
    }

    // Data.
    // =====

    /// \brief Invoke the Data function, trying different implementations.
    template <typename TRange>
    constexpr decltype(auto)
    InvokeData(Immutable<TRange> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeDataExtension(rhs))
        {
            return InvokeDataExtension(rhs);
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.GetData())
        {
            return rhs.GetData();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(Data(rhs))
        {
            return Data(rhs);
        };

        return Support::InvokeAny(extension,
                                  member_function,
                                  non_member_function)(rhs);
    }

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RANGES                                                               */
    /************************************************************************/

    template <typename TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::ViewOf(Forwarding<TRange> range) noexcept
    {
        return RangesADL::InvokeViewOf(Forward<TRange>(range));
    }

    template <typename TRange>
    [[nodiscard]] constexpr Bool
    Ranges
    ::IsEmpty(Forwarding<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(Forward<TRange>(range));

        return RangesADL::InvokeIsEmpty(range_view);
    }

    template <typename TRange>
    [[nodiscard]] constexpr Int
    Ranges
    ::Count(Forwarding<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(Forward<TRange>(range));

        return RangesADL::InvokeCount(range_view);
    }

    template <typename TRange>
    [[nodiscard]] constexpr decltype(auto)
    Ranges
    ::Front(Forwarding<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(Forward<TRange>(range));

        return RangesADL::InvokeFront(range_view);
    }

    template <typename TRange>
    [[nodiscard]] constexpr decltype(auto)
    Ranges
    ::Back(Forwarding<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(Forward<TRange>(range));

        return RangesADL::InvokeBack(range_view);
    }

    template <typename TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::PopFront(Forwarding<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(Forward<TRange>(range));

        return RangesADL::InvokePopFront(range_view);
    }

    template <typename TRange>
    [[nodiscard]] constexpr auto
    Ranges
    ::PopBack(Forwarding<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(Forward<TRange>(range));

        return RangesADL::InvokePopBack(range_view);
    }

    template <typename TRange>
    [[nodiscard]] constexpr decltype(auto)
    Ranges
    ::At(Forwarding<TRange> range, Int index) noexcept
    {
        auto range_view = Ranges::ViewOf(Forward<TRange>(range));

        return RangesADL::InvokeAt(range_view, index);
    }

    template <typename TRange>
    [[nodiscard]] constexpr decltype(auto)
    Ranges
    ::Select(Forwarding<TRange> range, Int offset, Int count) noexcept
    {
        auto range_view = Ranges::ViewOf(Forward<TRange>(range));

        return RangesADL::InvokeSelect(range_view, offset, count);
    }

    template <typename TRange>
    [[nodiscard]] constexpr decltype(auto)
    Ranges
    ::Data(Forwarding<TRange> range) noexcept
    {
        auto range_view = Ranges::ViewOf(Forward<TRange>(range));

        return RangesADL::InvokeData(range_view);
    }

}

// ===========================================================================
