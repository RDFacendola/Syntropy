
/// \file range.h
/// \brief This header is part of syntropy types. It contains definition of a range.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <type_traits>
#include <ostream>

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

        /// \brief Value type.
        using TValue = std::remove_reference_t<decltype(*std::declval<TIterator>())>;

        /// \brief Create a new range.
        /// \param begin Iterator to the first element in the range.
        /// \param end Iterator past the last element in the range.
        Range(TIterator begin, TIterator end);

        /// \brief Create a new range.
        /// \param begin First element in the range.
        /// \param size Number of elements in the range.
        Range(TIterator begin, std::int64_t size);

        /// \brief Get an iterator to the first element in the range.
        TIterator Begin() const;

        /// \brief Get an iterator past the last element in the range.
        TIterator End() const;

        /// \brief Access the first element in the range.
        TValue& GetFront() const;

        /// \brief Access the last element in the range.
        TValue& GetBack() const;

        /// \brief Access an element by index.
        TValue& operator[](std::int64_t index) const;

        /// \brief Get a pointer to the beginning of the range.
        TValue* GetData() const;

        /// \brief Check whether the range is empty.
        operator bool() const;

        /// \brief Get the number of elements in the range.
        std::int64_t GetSize() const;

        /// \brief Advance the range head forward by one element.
        /// This method results in undefined behavior is the range is empty.
        void PopFront();

    protected:

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

    /// \brief Create a new range from a pair of iterators.
    template <typename TIterator>
    Range<TIterator> MakeRange(TIterator begin, TIterator end);

    /// \brief Create a new range from an iterator and a number of elements.
    template <typename TIterator>
    Range<TIterator> MakeRange(TIterator begin, std::int64_t size);

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
    inline Range<TIterator>::Range(TIterator begin, std::int64_t count)
        : Range(begin, begin + count)
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
    inline typename Range<TIterator>::TValue& Range<TIterator>::GetFront() const
    {
        return *begin_;
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TValue& Range<TIterator>::GetBack() const
    {
        return *(end_ - 1);
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TValue& Range<TIterator>::operator[](std::int64_t index) const
    {
        return GetData()[index];
    }

    template <typename TIterator>
    inline typename Range<TIterator>::TValue* Range<TIterator>::GetData() const
    {
        return &GetFront();
    }

    template <typename TIterator>
    inline Range<TIterator>::operator bool() const
    {
        return GetSize() > 0;
    }

    template <typename TIterator>
    inline std::int64_t Range<TIterator>::GetSize() const
    {
        using std::distance;

        return distance(begin_, end_);
    }

    template <typename TIterator>
    inline void Range<TIterator>::PopFront()
    {
        ++begin_;
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
    inline Range<TIterator> MakeRange(TIterator begin, TIterator end)
    {
        return { begin, end };
    }

    template <typename TIterator>
    inline Range<TIterator> MakeRange(TIterator begin, std::int64_t size)
    {
        return { begin, size };
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