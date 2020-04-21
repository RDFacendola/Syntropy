
/// \file range.h
/// \brief This header is part of syntropy types. It contains definition of a range.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

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

        /// \brief Create a new component range.
        /// \param begin Begin of the range.
        /// \param end End of the range.
        Range(TIterator begin, TIterator end);

        /// \brief Get an iterator to the first element in the range.
        TIterator Begin() const;

        /// \brief Get an iterator past the last element in the range.
        TIterator End() const;

        /// \brief Reference to the element pointed by head.
        auto operator*();

        /// \brief Pointer to the element pointed by head.
        auto operator->();

        /// \brief Advance the range head forward by one element.
        /// This method results in undefined behavior if the range is empty.
        void Advance();

        /// \brief Check whether the range is empty.
        operator bool() const;

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

    /// \brief Stream insertion for a range of elements.
    template <typename TIterator>
    std::ostream& operator<<(std::ostream& out, const Range<TIterator>& range);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Range<TIterator>.

    template <typename TIterator>
    Range<TIterator>::Range(TIterator begin, TIterator end) :
        begin_(begin),
        end_(end)
    {
    
    }

    template <typename TIterator>
    TIterator Range<TIterator>::Begin() const
    {
        return begin_;
    }

    template <typename TIterator>
    TIterator Range<TIterator>::End() const
    {
        return end_;
    }

    template <typename TIterator>
    inline auto Range<TIterator>::operator*()
    {
        return *begin_;
    }

    template <typename TIterator>
    typename auto Range<TIterator>::operator->()
    {
        return &(*begin_);
    }

    template <typename TIterator>
    inline void Range<TIterator>::Advance()
    {
        ++begin_;
    }

    template <typename TIterator>
    inline Range<TIterator>::operator bool() const
    {
        return begin_ != end_;
    }

    // Non-member functions.

    template <typename TIterator>
    inline TIterator begin(const Range<TIterator>& range)
    {
        return range.Begin();
    }

    template <typename TIterator>
    inline TIterator end(Range<TIterator>& range)
    {
        return range.End();
    }

    template <typename TIterator>
    inline Range<TIterator> MakeRange(TIterator begin, TIterator end)
    {
        return { begin, end };
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