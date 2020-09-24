
/// \file vector_view.h
/// \brief This header is part of the Syntropy containers module. It contains non-owning views to vectors.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/core/range.h"
#include "syntropy/core/data_structures/vector.h"
#include "syntropy/language/traits.h"
#include "syntropy/language/support.h"

namespace Syntropy
{
    /************************************************************************/
    /* VECTOR VIEW                                                          */
    /************************************************************************/

    /// \brief Represents a non-owning view into a vector. Allows vector iteration and operations on individual elements.
    /// TElement can either be non-const or const.
    /// If TElement is non-const both read and write operations are allowed.
    /// If TElement is const only read operations are allowed.
    /// Underlying vector size cannot be changed via a view but can be changed externally: this view always refers to the most recent version of the vector.
    /// The intended use of this class is when a reference to a vector needs to be exposed, allowing for reads and (optionally) writes of individual elements but changing the collection size is undesired.
    /// \author Raffaele D. Facendola - January 2020.
    template <typename TElement>
    class VectorView
    {

        template <typename UElement>
        friend class VectorView;

    public:

        /// \brief Type of the underlying vector.
        /// Const elements are not supported: propagate the constness outside the container.
        using TVector = ConditionalT<Templates::IsConst<TElement>, const Array<Templates::RemoveConst<TElement>>, Array<TElement>>;

        /// \brief Type of a range in the vector view.
        using TRange = decltype(MakeRange(Declval<TVector&>()));

        /// \brief Type of a const range in the vector view.
        using TConstRange = decltype(MakeConstRange(Declval<TVector&>()));

        /// \brief Create a view into a non-const vector.
        template <typename UElement>
        VectorView(Array<UElement>& vector);

        /// \brief Create a view into a const vector.
        template <typename UElement>
        VectorView(const Array<UElement>& vector);

        /// \brief Default copy constructor.
        template <typename UElement>
        VectorView(const VectorView<UElement>& rhs);

        /// \brief Default assignment operator.
        VectorView& operator=(const VectorView& rhs) = default;

        /// \brief Default destructor.
        ~VectorView() = default;

        /// \brief Access an element by index.
        TElement& operator[](Int index);

        /// \brief Access an element by index.
        const TElement& operator[](Int index) const;

        /// \brief Get a reference to the first element in the vector.
        TElement& GetFront();

        /// \brief Get a reference to the first element in the vector.
        const TElement& GetFront() const;

        /// \brief Get a reference to the last element in the vector.
        TElement& GetBack();

        /// \brief Get a reference to the last element in the vector.
        const TElement& GetBack() const;

        /// \brief Check whether the vector is empty.
        /// \return Returns true if the container is empty, returns false otherwise.
        Bool IsEmpty() const noexcept;

        /// \brief Get the number of elements in the container.
        /// \return Returns the number of elements in the container.
        Int GetSize() const noexcept;

        /// \brief Get the underlying range.
        TRange GetRange();

        /// \brief Get the underlying constant range.
        TConstRange GetRange() const;

        /// \brief Get the underlying constant range.
        TConstRange GetConstRange() const;

    private:

        /// \brief Underlying vector.
        TVector* vector_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new vector view from a vector.
    template <typename TElement>
    VectorView<TElement> MakeVectorView(Array<TElement>& vector);

    /// \brief Create a new const vector view from a const vector.
    template <typename TElement>
    VectorView<const TElement> MakeVectorView(const Array<TElement>& vector);

    /// \brief Create a new vector view from a vector view.
    template <typename TElement>
    VectorView<TElement> MakeVectorView(const VectorView<TElement>& vector_view);

    /// \brief Create a new const vector view from a vector.
    template <typename TElement>
    VectorView<const TElement> MakeConstVectorView(const Array<TElement>& vector);

    /// \brief Create a new const vector view from a vector view.
    template <typename TElement>
    VectorView<const TElement> MakeConstVectorView(const VectorView<TElement>& vector_view);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VectorView<TElement>.

    template <typename TElement>
    template <typename UElement>
    inline VectorView<TElement>::VectorView(Array<UElement>& vector)
        : vector_(&vector)
    {

    }

    template <typename TElement>
    template <typename UElement>
    inline VectorView<TElement>::VectorView(const Array<UElement>& vector)
        : vector_(&vector)
    {
        static_assert(Templates::IsConst<TElement>, "Conversion loses const qualifiers.");
    }

    template <typename TElement>
    template <typename UElement>
    inline VectorView<TElement>::VectorView(const VectorView<UElement>& rhs)
        : vector_(rhs.vector_)
    {
        static_assert(Templates::IsConst<TElement> || !Templates::IsConst<UElement>, "Conversion loses const qualifiers.");
    }

    template <typename TElement>
    inline TElement& VectorView<TElement>::operator[](Int index)
    {
        return (*vector_)[index];
    }

    template <typename TElement>
    inline const TElement& VectorView<TElement>::operator[](Int index) const
    {
        return (*vector_)[index];
    }

    template <typename TElement>
    inline TElement& VectorView<TElement>::GetFront()
    {
        return (*vector_).front();
    }

    template <typename TElement>
    inline const TElement& VectorView<TElement>::GetFront() const
    {
        return ReadOnly(*vector_).front();
    }

    template <typename TElement>
    inline TElement& VectorView<TElement>::GetBack()
    {
        return (*vector_).back();
    }

    template <typename TElement>
    inline const TElement& VectorView<TElement>::GetBack() const
    {
        return AsConst(*vector_).back();
    }

    template <typename TElement>
    inline Bool VectorView<TElement>::IsEmpty() const noexcept
    {
        return (*vector_).empty();
    }

    template <typename TElement>
    inline Int VectorView<TElement>::GetSize() const noexcept
    {
        return (*vector_).size();
    }

    template <typename TElement>
    inline typename VectorView<TElement>::TRange VectorView<TElement>::GetRange()
    {
        return MakeRange(*vector_);
    }

    template <typename TElement>
    inline typename VectorView<TElement>::TConstRange VectorView<TElement>::GetRange() const
    {
        return GetConstRange();
    }

    template <typename TElement>
    inline typename  VectorView<TElement>::TConstRange VectorView<TElement>::GetConstRange() const
    {
        return MakeConstRange(*vector_);
    }

    // Non-member functions.

    template <typename TElement>
    VectorView<TElement> MakeVectorView(Array<TElement>& vector)
    {
        return { vector };
    }

    template <typename TElement>
    VectorView<const TElement> MakeVectorView(const Array<TElement>& vector)
    {
        return { vector };
    }

    template <typename TElement>
    VectorView<TElement> MakeVectorView(const VectorView<TElement>& vector_view)
    {
        return { vector_view };
    }

    template <typename TElement>
    VectorView<const TElement> MakeConstVectorView(const Array<TElement>& vector)
    {
        return { vector };
    }

    template <typename TElement>
    VectorView<const TElement> MakeConstVectorView(const VectorView<TElement>& vector_view)
    {
        return { vector_view };
    }

}
