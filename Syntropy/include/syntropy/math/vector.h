
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
    /* BASE VECTOR N                                                        */
    /************************************************************************/

    /// \brief Base definition for n-ary vectors.
    template <typename T, size_t kRank>
    struct BaseVectorN {};

    /// \brief Storage type for 2-vectors.
    template <typename T>
    struct BaseVectorN<T, 2>
    {
        T x_;           ///< \brief First element.
        T y_;           ///< \brief Second element.
    };

    /// \brief Storage type for 3-vectors.
    template <typename T>
    struct BaseVectorN<T, 3>
    {
        T x_;           ///< \brief First element.
        T y_;           ///< \brief Second element.
        T z_;           ///< \brief Third element.
    };

    /// \brief Storage type for 4-vectors.
    template <typename T>
    struct BaseVectorN<T, 4>
    {
        T x_;           ///< \brief First element.
        T y_;           ///< \brief Second element.
        T z_;           ///< \brief Third element.
        T w_;           ///< \brief Fourth element.
    };

    /************************************************************************/
    /* VECTOR N                                                             */
    /************************************************************************/

    /// \brief Represents a kRank-vector of elements of type T.
    /// \author Raffaele D. Facendola - January 2018
    template <typename T, size_t kRank>
    struct VectorN : BaseVectorN<T, kRank>
    {
        /// \brief Vector where each element equals 0.
        static const VectorN kZero;

        /// \brief Vector where each element equals 1.
        static const VectorN kOne;

        /// \brief Create a zero-vector.
        VectorN();

        /// \brief Create a vector having all equal elements.
        /// \param value Value for each element.
        VectorN(T value);

        /// \brief Create a vector from element values.
        /// \param ts Element values.
        template <typename... Ts>
        VectorN(Ts... ts);

        /// \brief Create a vector by copying or converting another vector.
        /// \param rhs Vector to copy\convert.
        template <typename U>
        explicit VectorN(const VectorN<U, kRank>& rhs);

        /// \brief Access an element in the vector.
        T& operator[](size_t index);

        /// \brief Access an element in the vector.
        const T& operator[](size_t index) const;

        /// \brief Sum a vector to this one.
        template <typename U>
        VectorN<T, kRank>& operator+=(const VectorN<U, kRank>& rhs);

        /// \brief Subtract a vector from this one.
        template <typename U>
        VectorN<T, kRank>& operator-=(const VectorN<U, kRank>& rhs);

        /// \brief Member-wise multiplication of this vector by another one.
        template <typename U>
        VectorN<T, kRank>& operator*=(const VectorN<U, kRank>& rhs);

        /// \brief Member-wise division of this vector by another one.
        template <typename U>
        VectorN<T, kRank>& operator/=(const VectorN<U, kRank>& rhs);

        /// \brief Member-wise modulus of this vector by another one.
        template <typename U>
        VectorN<T, kRank>& operator%=(const VectorN<U, kRank>& rhs);

    private:

        /// \brief Executes a unary operation on each element of this vector.
        template <typename TUnaryOp>
        VectorN<T, kRank>& ForEachElement(TUnaryOp op);

        /// \brief Executes a binary operation on each pair of elements of this vector and rhs.
        template <typename U, typename TBinaryOp>
        VectorN<T, kRank>& ForEachElement(const VectorN<U, kRank>& rhs, TBinaryOp op);

        /// \brief Executes a unary operation on each element of this vector.
        template <size_t... Is, typename TUnaryOp>
        void ForEachElement(std::index_sequence<Is...>, TUnaryOp op);

        /// \brief Executes a binary operation on each pair of elements of this vector and rhs.
        template <size_t... Is, typename U, typename TBinaryOp>
        void ForEachElement(std::index_sequence<Is...>, const VectorN<U, kRank>& rhs, TBinaryOp op);
    };

    /// \brief Vector addition.
    template <size_t kRank, typename T, typename U>
    auto operator+(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Vector difference.
    template <size_t kRank, typename T, typename U>
    auto operator-(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Vector negation.
    template <size_t kRank, typename T>
    auto operator-(const VectorN<T, kRank>& lhs);

    /// \brief Vector memberwise multiplication.
    template <size_t kRank, typename T, typename U>
    auto operator*(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Vector memberwise division.
    template <size_t kRank, typename T, typename U>
    auto operator/(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Vector memberwise modulus.
    template <size_t kRank, typename T, typename U>
    auto operator%(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /************************************************************************/
    /* CONCRETE VECTOR TYPES                                                */
    /************************************************************************/

    template <typename T>
    using Vector2 = VectorN<T, 2>;

    template <typename T>
    using Vector3 = VectorN<T, 3>;

    template <typename T>
    using Vector4 = VectorN<T, 4>;

    using Float2 = Vector2<float>;
    using Float3 = Vector3<float>;
    using Float4 = Vector4<float>;

    using Int2 = Vector2<int32_t>;
    using Int3 = Vector3<int32_t>;
    using Int4 = Vector4<int32_t>;

    using UInt2 = Vector2<uint32_t>;
    using UInt3 = Vector3<uint32_t>;
    using UInt4 = Vector4<uint32_t>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename T, size_t kRank>
    inline const VectorN<T, kRank> VectorN<T, kRank>::kZero = VectorN<T, kRank>(T(0));

    template <typename T, size_t kRank>
    inline const VectorN<T, kRank> VectorN<T, kRank>::kOne = VectorN<T, kRank>(T(1));

    template <typename T, size_t kRank>
    VectorN<T, kRank>::VectorN()
        : VectorN(T())
    {

    }

    template <typename T, size_t kRank>
    VectorN<T, kRank>::VectorN(T value)
    {
        ForEachElement([value](auto& lhs)
        {
            lhs = value;
        });
    }

    template <typename T, size_t kRank>
    template <typename... Ts>
    VectorN<T, kRank>::VectorN(Ts... ts)
        : BaseVectorN<T, kRank>{ ts... }
    {

    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>::VectorN(const VectorN<U, kRank>& rhs)
    {
        ForEachElement(rhs, [](auto& lhs, auto rhs)
        {
            lhs = static_cast<T>(rhs);
        });
    }

    template <typename T, size_t kRank>
    T& VectorN<T, kRank>::operator[](size_t index)
    {
        SYNTROPY_ASSERT(index < kRank);

        return reinterpret_cast<T*>(this)[index];
    }

    template <typename T, size_t kRank>
    const T& VectorN<T, kRank>::operator[](size_t index) const
    {
        SYNTROPY_ASSERT(index < kRank);

        return reinterpret_cast<const T*>(this)[index];
    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>& VectorN<T, kRank>::operator+=(const VectorN<U, kRank>& rhs)
    {
        return ForEachElement(rhs, [](auto& lhs, auto rhs)
        {
            lhs += static_cast<T>(rhs);
        });
    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>& VectorN<T, kRank>::operator-=(const VectorN<U, kRank>& rhs)
    {
        return ForEachElement(rhs, [](auto& lhs, auto rhs)
        {
            lhs -= static_cast<T>(rhs);
        });
    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>& VectorN<T, kRank>::operator*=(const VectorN<U, kRank>& rhs)
    {
        return ForEachElement(rhs, [](auto& lhs, auto rhs)
        {
            lhs *= static_cast<T>(rhs);
        });
    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>& VectorN<T, kRank>::operator/=(const VectorN<U, kRank>& rhs)
    {
        return ForEachElement(rhs, [](auto& lhs, auto rhs)
        {
            lhs /= static_cast<T>(rhs);
        });
    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>& VectorN<T, kRank>::operator%=(const VectorN<U, kRank>& rhs)
    {
        return ForEachElement(rhs, [](auto& lhs, auto rhs)
        {
            lhs %= static_cast<T>(rhs);
        });
    }

    template <typename T, size_t kRank>
    template <typename TUnaryOp>
    VectorN<T, kRank>& VectorN<T, kRank>::ForEachElement(TUnaryOp op)
    {
        ForEachElement(std::make_index_sequence<kRank>{}, std::forward<TUnaryOp>(op));
        return *this;
    }

    template <typename T, size_t kRank>
    template <typename U, typename TBinaryOp>
    VectorN<T, kRank>& VectorN<T, kRank>::ForEachElement(const VectorN<U, kRank>& rhs, TBinaryOp op)
    {
        ForEachElement(std::make_index_sequence<kRank>{}, rhs, std::forward<TBinaryOp>(op));
        return *this;
    }

    template <typename T, size_t kRank>
    template <size_t... Is, typename TUnaryOp>
    void VectorN<T, kRank>::ForEachElement(std::index_sequence<Is...>, TUnaryOp op)
    {
        (static_cast<void>(op((*this)[Is])), ...);
    }

    template <typename T, size_t kRank>
    template <size_t... Is, typename U, typename TBinaryOp>
    void VectorN<T, kRank>::ForEachElement(std::index_sequence<Is...>, const VectorN<U, kRank>& rhs, TBinaryOp op)
    {
        (static_cast<void>(op((*this)[Is], rhs[Is])), ...);
    }

    template <size_t kRank, typename T, typename U>
    auto operator+(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) += CommonVectorType(rhs);
    }

    template <size_t kRank, typename T, typename U>
    auto operator-(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) -= CommonVectorType(rhs);
    }

    template <size_t kRank, typename T>
    auto operator-(const VectorN<T, kRank>& lhs)
    {
        return VectorN<T, kRank>::kZero - lhs;
    }

    template <size_t kRank, typename T, typename U>
    auto operator*(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) *= CommonVectorType(rhs);
    }

    template <size_t kRank, typename T, typename U>
    auto operator/(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) /= CommonVectorType(rhs);
    }
    template <size_t kRank, typename T, typename U>
    auto operator%(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) %= CommonVectorType(rhs);
    }

}
