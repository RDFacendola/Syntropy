
/// \file range.h
/// \brief This header is part of Syntropy core module. It contains definition of ranges.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <ostream>
#include <type_traits>

#include "syntropy/core/types.h"
#include "syntropy/math/math.h"
#include "syntropy/diagnostics/assert.h"
#include "syntropy/language/utility.h"
#include "syntropy/language/tuple.h"

namespace syntropy
{
    /************************************************************************/
    /* RANGE <TITERATOR>                                                    */
    /************************************************************************/

    /// \brief Range defined between two iterators.
    /// \tparam TIterator Type of the iterators. Must be any forward iterator compliant with the STL.
    /// \author Raffaele D. Facendola - April 2015
    template <typename TIterator>
    class Range
    {
    public:

        /// \brief Reference type.
        using TReference = decltype(*std::declval<TIterator>());

        /// \brief Value type.
        using TValue = std::remove_reference_t<TReference>;

        /// \brief Distance type between two iterators.
        using TDistance = decltype(std::declval<TIterator>() - std::declval<TIterator>());

        /// \brief Create an empty range.
        Range() = default;

        /// \brief Create a new range.
        /// \param begin Iterator to the first element in the range.
        /// \param end Iterator past the last element in the range.
        Range(TIterator begin, TIterator end);

        /// \brief Create a new range.
        /// \param begin First element in the range.
        /// \param count Number of elements between the first and the last element in the range.
        Range(TIterator begin, TDistance count);

        /// \brief Copy constructor.
        template <typename UIterator>
        Range(const Range<UIterator>& rhs);

        /// \brief Default assignment operator.
        Range& operator=(const Range&) = default;

        /// \brief Get an iterator to the first element in the range.
        TIterator Begin() const;

        /// \brief Get an iterator past the last element in the range.
        TIterator End() const;

        /// \brief Get an iterator to the last element in the range.
        TIterator Last() const;

        /// \brief Access the first element in the range.
        TReference GetFront() const;

        /// \brief Access the last element in the range.
        TReference GetBack() const;

        /// \brief Access an element by offset from the first element in the range.
        TReference operator[](TDistance offset) const;

        /// \brief Get a pointer to the beginning of the range.
        TValue* GetData() const;

        /// \brief Check whether the range is empty.
        operator Bool() const;

        /// \brief Get the number of elements in the range.
        TDistance GetSize() const;

        /// \brief Advance the range head forward by one element.
        /// This method results in undefined behavior is the range is empty.
        /// \return Returns a reference to this.
        Range& PopFront() &;

        /// \brief Advance the range head forward by some elements.
        /// This method results in undefined behavior is the range is empty or exceeded.
        /// \return Returns a reference to this.
        Range& PopFront(TDistance elements) &;

        /// \brief Recede the range tail backwards by one element.
        /// This method results in undefined behavior is the range is empty.
        /// \return Returns a reference to this.
        Range& PopBack() &;

        /// \brief Recede the range tail backwards by some elements.
        /// This method results in undefined behavior is the range is empty or exceeded.
        /// \return Returns a reference to this.
        Range& PopBack(TDistance elements) &;

        /// \brief Create a new range whose head is advanced forward by one element.
        /// This method results in undefined behavior is the range is empty.
        /// \return Returns a new range.
        Range PopFront() &&;

        /// \brief Create a new range whose head is advanced forward by some elements.
        /// This method results in undefined behavior is the range is empty or exceeded.
        /// \return Returns a new range.
        Range PopFront(TDistance elements) &&;

        /// \brief Create a new range whose tail is receded backwards by one element.
        /// This method results in undefined behavior is the range is empty.
         /// \return Returns a new range.
        Range PopBack() && ;

        /// \brief Create a new range whose tail is receded backwards by some elements.
        /// This method results in undefined behavior is the range is empty.
         /// \return Returns a new range.
        Range PopBack(TDistance elements) &&;

        /// \brief Check whether a range is contained entirely inside this range.
        /// #DEPRECATED Use non-member function Contains.
        Bool Contains(const Range& rhs) const;

        /// \brief Check whether an iterator falls within this range.
        /// #DEPRECATED Use non-member function Contains.
        Bool Contains(const TIterator& rhs) const;

    private:

        /// \brief Iterator to the first element in the range.
        TIterator begin_;

        /// \brief Iterator past the last element in the range.
        TIterator end_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get an iterator to the first element in the range.
    template <typename TIterator>
    TIterator begin(const Range<TIterator>& range);

    /// \brief Get an iterator past the last element in the range.
    template <typename TIterator>
    TIterator end(const Range<TIterator>& range);

    /// \brief Equality comparison for Ranges.
    template <typename TIterator>
    Bool operator==(const Range<TIterator>& lhs, const Range<TIterator>& rhs);

    /// \brief Inequality comparison for Ranges.
    template <typename TIterator>
    Bool operator==(const Range<TIterator>& lhs, const Range<TIterator>& rhs);

    /// \brief Create a new range from a pair of iterators.
    template <typename TIterator>
    Range<TIterator> MakeRange(TIterator begin, TIterator end);

    /// \brief Create a new range from an iterator and a number of elements.
    template <typename TIterator, typename TDistance>
    Range<TIterator> MakeRange(TIterator begin, TDistance count);

    /// \brief Create a new range defined between the iterators to the beginning and the end of a container.
    template <typename TContainer>
    auto MakeRange(TContainer& container);

    /// \brief Create a new range defined between the iterators to the beginning and the end of a container.
    template <typename TContainer>
    auto MakeRange(const TContainer& container);

    /// \brief Create a new range defined between the iterators to the beginning and the end of a container.
    template <typename TContainer>
    auto MakeConstRange(const TContainer& container);

    /// \brief Get the intersection of two ranges.
    /// \return If the two ranges do not overlap returns the empty range, otherwise return the range which is common to both lhs and rhs.
    template <typename TIterator>
    Range<TIterator> Intersection(const Range<TIterator>& lhs, const Range<TIterator>& rhs);

    /// \brief Get the union of two ranges.
    /// \return If a range is completely contained in the other, returns the larger range, otherwise return the range which encloses both lhs and rhs.
    template <typename TIterator>
    Range<TIterator> Union(const Range<TIterator>& lhs, const Range<TIterator>& rhs);

    /// \brief Check whether a range is contained entirely in another range.
    /// \return Returns true if rhs is contained in lhs, returns false otherwise.
    template <typename TIterator>
    Bool Contains(const Range<TIterator>& lhs, const Range<TIterator>& rhs);

    /// \brief Check whether an iterator is contained in another range.
/// \return Returns true if rhs is contained in lhs, returns false otherwise.
    template <typename TIterator>
    Bool Contains(const Range<TIterator>& lhs, const TIterator& rhs);

    /// \brief Check whether a range overlaps another range.
    /// \return Returns true if the intersection of both lhs and rhs is non-empty.
    template <typename TIterator>
    Bool Overlaps(const Range<TIterator>& lhs, const Range<TIterator>& rhs);

    /// \brief Truncate a range size up to a given upper bound.
    /// \return Returns a range which is equal to lhs but with size is limited up to upper_bound.
    template <typename TIterator>
    Range<TIterator> UpperBound(const Range<TIterator>& lhs, const typename Range<TIterator>::TDistance& upper_bound);

    /// \brief Split a range into two subranges, given the distance from the range begin.
    /// If the provided split point exceeds lhs range, the behavior of this method is undefined.
    template <typename TIterator>
    Tuple<Range<TIterator>, Range<TIterator>> Split(const Range<TIterator>& lhs, const typename Range<TIterator>::TDistance& split);

    /// \brief Stream insertion for a range of elements.
    template <typename TIterator>
    std::ostream& operator<<(std::ostream& out, const Range<TIterator>& range);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Range<TIterator>.

    template <typename TIterator>
    inline Range<TIterator>::Range(TIterator begin, TIterator end)
        : begin_(begin)
        , end_(end)
    {
        SYNTROPY_ASSERT(begin_ <= end_);
    }

    template <typename TIterator>
    inline Range<TIterator>::Range(TIterator begin, TDistance count)
        : Range(begin, begin + count)
    {

    }

    template <typename TIterator>
    template <typename UIterator>
    inline Range<TIterator>::Range(const Range<UIterator>& rhs)
        : begin_(rhs.Begin())
        , end_(rhs.End())
    {

    }

    template <typename TIterator>
    inline TIterator Range<TIterator>::Begin() const
    {
        return begin_;
    }

    template <typename TIterator>
    inline TIterator Range<TIterator>::End() const
    {
        return end_;
    }

    template <typename TIterator>
    inline TIterator Range<TIterator>::Last() const
    {
        return end_ - TDistance{ 1 };
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TReference Range<TIterator>::GetFront() const
    {
        return *begin_;
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TReference Range<TIterator>::GetBack() const
    {
        return *Last();
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TReference Range<TIterator>::operator[](TDistance offset) const
    {
        return *(Begin() + offset);
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TValue* Range<TIterator>::GetData() const
    {
        return &GetFront();
    }

    template <typename TIterator>
    inline Range<TIterator>::operator Bool() const
    {
        return GetSize() > TDistance{ 0 };
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TDistance Range<TIterator>::GetSize() const
    {
        using std::distance;

        return distance(begin_, end_);
    }

    template <typename TIterator>
    inline Range<TIterator>& Range<TIterator>::PopFront() &
    {
        ++begin_;

        return *this;
    }

    template <typename TIterator>
    inline Range<TIterator>& Range<TIterator>::PopFront(TDistance elements) &
    {
        begin_ += elements;

        return *this;
    }

    template <typename TIterator>
    inline Range<TIterator>& Range<TIterator>::PopBack() &
    {
        --end_;

        return *this;
    }

    template <typename TIterator>
    inline Range<TIterator>& Range<TIterator>::PopBack(TDistance elements) &
    {
        end_ -= elements;

        return *this;
    }

    template <typename TIterator>
    inline Range<TIterator> Range<TIterator>::PopFront() &&
    {
        return { ++begin_, end_ };
    }

    template <typename TIterator>
    inline Range<TIterator> Range<TIterator>::PopFront(TDistance elements) &&
    {
        begin_ += elements;

        return *this;
    }

    template <typename TIterator>
    inline Range<TIterator> Range<TIterator>::PopBack() &&
    {
        return { begin_, --end_ };
    }

    template <typename TIterator>
    inline Range<TIterator> Range<TIterator>::PopBack(TDistance elements) &&
    {
        return { begin_, end_ - elements };
    }

    template <typename TIterator>
    inline Bool Range<TIterator>::Contains(const Range& rhs) const
    {
        return (Union(*this, rhs) == *this);
    }

    template <typename TIterator>
    inline Bool Range<TIterator>::Contains(const TIterator& rhs) const
    {
        return (begin_ <= rhs) && (rhs < end_);
    }

    // Non-member functions.

    template <typename TIterator>
    inline TIterator begin(const Range<TIterator>& range)
    {
        return range.Begin();
    }

    template <typename TIterator>
    inline TIterator end(const Range<TIterator>& range)
    {
        return range.End();
    }

    template <typename TIterator>
    inline Bool operator==(const Range<TIterator>& lhs, const Range<TIterator>& rhs)
    {
        return (lhs.Begin() == rhs.Begin()) && (lhs.End() == rhs.End());
    }

    template <typename TIterator>
    inline Bool operator!=(const Range<TIterator>& lhs, const Range<TIterator>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TIterator>
    inline Range<TIterator> MakeRange(TIterator begin, TIterator end)
    {
        return { begin, end };
    }

    template <typename TIterator, typename TDistance>
    inline Range<TIterator> MakeRange(TIterator begin, TDistance count)
    {
        return { begin, count };
    }

    template <typename TContainer>
    inline auto MakeRange(TContainer& container)
    {
        using std::begin;
        using std::end;

        return MakeRange(begin(container), end(container));
    }

    template <typename TContainer>
    inline auto MakeRange(const TContainer& container)
    {
        using std::begin;
        using std::end;

        return MakeRange(begin(container), end(container));
    }

    template <typename TContainer>
    inline auto MakeConstRange(const TContainer& container)
    {
        using std::cbegin;
        using std::cend;

        return MakeRange(cbegin(container), cend(container));
    }

    template <typename TIterator>
    inline Range<TIterator> Intersection(const Range<TIterator>& lhs, const Range<TIterator>& rhs)
    {
        auto begin = Math::Max(lhs.Begin(), rhs.Begin());
        auto end = Math::Min(lhs.End(), rhs.End());

        if (begin <= end)
        {
            return { begin, end };
        }

        return {};
    }

    template <typename TIterator>
    inline Range<TIterator> Union(const Range<TIterator>& lhs, const Range<TIterator>& rhs)
    {
        auto begin = Math::Min(lhs.Begin(), rhs.Begin());
        auto end = Math::Max(lhs.End(), rhs.End());

        return { begin, end };
    }

    template <typename TIterator>
    inline Bool Contains(const Range<TIterator>& lhs, const Range<TIterator>& rhs)
    {
        return Intersection(lhs, rhs) == rhs;
    }

    template <typename TIterator>
    inline Bool Contains(const Range<TIterator>& lhs, const TIterator& rhs)
    {
        return (lhs.Begin() <= rhs) && (rhs < lhs.End());
    }

    template <typename TIterator>
    inline Bool Overlaps(const Range<TIterator>& lhs, const Range<TIterator>& rhs)
    {
        return !!Intersection(lhs, rhs);
    }

    template <typename TIterator>
    inline Range<TIterator> UpperBound(const Range<TIterator>& lhs, const typename Range<TIterator>::TDistance& upper_bound)
    {
        auto range_size = Math::Min(lhs.GetSize(), upper_bound);

        return Range<TIterator>{ lhs.Begin(), range_size };
    }

    template <typename TIterator>
    inline Tuple<Range<TIterator>, Range<TIterator>> Split(const Range<TIterator>& lhs, const typename Range<TIterator>::TDistance& split)
    {
        auto right = Range<TIterator>{ lhs.Begin() + split, lhs.End() };
        auto left = Range<TIterator>{ lhs.Begin(), right.Begin() };

        return { left, right };
    }

    template <typename TIterator>
    std::ostream& operator<<(std::ostream& out, const Range<TIterator>& range)
    {
        // Output format: e0, e1, ..., en

        if (range)
        {
            auto&& it = range.Begin();

            out << *it;

            for (++it; it != range.End(); ++it)
            {
                out << ", " << *it;
            }
        }

        return out;
    }

}