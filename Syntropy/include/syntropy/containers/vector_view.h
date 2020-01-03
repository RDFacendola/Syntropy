
/// \file vector_view.h
/// \brief This header is part of the syntropy containers. It contains non-owning views to vectors.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <vector>
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
    public:

        /// \brief Type of the underlying vector.
        using TVector = std::conditional_t<std::is_const_v<TElement>, const std::vector<std::remove_const_t<TElement>>, std::vector<TElement>>;

        /// \brief Create a view into a vector.
        VectorView(TVector& vector);

        /// \brief Default copy constructor.
        VectorView(const VectorView& rhs) = default;

        /// \brief Default assignment operator.
        VectorView& operator=(VectorView& rhs) = default;

        /// \brief Default destructor.
        ~VectorView() = default;

        /// \brief Access an element by index.
        TElement& operator[](std::size_t index);

        /// \brief Access an element by index.
        const TElement& operator[](std::size_t index) const;

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
        std::size_t GetSize() const noexcept;

    private:

        /// \brief Underlying vector.
        TVector* vector_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new vector view from a vector.
    template <typename TElement>
    VectorView<TElement> MakeVectorView(std::vector<TElement>& vector);

    /// \brief Create a new constant vector view from a vector.
    template <typename TElement>
    VectorView<const TElement> MakeConstVectorView(std::vector<TElement>& vector);

    /// \brief Create a new constant vector view from a constant vector.
    template <typename TElement>
    VectorView<const TElement> MakeConstVectorView(const std::vector<TElement>& vector);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // VectorView<TElement>.

    template <typename TElement>
    inline VectorView<TElement>::VectorView(TVector& vector)
        : vector_(&vector)
    {

    }

    template <typename TElement>
    inline TElement& VectorView<TElement>::operator[](std::size_t index)
    {
        return (*vector_)[index];
    }

    template <typename TElement>
    inline const TElement& VectorView<TElement>::operator[](std::size_t index) const
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
    inline std::size_t VectorView<TElement>::GetSize() const noexcept
    {
        return (*vector_).size();
    }

    // Non-member functions.

    template <typename TElement>
    inline VectorView<TElement> MakeVectorView(std::vector<TElement>& vector)
    {
        return { vector };
    }

    template <typename TElement>
    inline VectorView<const TElement> MakeConstVectorView(std::vector<TElement>& vector)
    {
        return { vector };
    }

    template <typename TElement>
    inline VectorView<const TElement> MakeConstVectorView(const std::vector<TElement>& vector)
    {
        return { vector };
    }
}
