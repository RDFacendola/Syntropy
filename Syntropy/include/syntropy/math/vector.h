
/// \file vector.h
/// \brief This header is part of the syntropy math system. It contains classes used to handle vectors.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <array>
#include <utility>

#include "syntropy/diagnostics/assert.h"
#include "syntropy/patterns/tuple.h"
#include "syntropy/math/math.h"
#include "syntropy/type_traits.h"
#include "syntropy/patterns/utility.h"

namespace syntropy
{
    /************************************************************************/
    /* BASE VECTOR N                                                        */
    /************************************************************************/

    /// \brief Base definition for n-ary vectors.
    template <typename T, size_t kRank>
    struct BaseVectorN
    {
        std::array<T, kRank> e_;        /// \brief Elements in the vector.
    };

    /// \brief Storage type for 1-vectors.
    template <typename T>
    struct BaseVectorN<T, 1>
    {
        T x_;                           ///< \brief First element.
    };

    /// \brief Storage type for 2-vectors.
    template <typename T>
    struct BaseVectorN<T, 2>
    {
        T x_;                           ///< \brief First element.
        T y_;                           ///< \brief Second element.
    };

    /// \brief Storage type for 3-vectors.
    template <typename T>
    struct BaseVectorN<T, 3>
    {
        T x_;                           ///< \brief First element.
        T y_;                           ///< \brief Second element.
        T z_;                           ///< \brief Third element.
    };

    /// \brief Storage type for 4-vectors.
    template <typename T>
    struct BaseVectorN<T, 4>
    {
        T x_;                           ///< \brief First element.
        T y_;                           ///< \brief Second element.
        T z_;                           ///< \brief Third element.
        T w_;                           ///< \brief Fourth element.
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

        /// \brief Create an uninitialized vector.
        VectorN(uninitialized_t);

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
        constexpr T& operator[](size_t index) noexcept;

        /// \brief Access an element in the vector.
        constexpr const T& operator[](size_t index) const noexcept;

        /// \brief Sum a value to this vector.
        VectorN<T, kRank>& operator+=(T rhs);

        /// \brief Subtract a value from this vector.
        VectorN<T, kRank>& operator-=(T rhs);

        /// \brief Member-wise multiplication of this vector by a value.
        VectorN<T, kRank>& operator*=(T rhs);

        /// \brief Member-wise division of this vector by a value.
        VectorN<T, kRank>& operator/=(T rhs);

        /// \brief Member-wise modulus of this vector by a value.
        VectorN<T, kRank>& operator%=(T rhs);

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

    /// \brief Specialization of LockstepRank for VectorN.
    template <size_t kRank, typename... Ts>
    struct LockstepRank<VectorN<Ts, kRank>...>
    {
        static constexpr size_t Value = kRank;
    };

    /************************************************************************/
    /* CONCRETE VECTOR TYPES                                                */
    /************************************************************************/

    template <typename T>
    using Vector1 = VectorN<T, 1>;

    template <typename T>
    using Vector2 = VectorN<T, 2>;

    template <typename T>
    using Vector3 = VectorN<T, 3>;

    template <typename T>
    using Vector4 = VectorN<T, 4>;

    using Float1 = Vector1<float>;
    using Float2 = Vector2<float>;
    using Float3 = Vector3<float>;
    using Float4 = Vector4<float>;

    using Int1 = Vector1<int32_t>;
    using Int2 = Vector2<int32_t>;
    using Int3 = Vector3<int32_t>;
    using Int4 = Vector4<int32_t>;

    using UInt1 = Vector1<uint32_t>;
    using UInt2 = Vector2<uint32_t>;
    using UInt3 = Vector3<uint32_t>;
    using UInt4 = Vector4<uint32_t>;

    using Long1 = Vector1<int64_t>;
    using Long2 = Vector2<int64_t>;
    using Long3 = Vector3<int64_t>;
    using Long4 = Vector4<int64_t>;

    using ULong1 = Vector1<uint64_t>;
    using ULong2 = Vector2<uint64_t>;
    using ULong3 = Vector3<uint64_t>;
    using ULong4 = Vector4<uint64_t>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Extracts the I-th element from the vector.I must be an integer value in[0, kRank).
    template <std::size_t I, typename T, std::size_t kRank>
    constexpr T& get(VectorN<T, kRank>& rhs) noexcept;

    /// \brief Extracts the I-th element from the vector.I must be an integer value in[0, kRank).
    template <std::size_t I, typename T, std::size_t kRank>
    constexpr T&& get(VectorN<T, kRank>&& rhs) noexcept;

    /// \brief Extracts the I-th element from the vector.I must be an integer value in[0, kRank).
    template <std::size_t I, typename T, std::size_t kRank>
    constexpr const T& get(const VectorN<T, kRank>& rhs) noexcept;

    /// \brief Extracts the I-th element from the vector.I must be an integer value in[0, kRank).
    template <std::size_t I, typename T, std::size_t kRank>
    constexpr const T&& get(const VectorN<T, kRank>&& rhs) noexcept;

    /// \brief Equality comparison.
    template <typename T, typename U, size_t kRank>
    bool operator==(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Inequality comparison.
    template <typename T, typename U, size_t kRank>
    bool operator!=(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Sum a value to a vector.
    template <typename T, size_t kRank>
    auto operator+(const VectorN<T, kRank>& lhs, T rhs);

    /// \brief Sum a vector to a value.
    template <typename T, size_t kRank>
    auto operator+(T lhs, const VectorN<T, kRank>& rhs);

    /// \brief Subtract a value from a vector.
    template <typename T, size_t kRank>
    auto operator-(const VectorN<T, kRank>& lhs, T rhs);

    /// \brief Subtract a vector from a value.
    template <typename T, size_t kRank>
    auto operator-(T lhs, const VectorN<T, kRank>& rhs);

    /// \brief Multiply a vector by a value.
    template <typename T, size_t kRank>
    auto operator*(const VectorN<T, kRank>& lhs, T rhs);

    /// \brief Multiply a value by a vector.
    template <typename T, size_t kRank>
    auto operator*(T lhs, const VectorN<T, kRank>& rhs);

    /// \brief Divide a vector by a value.
    template <typename T, size_t kRank>
    auto operator/(const VectorN<T, kRank>& lhs, T rhs);

    /// \brief Divide a vector by a value.
    template <typename T, size_t kRank>
    auto operator/(T lhs, const VectorN<T, kRank>& rhs);

    /// \brief Modulus of a vector by a value.
    template <typename T, size_t kRank>
    auto operator%(const VectorN<T, kRank>& lhs, T rhs);

    /// \brief Modulus of a value by a vector.
    template <typename T, size_t kRank>
    auto operator%(T lhs, const VectorN<T, kRank>& rhs);

    /// \brief Vector sum.
    template <typename T, typename U, size_t kRank>
    auto operator+(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Vector difference.
    template <typename T, typename U, size_t kRank>
    auto operator-(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Vector negation.
    template <typename T, size_t kRank>
    auto operator-(const VectorN<T, kRank>& lhs);

    /// \brief Vector memberwise multiplication.
    template <typename T, typename U, size_t kRank>
    auto operator*(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Vector memberwise division.
    template <typename T, typename U, size_t kRank>
    auto operator/(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Vector memberwise modulus.
    template <typename T, typename U, size_t kRank>
    auto operator%(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Reinterpret a kInRank-ary vector as a kOutRank-ary one.
    template <std::size_t kOutRank, std::size_t kIndex, typename T, std::size_t kInRank>
    const VectorN<T, kOutRank>& AsVector(const VectorN<T, kInRank>& rhs);

    /// \brief Reinterpret a kInRank-ary vector as a kOutRank-ary one.
    template <std::size_t kOutRank, std::size_t kIndex, typename T, std::size_t kInRank>
    VectorN<T, kOutRank>& AsVector(VectorN<T, kInRank>& rhs);

    /// \brief Get a new vector by shuffling the elements of another vector.
    template <std::size_t I, std::size_t... Is, typename T, std::size_t kRank>
    decltype(auto) Shuffle(const VectorN<T, kRank>& rhs);

    /// \brief Get a new vector by shuffling the elements of another vector.
    template <std::size_t I, std::size_t... Is, typename T, std::size_t kRank>
    decltype(auto) Shuffle(VectorN<T, kRank>& rhs);

    /// \brief Append a vector to another vector.
    template <typename T, std::size_t kTRank, typename U, std::size_t kURank>
    auto Append(const VectorN<T, kTRank>& lhs, const VectorN<U, kURank>& rhs);

    /// \brief Append a value to a vector.
    template <typename T, std::size_t kRank>
    auto Append(const VectorN<T, kRank>& lhs, T rhs);

    /// \brief Append a value to a vector.
    template <typename T, std::size_t kRank>
    auto Append(T lhs, const VectorN<T, kRank>& rhs);

    /// \brief Get the the XY elements of a N-vector and return it as a 2-vector.
    /// \see Shuffle(rhs).
    template <typename TVector>
    decltype(auto) XY(TVector&& rhs);

    /// \brief Get the the ZW elements of a N-vector and return it as a 2-vector.
    /// \see Shuffle(rhs).
    template <typename TVector>
    decltype(auto) ZW(TVector&& rhs);

    /// \brief Get the the XYZ elements of a N-vector and return it as a 3-vector.
    /// \see Shuffle(rhs).
    template <typename TVector>
    decltype(auto) XYZ(TVector&& rhs);

    /// \brief Get the dot product of two vectors.
    template <typename T, typename U, size_t kRank>
    auto Dot(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Get a vector which is orthogonal to rhs.
    template <typename T>
    Vector2<T> Cross(const Vector2<T>& rhs);

    /// \brief Get the length of a vector which is orthogonal to both lhs and rhs.
    template <typename T>
    float Cross(const Vector2<T>& lhs, const Vector2<T>& rhs);

    /// \brief Get a vector which is orthogonal to both lhs and rhs.
    template <typename T>
    Vector3<T> Cross(const Vector3<T>& lhs, const Vector3<T>& rhs);

    /// \brief Get the euclidean length of a vector.
    template <typename T, size_t kRank>
    float Length(const VectorN<T, kRank>& rhs);

    /// \brief Get the squared euclidean length of a vector.
    template <typename T, size_t kRank>
    float SqrLength(const VectorN<T, kRank>& rhs);

    /// \brief Get the Manhattan length of a vector.
    template <typename T, size_t kRank>
    T ManhattanLength(const VectorN<T, kRank>& rhs);

    /// \brief Get the Chebyshev length of a vector.
    template <typename T, size_t kRank>
    T ChebyshevLength(const VectorN<T, kRank>& rhs);

    /// \brief Normalize rhs using the euclidean metric.
    template <typename T, size_t kRank>
    VectorN<T, kRank> Normalize(const VectorN<T, kRank>& rhs);

    /// \brief Get the euclidean distance between two vectors.
    template <typename T, typename U, size_t kRank>
    float Distance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Get the squared euclidean distance between two vectors.
    template <typename T, typename U, size_t kRank>
    float SqrDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Get the Manhattan distance between two vectors.
    template <typename T, typename U, size_t kRank>
    auto ManhattanDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /// \brief Get the Chebyshev distance between two vectors.
    template <typename T, typename U, size_t kRank>
    auto ChebyshevDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename T, size_t kRank>
    inline const VectorN<T, kRank> VectorN<T, kRank>::kZero = VectorN<T, kRank>(T(0));

    template <typename T, size_t kRank>
    inline const VectorN<T, kRank> VectorN<T, kRank>::kOne = VectorN<T, kRank>(T(1));

    template <typename T, size_t kRank>
    VectorN<T, kRank>::VectorN(uninitialized_t)
    {

    }

    template <typename T, size_t kRank>
    VectorN<T, kRank>::VectorN()
        : VectorN(kZero)
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
        static_assert(sizeof...(Ts) == kRank, "Vector rank mismatch.");
    }

    template <typename T, size_t kRank>
    template <typename U>
    VectorN<T, kRank>::VectorN(const VectorN<U, kRank>& rhs)
    {
        LockstepApply([](auto& lhs, auto rhs) { lhs = static_cast<T>(rhs); }, *this, rhs);
    }

    template <typename T, size_t kRank>
    constexpr T& VectorN<T, kRank>::operator[](size_t index) noexcept
    {
        SYNTROPY_ASSERT(index < kRank);

        return reinterpret_cast<T*>(this)[index];
    }

    template <typename T, size_t kRank>
    constexpr const T& VectorN<T, kRank>::operator[](size_t index) const noexcept
    {
        SYNTROPY_ASSERT(index < kRank);

        return reinterpret_cast<const T*>(this)[index];
    }

    template <typename T, size_t kRank>
    VectorN<T, kRank>& VectorN<T, kRank>::operator+=(T rhs)
    {
        LockstepApply([rhs](auto& lhs) { lhs += rhs; }, *this);

        return *this;
    }

    template <typename T, size_t kRank>
    VectorN<T, kRank>& VectorN<T, kRank>::operator-=(T rhs)
    {
        LockstepApply([rhs](auto& lhs) { lhs -= rhs; }, *this);

        return *this;
    }

    template <typename T, size_t kRank>
    VectorN<T, kRank>& VectorN<T, kRank>::operator*=(T rhs)
    {
        LockstepApply([rhs](auto& lhs) { lhs *= rhs; }, *this);

        return *this;
    }

    template <typename T, size_t kRank>
    VectorN<T, kRank>& VectorN<T, kRank>::operator/=(T rhs)
    {
        LockstepApply([rhs](auto& lhs) { lhs /= rhs; }, *this);

        return *this;
    }

    template <typename T, size_t kRank>
    VectorN<T, kRank>& VectorN<T, kRank>::operator%=(T rhs)
    {
        LockstepApply([rhs](auto& lhs) { lhs %= rhs; }, *this);

        return *this;
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
        LockstepApply([](auto& lhs, auto rhs) { lhs = Mod(lhs, static_cast<T>(rhs)); }, *this, rhs);

        return *this;
    }

    template <std::size_t I, typename T, std::size_t kRank>
    constexpr T& get(VectorN<T, kRank>& rhs) noexcept
    {
        return rhs[I];
    }

    template <std::size_t I, typename T, std::size_t kRank>
    constexpr T&& get(VectorN<T, kRank>&& rhs) noexcept
    {
        return rhs[I];
    }

    template <std::size_t I, typename T, std::size_t kRank>
    constexpr const T& get(const VectorN<T, kRank>& rhs) noexcept
    {
        return rhs[I];
    }

    template <std::size_t I, typename T, std::size_t kRank>
    constexpr const T&& get(const VectorN<T, kRank>&& rhs) noexcept
    {
        return rhs[I];
    }

    template <typename T, typename U, size_t kRank>
    bool operator==(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        auto result = true;

        LockstepApply([&result](auto l, auto r) { result &= (l == r); }, lhs, rhs);         // Avoid branching.

        return result;
    }

    template <typename T, typename U, size_t kRank>
    inline bool operator!=(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T, size_t kRank>
    auto operator+(const VectorN<T, kRank>& lhs, T rhs)
    {
        return VectorN<T, kRank>(lhs) += rhs;
    }

    template <typename T, size_t kRank>
    auto operator+(T lhs, const VectorN<T, kRank>& rhs)
    {
        return VectorN<T, kRank>(lhs) += rhs;
    }

    template <typename T, size_t kRank>
    auto operator-(const VectorN<T, kRank>& lhs, T rhs)
    {
        return VectorN<T, kRank>(lhs) -= rhs;
    }

    template <typename T, size_t kRank>
    auto operator-(T lhs, const VectorN<T, kRank>& rhs)
    {
        return VectorN<T, kRank>(lhs) -= rhs;
    }

    template <typename T, size_t kRank>
    auto operator*(const VectorN<T, kRank>& lhs, T rhs)
    {
        return VectorN<T, kRank>(lhs) *= rhs;
    }

    template <typename T, size_t kRank>
    auto operator*(T lhs, const VectorN<T, kRank>& rhs)
    {
        return VectorN<T, kRank>(lhs) *= rhs;
    }

    template <typename T, size_t kRank>
    auto operator/(const VectorN<T, kRank>& lhs, T rhs)
    {
        return VectorN<T, kRank>(lhs) /= rhs;
    }

    template <typename T, size_t kRank>
    auto operator/(T lhs, const VectorN<T, kRank>& rhs)
    {
        return VectorN<T, kRank>(lhs) /= rhs;
    }

    template <typename T, size_t kRank>
    auto operator%(const VectorN<T, kRank>& lhs, T rhs)
    {
        return VectorN<T, kRank>(lhs) %= rhs;
    }

    template <typename T, size_t kRank>
    auto operator%(T lhs, const VectorN<T, kRank>& rhs)
    {
        return VectorN<T, kRank>(lhs) %= rhs;
    }

    template <typename T, typename U, size_t kRank>
    inline auto operator+(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) += CommonVectorType(rhs);
    }

    template <typename T, typename U, size_t kRank>
    inline auto operator-(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) -= CommonVectorType(rhs);
    }

    template <typename T, size_t kRank>
    inline auto operator-(const VectorN<T, kRank>& lhs)
    {
        return VectorN<T, kRank>::kZero - lhs;
    }

    template <typename T, typename U, size_t kRank>
    inline auto operator*(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) *= CommonVectorType(rhs);
    }

    template <typename T, typename U, size_t kRank>
    inline auto operator/(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) /= CommonVectorType(rhs);
    }

    template <typename T, typename U, size_t kRank>
    inline auto operator%(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) %= CommonVectorType(rhs);
    }

    template <std::size_t kOutRank, std::size_t kIndex, typename T, std::size_t kInRank>
    inline const VectorN<T, kOutRank>& AsVector(const VectorN<T, kInRank>& rhs)
    {
        static_assert(kOutRank + kIndex <= kInRank, "The out rank must be equal or smaller than the in rank.");

        return *reinterpret_cast<const VectorN<T, kOutRank>*>(&rhs[kIndex]);
    }

    template <std::size_t kOutRank, std::size_t kIndex, typename T, std::size_t kInRank>
    inline VectorN<T, kOutRank>& AsVector(VectorN<T, kInRank>& rhs)
    {
        return AsNonConst(AsVector<kOutRank, kIndex>(std::as_const(rhs)));
    }

    template <std::size_t I, std::size_t... Is, typename T, std::size_t kRank>
    inline decltype(auto) Shuffle(const VectorN<T, kRank>& rhs)
    {
        static_assert((I < kRank) && ((Is < kRank) && ...), "Invalid element index. I and Is must be in [0; kRank).");

        constexpr std::size_t kOutRank = sizeof...(Is) + 1;

        if constexpr (!is_contiguous_index_sequence_v<I, Is...>)
        {
            return VectorN<T, kOutRank>{rhs[I], rhs[Is]...};
        }
        else
        {
            return AsVector<kOutRank, I>(rhs);
        }
    }

    template <std::size_t I, std::size_t... Is, typename T, std::size_t kRank>
    inline decltype(auto) Shuffle(VectorN<T, kRank>& rhs)
    {
        return AsNonConst(Shuffle<I, Is...>(std::as_const(rhs)));
    }

    template <typename T, std::size_t kTRank, typename U, std::size_t kURank>
    inline auto Append(const VectorN<T, kTRank>& lhs, const VectorN<U, kURank>& rhs)
    {
        auto out = VectorN<T, kTRank + kURank>(uninitialized);

        AsVector<kTRank, 0>(out) = lhs;
        AsVector<kURank, kTRank>(out) = rhs;

        return out;
    }

    template <typename T, std::size_t kRank>
    inline auto Append(const VectorN<T, kRank>& lhs, T rhs)
    {
        return Append(lhs, Vector1<T>{rhs});
    }

    template <typename T, std::size_t kRank>
    inline auto Append(T lhs, const VectorN<T, kRank>& rhs)
    {
        return Append(Vector1<T>{lhs}, rhs);
    }

    template <typename TVector>
    decltype(auto) XY(TVector&& rhs)
    {
        return Shuffle<0, 1>(std::forward<TVector>(rhs));
    }

    template <typename TVector>
    decltype(auto) ZW(TVector&& rhs)
    {
        return Shuffle<2, 3>(std::forward<TVector>(rhs));
    }

    template <typename TVector>
    decltype(auto) XYZ(TVector&& rhs)
    {
        return Shuffle<0, 1, 2>(std::forward<TVector>(rhs));
    }

    template <typename T, typename U, size_t kRank>
    auto Dot(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        auto dot = std::common_type_t<T, U>{};

        LockstepApply([&dot](auto l, auto r) { dot += l * r; }, lhs, rhs);

        return dot;
    }

    template <typename T>
    inline Vector2<T> Cross(const Vector2<T>& rhs)
    {
        return { rhs.y_, -rhs.x };
    }

    template <typename T>
    inline float Cross(const Vector2<T>& lhs, const Vector2<T>& rhs)
    {
        return lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_;
    }

    template <typename T>
    inline Vector3<T> Cross(const Vector3<T>& lhs, const Vector3<T>& rhs)
    {
        return { lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_, lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_, lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_ };
    }

    template <typename T, size_t kRank>
    inline float Length(const VectorN<T, kRank>& rhs)
    {
        return FastSqrt(SqrLength(rhs));
    }

    template <typename T, size_t kRank>
    inline float SqrLength(const VectorN<T, kRank>& rhs)
    {
        return Dot(rhs, rhs);
    }

    template <typename T, size_t kRank>
    T ManhattanLength(const VectorN<T, kRank>& rhs)
    {
        auto length = T{};

        LockstepApply([&length](auto r) { length += FastAbs(r); }, rhs);

        return length;
    }

    template <typename T, size_t kRank>
    VectorN<T, kRank> Normalize(const VectorN<T, kRank>& rhs)
    {
        return rhs / Length(rhs);
    }

    template <typename T, size_t kRank>
    T ChebyshevLength(const VectorN<T, kRank>& rhs)
    {
        auto length = T{};

        LockstepApply([&length](auto r) { length = std::max(length, FastAbs(r)); }, rhs);

        return length;
    }

    template <typename T, typename U, size_t kRank>
    inline float Distance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return Length(lhs - rhs);
    }

    template <typename T, typename U, size_t kRank>
    inline float SqrDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return SqrLength(lhs - rhs);
    }

    template <typename T, typename U, size_t kRank>
    inline auto ManhattanDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return ManhattanLength(lhs - rhs);
    }

    template <typename T, typename U, size_t kRank>
    inline auto ChebyshevDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return ChebyshevLength(lhs - rhs);
    }

}
