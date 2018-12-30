
/// \file range.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

namespace syntropy
{

    /// \brief Range defined between two iterators.
    /// A range supports iteration.
    /// \tparam TIterator Type of the iterators. Must be any forward iterator compliant with the STL.
    /// \author Raffaele D. Facendola - April 2015
    template <typename TIterator>
    class Range
    {
    public:

        /// \brief Standard STL defines.
        using iterator = TIterator;
        using difference_type = typename TIterator::difference_type;
        using size_type = size_t;
        using value_type = typename TIterator::value_type;
        using pointer = typename TIterator::pointer;
        using reference = typename TIterator::reference;

        /// \brief Create a new component range.
        /// \param begin Begin of the range.
        /// \param end End of the range.
        Range(iterator begin, iterator end);

        /// \brief Begin of the range.
        /// \return Returns an iterator pointing to the begin of the range.
        iterator begin() const;

        /// \brief End of the range.
        /// \return Returns an iterator pointing past the end of the range.
        iterator end() const;

        /// \brief Reference to the element pointed by head.
        reference operator*();

        /// \brief Pointer to the element pointed by head.
        pointer operator->();

        /// \brief Moves the head of the range forward by one element. Prefix.
        /// \return Returns the reference to this range.
        Range& operator++();

        /// \brief Moves the head of the range forward by one element. Postfix.
        /// \return Returns the old range.
        Range operator++(int);

        /// \brief Check whether this range is empty.
        /// \return Returns true if the range is empty, false otherwise.
        bool IsEmpty() const;

    protected:

        iterator begin_;	///< \brief Points to the first element of the range.

        iterator end_;		///< \brief Points to one element past the end of the range.

    };

    /// \brief Create a new range from a pair of iterators.
    template <typename TIterator>
    Range<TIterator> MakeRange(TIterator begin, TIterator end);

    /// \brief Stream insertion for a range of elements.
    template <typename TIterator>
    std::ostream& operator<<(std::ostream& out, const Range<TIterator>& range);

}

// Implementation

namespace syntropy
{

    /************************************************************************/
    /* RANGE                                                                */
    /************************************************************************/

    template <typename TIterator>
    Range<TIterator>::Range(iterator begin, iterator end) :
        begin_(begin),
        end_(end)
    {
    
    }

    template <typename TIterator>
    typename Range<TIterator>::iterator Range<TIterator>::begin() const
    {
        return begin_;
    }

    template <typename TIterator>
    typename Range<TIterator>::iterator Range<TIterator>::end() const
    {
        return end_;
    }

    template <typename TIterator>
    typename Range<TIterator>::reference Range<TIterator>::operator*()
    {
        return begin_.operator*();
    }

    template <typename TIterator>
    typename Range<TIterator>::pointer Range<TIterator>::operator->()
    {
        return begin_.operator->();
    }

    template <typename TIterator>
    Range<TIterator>& Range<TIterator>::operator++()
    {
        ++begin_;

        return *this;
    }

    template <typename TIterator>
    Range<TIterator> Range<TIterator>::operator++(int)
    {
        Range<TIterator> old(*this);

        ++begin_;

        return old;
    }

    template <typename TIterator>
    bool Range<TIterator>::IsEmpty() const
    {
        return begin_ == end_;
    }

    template <typename TIterator>
    Range<TIterator> MakeRange(TIterator begin, TIterator end)
    {
        return Range<TIterator>(begin, end);
    }

    template <typename TIterator>
    std::ostream& operator<<(std::ostream& out, const Range<TIterator>& range)
    {
        if (!range.IsEmpty())
        {
            auto&& it = range.begin();

            out << *it;                      // First element

            for (++it; it != range.end(); ++it)
            {
                out << ", " << *it;          // Remaining elements.
            }
        }

        return out;
    }

}