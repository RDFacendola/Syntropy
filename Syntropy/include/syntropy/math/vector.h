
/// \file vector.h
/// \brief This header is part of the syntropy math system. It contains classes used to handle vectors.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits> 
#include <utility>

#include "syntropy/diagnostics/assert.h"

#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* VECTOR N                                                             */
    /************************************************************************/

    /// \brief Represents a kRank-vector of elements of type T.
    /// \author Raffaele D. Facendola - January 2018
    template <typename T, size_t kRank, typename TStorage>
    struct VectorN : TStorage
    {
        static_assert(std::is_arithmetic_v<T>, "T must be either floating type or integral type.");
        static_assert(std::is_pod_v<TStorage>, "TStorage must be a POD type.");
        static_assert(sizeof(TStorage) == sizeof(T[kRank]), "TStorage size must match the same exact storage requirement needed by a kRank-vector of type T.");

        /// \brief Default constructor. Create a null vector.
        VectorN()
            : VectorN(T())
        {

        }

        /// \brief Create a vector having all equal elements.
        /// \param t Value for each element.
        VectorN(T t)
            : VectorN(std::make_index_sequence<kRank>{}, t)
        {

        }

        /// \brief Create a vector from element values.
        /// \param ts Element values.
        template <typename... Ts, typename = std::enable_if_t<sizeof...(Ts) == kRank>>
        VectorN(Ts... ts)
            : TStorage{ts...}
        {

        }

        /// \brief Create a vector by copying or converting another vector.
        /// \param rhs Vector to copy\convert.
        template <typename U, typename TStorageU>
        explicit VectorN(const VectorN<U, kRank, TStorageU>& rhs)
            : VectorN(std::make_index_sequence<kRank>{}, rhs)
        {

        }

        /// \brief Access an element in the vector.
        T& operator[](size_t index)
        {
            SYNTROPY_ASSERT(index < kRank);

            return reinterpret_cast<T*>(this)[index];
        }

        /// \brief Access an element in the vector.
        const T& operator[](size_t index) const
        {
            SYNTROPY_ASSERT(index < kRank);

            return reinterpret_cast<const T*>(this)[index];
        }

    private:

        /// \brief Create a vector from a single value.
        template <size_t... Is>
        VectorN(std::index_sequence<Is...>, T t)
        {
            (static_cast<void>((*this)[Is] = t), ...);
        }

        /// \brief Create a vector by copying another vector element-wise.
        template <size_t... Is, typename U, typename TStorageU>
        VectorN(std::index_sequence<Is...>, const VectorN<U, kRank, TStorageU>& rhs)
        {
            (static_cast<void>((*this)[Is] = static_cast<T>(rhs[Is])), ...);
        }
    };

    /************************************************************************/
    /* VECTOR STORAGE                                                       */
    /************************************************************************/

    /// \brief Storage type for 2-vectors.
    template <typename T>
    struct VectorStorage2
    {
        T x_;           ///< \brief First element.
        T y_;           ///< \brief Second element.
    };

    /// \brief Storage type for 3-vectors.
    template <typename T>
    struct VectorStorage3
    {
        T x_;           ///< \brief First element.
        T y_;           ///< \brief Second element.
        T z_;           ///< \brief Third element.
    };

    /// \brief Storage type for 4-vectors.
    template <typename T>
    struct VectorStorage4
    {
        T x_;           ///< \brief First element.
        T y_;           ///< \brief Second element.
        T z_;           ///< \brief Third element.
        T w_;           ///< \brief Fourth element.
    };

    /************************************************************************/
    /* HELPER TYPES                                                         */
    /************************************************************************/

    template <typename T>
    using Vector2 = VectorN<T, 2, VectorStorage2<T>>;

    template <typename T>
    using Vector3 = VectorN<T, 3, VectorStorage3<T>>;

    template <typename T>
    using Vector4 = VectorN<T, 4, VectorStorage4<T>>;

    using Float2 = Vector2<float>;
    using Float3 = Vector3<float>;
    using Float4 = Vector4<float>;

    using Int2 = Vector2<int32_t>;
    using Int3 = Vector3<int32_t>;
    using Int4 = Vector4<int32_t>;

    using UInt2 = Vector2<uint32_t>;
    using UInt3 = Vector3<uint32_t>;
    using UInt4 = Vector4<uint32_t>;
}