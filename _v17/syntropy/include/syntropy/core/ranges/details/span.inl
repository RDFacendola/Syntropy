
/// \file span.inl
///
/// \author Raffaele D. Facendola - June 2020.

// ===========================================================================

#include "syntropy/core/ranges/contiguous_range.h"
#include "syntropy/language/support/view.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* BASE SPAN                                                            */
    /************************************************************************/

    template <typename TType>
    constexpr BaseSpan<TType>
    ::BaseSpan(Null) noexcept
    {

    }

    template <typename TType>
    constexpr BaseSpan<TType>
    ::BaseSpan(BaseSpan<TType>::PointerType begin, Int count)
                   noexcept
        : data_(begin)
        , count_(count)
    {

    }

    template <typename TType>
    constexpr BaseSpan<TType>
    ::BaseSpan(BaseSpan<TType>::PointerType begin,
               BaseSpan<TType>::PointerType end) noexcept
        : BaseSpan(begin, ToInt(end - begin))
    {

    }

    template <typename TType>
    template <typename UType>
    constexpr BaseSpan<TType>
    ::BaseSpan(Immutable<BaseSpan<UType>> rhs) noexcept
        : data_(ToPtr<TType>(rhs.data_))
        , count_(rhs.count_)
    {

    }

    template <typename TType>
    template <typename UType>
    constexpr Mutable<BaseSpan<TType>> BaseSpan<TType>
    ::operator=(Immutable<BaseSpan<UType>> rhs) noexcept
    {
        data_ = ToPtr<TType>(rhs.data_);
        count_ = rhs.count_;

        return *this;
    }

    template <typename TType>
    [[nodiscard]] constexpr BaseSpan<TType>
    ::operator Bool() const noexcept
    {
        return count_ > ToInt(0);
    }

    template <typename TType>
    [[nodiscard]] constexpr
    typename BaseSpan<TType>::ReferenceType
    BaseSpan<TType>
    ::operator[](Int index)
    const noexcept
    {
        return data_[ToInt(index)];
    }

    template <typename TType>
    [[nodiscard]] constexpr
    typename BaseSpan<TType>::PointerType BaseSpan<TType>
    ::GetData() const noexcept
    {
        return data_;
    }

    template <typename TType>
    [[nodiscard]] constexpr Int
    BaseSpan<TType>
    ::GetCount() const noexcept
    {
        return count_;
    }

    template <typename TType>
    [[nodiscard]] constexpr
    BaseSpan<TType>
    BaseSpan<TType>
    ::Select(Int offset, Int count) const noexcept
    {
        return { data_ + offset, count };
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // BaseSpan.
    // =========

    template <IsSpan TSpan, IsSpan USpan>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<TSpan> lhs, Immutable<USpan> rhs) noexcept
    {
        return Ranges::AreEquivalent(lhs, rhs);
    }

    template <IsSpan TSpan, IsSpan USpan>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<TSpan> lhs, Immutable<USpan> rhs) noexcept
    {
        return Ranges::Compare(lhs, rhs);
    }

    // Access.
    // =======

    template <typename TType>
    [[nodiscard]] constexpr Span<TType>
    ToReadOnly(Immutable<BaseSpan<TType>> rhs) noexcept
    {
        return { Support::ToReadOnly(rhs.GetData()), rhs.GetCount() };
    }

    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType>
    ToReadWrite(Immutable<BaseSpan<TType>> rhs) noexcept
    {
        return { Support::ToReadWrite(rhs.GetData()), rhs.GetCount() };
    }

    // Utilities.
    // ==========

    template <typename TType, typename TCount>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Immutable<TCount> count) noexcept
    {
        return { begin, count };
    }

    template <typename TType>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Ptr<TType> end) noexcept
    {
        return { begin, end };
    }

    template <typename TType, typename TCount>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, Immutable<TCount> count) noexcept
    {
        return { begin, count };
    }

    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, RWPtr<TType> end) noexcept
    {
        return { begin, end };
    }

    template <typename TType, Int VCount>
    [[nodiscard]] constexpr auto
    MakeSpan(TType(&rhs)[VCount]) noexcept
    {
        return MakeSpan(PtrOf(rhs[0]), VCount);
    }

}

// ===========================================================================
