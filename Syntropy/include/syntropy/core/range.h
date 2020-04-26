
/// \file range.h
/// \brief This header is part of Syntropy core module. It contains definition of ranges.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "syntropy/syntropy.h"

#include "syntropy/language/type_traits.h"
#include "syntropy/language/utility.h"
#include "syntropy/language/stream.h"

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

        /// \brief Difference type between two iterators.
        using TDifference = decltype(std::declval<TIterator>() - std::declval<TIterator>());

        /// \brief Create an empty range.
        Range() = default;

        /// \brief Create a new range.
        /// \param begin Iterator to the first element in the range.
        /// \param end Iterator past the last element in the range.
        Range(TIterator begin, TIterator end);

        /// \brief Create a new range.
        /// \param begin First element in the range.
        /// \param count Number of elements in the range.
        Range(TIterator begin, TDifference count);

        /// \brief Copy constructor.
        template <typename UIterator>
        Range(const Range<UIterator>& rhs);

        /// \brief DEFAULT assignment operator.
        Range& operator=(const Range&) = default;

        /// \brief Get an iterator to the first element in the range.
        TIterator Begin() const;

        /// \brief Get an iterator past the last element in the range.
        TIterator End() const;

        /// \brief Access the first element in the range.
        TReference GetFront() const;

        /// \brief Access the last element in the range.
        TReference GetBack() const;

        /// \brief Access an element by offset from the first element in the range.
        TReference operator[](TDifference offset) const;

        /// \brief Get a pointer to the beginning of the range.
        TValue* GetData() const;

        /// \brief Check whether the range is empty.
        operator bool() const;

        /// \brief Get the number of elements in the range.
        TDifference GetSize() const;

        /// \brief Advance the range head forward by one element.
        /// This method results in undefined behavior is the range is empty.
        void PopFront();

        /// \brief Check whether a range is contained entirely inside this range.
        bool Contains(const Range& rhs) const;

        /// \brief Check whether an iterator falls within this range.
        bool Contains(const TIterator& rhs) const;

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
    bool operator==(const Range<TIterator>& lhs, const Range<TIterator>& rhs);

    /// \brief Inequality comparison for Ranges.
    template <typename TIterator>
    bool operator==(const Range<TIterator>& lhs, const Range<TIterator>& rhs);

    /// \brief Create a new range from a pair of iterators.
    template <typename TIterator>
    Range<TIterator> MakeRange(TIterator begin, TIterator end);

    /// \brief Create a new range from an iterator and a number of elements.
    template <typename TIterator, typename TDifference>
    Range<TIterator> MakeRange(TIterator begin, TDifference count);

    /// \brief Create a new range from a collection.
    template <typename TCollection>
    auto MakeRange(TCollection& collection);

    /// \brief Create a new range from a collection.
    template <typename TCollection>
    auto MakeRange(const TCollection& collection);

    /// \brief Create a new range from a collection.
    template <typename TCollection>
    auto MakeConstRange(const TCollection& collection);

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
    
    }

    template <typename TIterator>
    inline Range<TIterator>::Range(TIterator begin, TDifference count)
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
    inline typename Range<TIterator>::TReference Range<TIterator>::GetFront() const
    {
        return *begin_;
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TReference Range<TIterator>::GetBack() const
    {
        return *(end_ - 1);
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TReference Range<TIterator>::operator[](TDifference offset) const
    {
        return *(Begin() + offset);
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TValue* Range<TIterator>::GetData() const
    {
        return &GetFront();
    }

    template <typename TIterator>
    inline Range<TIterator>::operator bool() const
    {
        return GetSize() > TDifference{ 0 };
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TDifference Range<TIterator>::GetSize() const
    {
        using std::distance;

        return distance(begin_, end_);
    }

    template <typename TIterator>
    inline void Range<TIterator>::PopFront()
    {
        ++begin_;
    }

    template <typename TIterator>
    inline bool Range<TIterator>::Contains(const Range& rhs) const
    {
        return (begin_ <= rhs.Begin()) && (rhs.End() <= end_);
    }

    template <typename TIterator>
    inline bool Range<TIterator>::Contains(const TIterator& rhs) const
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
    inline bool operator==(const Range<TIterator>& lhs, const Range<TIterator>& rhs)
    {
        return (lhs.Begin() == rhs.Begin()) && (lhs.End() == rhs.End());
    }

    template <typename TIterator>
    inline bool operator!=(const Range<TIterator>& lhs, const Range<TIterator>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TIterator>
    inline Range<TIterator> MakeRange(TIterator begin, TIterator end)
    {
        return { begin, end };
    }

    template <typename TIterator, typename TDifference>
    inline Range<TIterator> MakeRange(TIterator begin, TDifference count)
    {
        return { begin, count };
    }

    template <typename TCollection>
    inline auto MakeRange(TCollection& collection)
    {
        using std::begin;
        using std::end;

        return MakeRange(begin(collection), end(collection));
    }

    template <typename TCollection>
    inline auto MakeRange(const TCollection& collection)
    {
        using std::begin;
        using std::end;

        return MakeRange(begin(collection), end(collection));
    }

    template <typename TCollection>
    inline auto MakeConstRange(const TCollection& collection)
    {
        using std::cbegin;
        using std::cend;

        return MakeRange(cbegin(collection), cend(collection));
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