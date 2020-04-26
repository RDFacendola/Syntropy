
/// \file vector_view.h
/// \brief This header is part of the syntropy containers. It contains non-owning views to vectors.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/containers/vector.h"
#include <type_traits>

namespace syntropy
{
    /************************************************************************/
    /* VECTOR VIEW                                                          */
    /************************************************************************/

    /// \brief Represents a non-owning view into a vector. Allows vector iteration and operations on individual elements.
    /// TElement can either be non-const or const.
    /// If TElement is non-const both read and write operations are allowed.
    /// If TElement is const only read operations are allowed.
    /// Underlying vector size cannot be changed via a view but can be changed externally: this view always refers to the most recent version of the vector.
    /// \author Raffaele D. Facendola - January 2020.
    template <typename TElement>
    class VectorView
    {

        template <typename UElement>
        friend class VectorView;

    public:

        /// \brief Create a view into a non-const vector.
        template <typename UElement>
        VectorView(Vector<UElement>& vector);

        /// \brief Create a view into a const vector.
        template <typename UElement>
        VectorView(const Vector<UElement>& vector);

        /// \brief Default copy constructor.
        template <typename UElement>
        VectorView(const VectorView<UElement>& rhs);

        /// \brief Default assignment operator.
        VectorView& operator=(const VectorView& rhs) = default;

        /// \brief Default destructor.
        ~VectorView() = default;

        /// \brief Access an element by index.
        TElement& operator[](std::int64_t index);

        /// \brief Access an element by index.
        const TElement& operator[](std::int64_t index) const;

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
        bool IsEmpty() const noexcept;

        /// \brief Get the number of elements in the container.
        /// \return Returns the number of elements in the container.
        std::int64_t GetSize() const noexcept;

    private:

        /// \brief Const elements are not supported: propagate the constness outside the container.
        using TVector = std::conditional_t<std::is_const_v<TElement>, const Vector<std::remove_const_t<TElement>>, Vector<TElement>>;

        /// \brief Underlying vector.
        TVector* vector_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new vector view from a vector.
    template <typename TElement>
    VectorView<TElement> MakeVectorView(Vector<TElement>& vector);

    /// \brief Create a new const vector view from a const vector.
    template <typename TElement>
    VectorView<const TElement> MakeVectorView(const Vector<TElement>& vector);

    /// \brief Create a new vector view from a vector view.
    template <typename TElement>
    VectorView<TElement> MakeVectorView(const VectorView<TElement>& vector_view);

    /// \brief Create a new const vector view from a vector.
    template <typename TElement>
    VectorView<const TElement> MakeConstVectorView(const Vector<TElement>& vector);

    /// \brief Create a new const vector view from a vector view.
    template <typename TElement>
    VectorView<const TElement> MakeConstVectorView(const VectorView<TElement>& vector_view);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VectorView<TElement>.

    template <typename TElement>
    template <typename UElement>
    inline VectorView<TElement>::VectorView(Vector<UElement>& vector)
        : vector_(&vector)
    {

    }

    template <typename TElement>
    template <typename UElement>
    inline VectorView<TElement>::VectorView(const Vector<UElement>& vector)
        : vector_(&vector)
    {
        static_assert(std::is_const_v<TElement>, "Conversion loses const qualifiers.");
    }

    template <typename TElement>
    template <typename UElement>
    inline VectorView<TElement>::VectorView(const VectorView<UElement>& rhs)
        : vector_(rhs.vector_)
    {
        static_assert(std::is_const_v<TElement> || !std::is_const_v<UElement>, "Conversion loses const qualifiers.");
    }

    template <typename TElement>
    inline TElement& VectorView<TElement>::operator[](std::int64_t index)
    {
        return (*vector_)[index];
    }

    template <typename TElement>
    inline const TElement& VectorView<TElement>::operator[](std::int64_t index) const
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
        return std::as_const(*vector_).front();
    }

    template <typename TElement>
    inline TElement& VectorView<TElement>::GetBack()
    {
        return (*vector_).back();
    }

    template <typename TElement>
    inline const TElement& VectorView<TElement>::GetBack() const
    {
        return std::as_const(*vector_).back();
    }

    template <typename TElement>
    inline bool VectorView<TElement>::IsEmpty() const noexcept
    {
        return (*vector_).empty();
    }

    template <typename TElement>
    inline std::int64_t VectorView<TElement>::GetSize() const noexcept
    {
        return (*vector_).size();
    }

    // Non-member functions.

    template <typename TElement>
    VectorView<TElement> MakeVectorView(Vector<TElement>& vector)
    {
        return { vector };
    }

    template <typename TElement>
    VectorView<const TElement> MakeVectorView(const Vector<TElement>& vector)
    {
        return { vector };
    }

    template <typename TElement>
    VectorView<TElement> MakeVectorView(const VectorView<TElement>& vector_view)
    {
        return { vector_view };
    }

    template <typename TElement>
    VectorView<const TElement> MakeConstVectorView(const Vector<TElement>& vector)
    {
        return { vector };
    }

    template <typename TElement>
    VectorView<const TElement> MakeConstVectorView(const VectorView<TElement>& vector_view)
    {
        return { vector_view };
    }

}
