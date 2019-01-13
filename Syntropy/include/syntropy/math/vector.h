
/// \file vector.h
/// \brief This header is part of the syntropy math system. It contains classes used to handle vectors.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits> 
#include <utility>
#include <algorithm>

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
    };

    /// \brief Vector sum.
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

    /// \brief Step on each element tuple of all the provided vectors executing a function.
    template <typename TOp, typename... TVectors>
    constexpr void LockstepApply(TOp op, TVectors&&... vectors);

    /// \brief Get the dot product of two vectors.
    template <size_t kRank, typename T, typename U>
    auto Dot(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Get the euclidean length of a vector.
    template <size_t kRank, typename T>
    float Length(const VectorN<T, kRank>& rhs);

    /// \brief Get the squared euclidean length of a vector.
    template <size_t kRank, typename T>
    float Length2(const VectorN<T, kRank>& rhs);

    /// \brief Get the Manhattan length of a vector.
    template <size_t kRank, typename T>
    T ManhattanLength(const VectorN<T, kRank>& rhs);

    /// \brief Get the Chebyshev length of a vector.
    template <size_t kRank, typename T>
    T ChebyshevLength(const VectorN<T, kRank>& rhs);

    /// \brief Get the euclidean distance between two vectors.
    template <size_t kRank, typename T, typename U>
    float Distance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Get the squared euclidean distance between two vectors.
    template <size_t kRank, typename T, typename U>
    float Distance2(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Get the Manhattan distance between two vectors.
    template <size_t kRank, typename T, typename U>
    auto ManhattanDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Get the Chebyshev distance between two vectors.
    template <size_t kRank, typename T, typename U>
    auto ChebyshevDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

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

    namespace details
    {
        /// \brief Provides a member value containing the rank of the elements in TVectors if the rank is the same for each element.
        template <typename... TVectors>
        struct LockstepRank{};

        /// \brief Helper constant for LockstepRank.
        template <typename... TVectors>
        constexpr size_t LockstepRankV = LockstepRank<std::decay_t<TVectors>...>::Value;

        /// \brief Specialization for VectorN.
        template <size_t kRank, typename... Ts>
        struct LockstepRank<VectorN<Ts, kRank>...>
        {
            static constexpr size_t Value = kRank;
        };

        /// \brief Apply a function on all the i-th elements of the provided vectors at once.
        template <size_t I, typename TOp, typename... TVectors>
        inline constexpr void LockstepApply(TOp&& op, TVectors&&... vectors)
        {
            op(vectors[I]...);
        }

        /// \brief Apply a function on all the i-th elements of the provided vectors at once.
         template <typename TOp, typename... TVectors, size_t... Is>
         inline constexpr void LockstepApply(TOp&& op, TVectors&&... vectors, std::index_sequence<Is...>)
         {
             (LockstepApply<Is>(std::forward<TOp>(op), std::forward<TVectors>(vectors)...), ...);
         }
    }

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
        LockstepApply([value](auto& rhs) { rhs = value; }, *this);
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
        LockstepApply([](auto& lhs, auto rhs) { lhs = static_cast<T>(rhs); }, *this, rhs);
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
        LockstepApply([](auto& lhs, auto rhs) { lhs += static_cast<T>(rhs); }, *this, rhs);

        return *this;
    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>& VectorN<T, kRank>::operator-=(const VectorN<U, kRank>& rhs)
    {
        LockstepApply([](auto& lhs, auto rhs) { lhs -= static_cast<T>(rhs); }, *this, rhs);

        return *this;
    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>& VectorN<T, kRank>::operator*=(const VectorN<U, kRank>& rhs)
    {
        LockstepApply([](auto& lhs, auto rhs) {lhs *= static_cast<T>(rhs); }, *this, rhs);

        return *this;
    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>& VectorN<T, kRank>::operator/=(const VectorN<U, kRank>& rhs)
    {
        LockstepApply([](auto& lhs, auto rhs) {lhs /= static_cast<T>(rhs); }, *this, rhs);

        return *this;
    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>& VectorN<T, kRank>::operator%=(const VectorN<U, kRank>& rhs)
    {
        LockstepApply(*this, rhs, [](auto& lhs, auto rhs)
        {
            lhs = Mod(lhs, static_cast<T>(rhs));
        });

        return *this;
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

    template <typename TOp, typename... TVectors>
    inline constexpr void LockstepApply(TOp op, TVectors&&... vectors)
    {
        details::LockstepApply<TOp, TVectors...>(std::forward<TOp>(op), std::forward<TVectors>(vectors)..., std::make_index_sequence<details::LockstepRankV<TVectors...>>{});
    }

    template <size_t kRank, typename T, typename U>
    auto Dot(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        auto dot = std::common_type_t<T, U>{};

        LockstepApply([&dot](auto l, auto r) { dot += l * r; }, lhs, rhs);

        return dot;
    }

    template <size_t kRank, typename T>
    inline float Length(const VectorN<T, kRank>& rhs)
    {
        return FastSqrt(Length2(rhs));
    }

    template <size_t kRank, typename T>
    inline float Length2(const VectorN<T, kRank>& rhs)
    {
        return Dot(rhs, rhs);
    }

    template <size_t kRank, typename T>
    T ManhattanLength(const VectorN<T, kRank>& rhs)
    {
        auto length = T{};

        LockstepApply([&length](auto r) { length += FastAbs(r); }, rhs);

        return length;
    }

    template <size_t kRank, typename T>
    T ChebyshevLength(const VectorN<T, kRank>& rhs)
    {
        auto length = T{};

        LockstepApply([&length](auto r) { length = std::max(length, FastAbs(r)); }, rhs);

        return length;
    }

    template <size_t kRank, typename T, typename U>
    inline float Distance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return Length(lhs - rhs);
    }

    template <size_t kRank, typename T, typename U>
    inline float Distance2(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return Length2(lhs - rhs);
    }

    template <size_t kRank, typename T, typename U>
    inline auto ManhattanDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return ManhattanLength(lhs - rhs);
    }

    template <size_t kRank, typename T, typename U>
    inline auto ChebyshevDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return ChebyshevLength(lhs - rhs);
    }

}
