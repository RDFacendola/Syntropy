
/// \file vector.h
/// \brief This header is part of the Syntropy math module. It contains definitions for n-ary vectors.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <cstdint>

#include "syntropy/language/tuple.h"
#include "syntropy/language/utility.h"
#include "syntropy/language/foundation.h"
#include "syntropy/math/math.h"

namespace Syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Exposes vector-related classes and operations.
    /// \author Raffaele D. Facendola - 2018.
    namespace Math
    {

        /************************************************************************/
        /* MATH :: BASE VECTOR N                                                */
        /************************************************************************/

        /// \brief Base definition for n-ary vectors.
        template <typename T, Int kRank>
        struct BaseVectorN
        {
            /// \brief Elements in the vector.
            std::array<T, kRank> e_;
        };

        /// \brief Storage type for 1-vectors.
        template <typename T>
        struct BaseVectorN<T, 1>
        {
            /// \brief First element.
            T x_{};
        };

        /// \brief Storage type for 2-vectors.
        template <typename T>
        struct BaseVectorN<T, 2>
        {
            /// \brief First element.
            T x_{};

            /// \brief Second element.
            T y_{};
        };

        /// \brief Storage type for 3-vectors.
        template <typename T>
        struct BaseVectorN<T, 3>
        {
            /// \brief First element.
            T x_{};

            /// \brief Second element.
            T y_{};

            /// \brief Third element.
            T z_{};
        };

        /// \brief Storage type for 4-vectors.
        template <typename T>
        struct BaseVectorN<T, 4>
        {
            /// \brief First element.
            T x_{};

            /// \brief Second element.
            T y_{};

            /// \brief Third element.
            T z_{};

            /// \brief Fourth element.
            T w_{};
        };

        /************************************************************************/
        /* MATH :: VECTOR N                                                     */
        /************************************************************************/

        /// \brief Represents a kRank-vector of elements of type T.
        /// \author Raffaele D. Facendola - January 2018
        template <typename T, Int kRank>
        struct VectorN : BaseVectorN<T, kRank>
        {
            /// \brief Create a zero-vector.
            VectorN() = default;

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

        /************************************************************************/
        /* TYPE ALIAS                                                           */
        /************************************************************************/

        /// \brief Type alias for a generic 1-element vector.
        template <typename T>
        using Vector1 = VectorN<T, 1>;

        /// \brief Type alias for a generic 2-elements vector.
        template <typename T>
        using Vector2 = VectorN<T, 2>;

        /// \brief Type alias for a generic 3-elements vector.
        template <typename T>
        using Vector3 = VectorN<T, 3>;

        /// \brief Type alias for a generic 4-elements vector.
        template <typename T>
        using Vector4 = VectorN<T, 4>;

        /// \brief Type alias for 1-element Float vector.
        using Float1 = Vector1<Float>;

        /// \brief Type alias for 2-elements Float vector.
        using Float2 = Vector2<Float>;

        /// \brief Type alias for 3-elements Float vector.
        using Float3 = Vector3<Float>;

        /// \brief Type alias for 4-elements Float vector.
        using Float4 = Vector4<Float>;

        /// \brief Type alias for 1-element integer vector.
        using Int1 = Vector1<Int>;

        /// \brief Type alias for 2-elements integer vector.
        using Int2 = Vector2<Int>;

        /// \brief Type alias for 3-elements integer vector.
        using Int3 = Vector3<Int>;

        /// \brief Type alias for 4-elements integer vector.
        using Int4 = Vector4<Int>;

        /************************************************************************/
        /* MATH :: NON-MEMBER FUNCTIONS                                         */
        /************************************************************************/

        /// \brief Extracts the I-th element from the vector.I must be an integer value in[0, kRank).
        template <Int I, typename T, Int kRank>
        constexpr T& get(VectorN<T, kRank>& rhs) noexcept;

        /// \brief Extracts the I-th element from the vector.I must be an integer value in[0, kRank).
        template <Int I, typename T, Int kRank>
        constexpr T&& get(VectorN<T, kRank>&& rhs) noexcept;

        /// \brief Extracts the I-th element from the vector.I must be an integer value in[0, kRank).
        template <Int I, typename T, Int kRank>
        constexpr const T& get(const VectorN<T, kRank>& rhs) noexcept;

        /// \brief Extracts the I-th element from the vector.I must be an integer value in[0, kRank).
        template <Int I, typename T, Int kRank>
        constexpr const T&& get(const VectorN<T, kRank>&& rhs) noexcept;

        /// \brief Get an iterator to the first vector element.
        template <typename T, Int kRank>
        T* begin(VectorN<T, kRank>& rhs);

        /// \brief Get an iterator past the last vector element.
        template <typename T, Int kRank>
        T* end(VectorN<T, kRank>& rhs);

        /// \brief Get a const iterator to the first vector element.
        template <typename T, Int kRank>
        const T* begin(const VectorN<T, kRank>& rhs);

        /// \brief Get a const iterator past the last vector element.
        template <typename T, Int kRank>
        const T* end(const VectorN<T, kRank>& rhs);

        /// \brief Get a const iterator to the first vector element.
        template <typename T, Int kRank>
        const T* cbegin(const VectorN<T, kRank>& rhs);

        /// \brief Get a const iterator past the last vector element.
        template <typename T, Int kRank>
        const T* cend(const VectorN<T, kRank>& rhs);

        /// \brief Equality comparison.
        template <typename T, typename U, Int kRank>
        Bool operator==(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Inequality comparison.
        template <typename T, typename U, Int kRank>
        Bool operator!=(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Sum a value to a vector.
        template <typename T, Int kRank>
        auto operator+(const VectorN<T, kRank>& lhs, T rhs);

        /// \brief Sum a vector to a value.
        template <typename T, Int kRank>
        auto operator+(T lhs, const VectorN<T, kRank>& rhs);

        /// \brief Subtract a value from a vector.
        template <typename T, Int kRank>
        auto operator-(const VectorN<T, kRank>& lhs, T rhs);

        /// \brief Subtract a vector from a value.
        template <typename T, Int kRank>
        auto operator-(T lhs, const VectorN<T, kRank>& rhs);

        /// \brief Multiply a vector by a value.
        template <typename T, Int kRank>
        auto operator*(const VectorN<T, kRank>& lhs, T rhs);

        /// \brief Multiply a value by a vector.
        template <typename T, Int kRank>
        auto operator*(T lhs, const VectorN<T, kRank>& rhs);

        /// \brief Divide a vector by a value.
        template <typename T, Int kRank>
        auto operator/(const VectorN<T, kRank>& lhs, T rhs);

        /// \brief Divide a vector by a value.
        template <typename T, Int kRank>
        auto operator/(T lhs, const VectorN<T, kRank>& rhs);

        /// \brief Modulus of a vector by a value.
        template <typename T, Int kRank>
        auto operator%(const VectorN<T, kRank>& lhs, T rhs);

        /// \brief Modulus of a value by a vector.
        template <typename T, Int kRank>
        auto operator%(T lhs, const VectorN<T, kRank>& rhs);

        /// \brief Vector sum.
        template <typename T, typename U, Int kRank>
        auto operator+(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Vector difference.
        template <typename T, typename U, Int kRank>
        auto operator-(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Vector negation.
        template <typename T, Int kRank>
        auto operator-(const VectorN<T, kRank>& lhs);

        /// \brief Vector member-wise multiplication.
        template <typename T, typename U, Int kRank>
        auto operator*(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Vector member-wise division.
        template <typename T, typename U, Int kRank>
        auto operator/(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Vector member-wise modulus.
        template <typename T, typename U, Int kRank>
        auto operator%(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Reinterpret a kInRank-ary vector as a kOutRank-ary one.
        template <Int kOutRank, Int kIndex, typename T, Int kInRank>
        const VectorN<T, kOutRank>& AsVector(const VectorN<T, kInRank>& rhs);

        /// \brief Reinterpret a kInRank-ary vector as a kOutRank-ary one.
        template <Int kOutRank, Int kIndex, typename T, Int kInRank>
        VectorN<T, kOutRank>& AsVector(VectorN<T, kInRank>& rhs);

        /// \brief Get a new vector by shuffling the elements of another vector.
        template <Int I, Int... Is, typename T, Int kRank>
        decltype(auto) Shuffle(const VectorN<T, kRank>& rhs);

        /// \brief Get a new vector by shuffling the elements of another vector.
        template <Int I, Int... Is, typename T, Int kRank>
        decltype(auto) Shuffle(VectorN<T, kRank>& rhs);

        /// \brief Append a vector to another vector.
        template <typename T, Int kTRank, typename U, Int kURank>
        auto Append(const VectorN<T, kTRank>& lhs, const VectorN<U, kURank>& rhs);

        /// \brief Append a value to a vector.
        template <typename T, Int kRank, typename U>
        auto Append(const VectorN<T, kRank>& lhs, U rhs);

        /// \brief Append a value to a vector.
        template <typename T, Int kRank, typename U>
        auto Append(U lhs, const VectorN<T, kRank>& rhs);

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
        template <typename T, typename U, Int kRank>
        auto Dot(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Get a vector which is orthogonal to rhs.
        template <typename T>
        Vector2<T> Cross(const Vector2<T>& rhs);

        /// \brief Get the length of a vector which is orthogonal to both lhs and rhs.
        template <typename T>
        Float Cross(const Vector2<T>& lhs, const Vector2<T>& rhs);

        /// \brief Get a vector which is orthogonal to both lhs and rhs.
        template <typename T>
        Vector3<T> Cross(const Vector3<T>& lhs, const Vector3<T>& rhs);

        /// \brief Get the euclidean length of a vector.
        template <typename T, Int kRank>
        Float Length(const VectorN<T, kRank>& rhs);

        /// \brief Get the squared euclidean length of a vector.
        template <typename T, Int kRank>
        Float LengthSqr(const VectorN<T, kRank>& rhs);

        /// \brief Get the Manhattan length of a vector.
        template <typename T, Int kRank>
        T ManhattanLength(const VectorN<T, kRank>& rhs);

        /// \brief Get the Chebyshev length of a vector.
        template <typename T, Int kRank>
        T ChebyshevLength(const VectorN<T, kRank>& rhs);

        /// \brief Normalize rhs using the euclidean metric.
        template <typename T, Int kRank>
        VectorN<T, kRank> Normalize(const VectorN<T, kRank>& rhs);

        /// \brief Check whether a vector is normalized.
        template <typename T, Int kRank>
        Bool IsNormalized(const VectorN<T, kRank>& rhs, Float epsilon = 0.01f);

        /// \brief Get the euclidean distance between two vectors.
        template <typename T, typename U, Int kRank>
        Float Distance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Get the squared euclidean distance between two vectors.
        template <typename T, typename U, Int kRank>
        Float DistanceSqr(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Get the Manhattan distance between two vectors.
        template <typename T, typename U, Int kRank>
        auto ManhattanDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Get the Chebyshev distance between two vectors.
        template <typename T, typename U, Int kRank>
        auto ChebyshevDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs);

        /// \brief Get the element-wise absolute of rhs.
        template <typename T, Int kRank>
        VectorN<T, kRank> Abs(const VectorN<T, kRank>& rhs);

        /// \brief Get the element-wise wrap of lhs in the range [0; rhs).
        template <typename T, Int kRank>
        VectorN<T, kRank> Wrap(const VectorN<T, kRank>& lhs, const VectorN<T, kRank>& rhs);

        /// \brief Get the member-wise ceil of lhs.
        template <typename T, Int kRank>
        VectorN<T, kRank> Ceil(const VectorN<T, kRank>& rhs);

        /// \brief Get the member-wise floor of lhs.
        template <typename T, Int kRank>
        VectorN<T, kRank> Floor(const VectorN<T, kRank>& rhs);

        /// \brief Stream insertion for VectorN.
        template <typename T, Int kRank>
        std::ostream& operator<<(std::ostream& out, const VectorN<T, kRank>& vector);

        /// \brief Stream extraction for VectorN.
        template <typename T, Int kRank>
        std::istream& operator>>(std::istream& in, VectorN<T, kRank>& vector);

    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Specialization of LockstepRankV for VectorN.
    template <Int kRank, typename... Ts>
    constexpr Int LockstepRankV<Math::VectorN<Ts, kRank>...> = kRank;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Math::VectorN<T, kRank>.

    template <typename T, Int kRank>
    Math::VectorN<T, kRank>::VectorN(T value)
    {
        LockstepApply([value](auto& rhs) { rhs = value; }, *this);
    }

    template <typename T, Int kRank>
    template <typename... Ts>
    Math::VectorN<T, kRank>::VectorN(Ts... ts)
        : BaseVectorN<T, kRank>{ ts... }
    {
        static_assert(sizeof...(Ts) == kRank, "Vector rank mismatch.");
    }

    template <typename T, Int kRank>
    template <typename U>
    Math::VectorN<T, kRank>::VectorN(const VectorN<U, kRank>& rhs)
    {
        LockstepApply([](auto& lhs, auto rhs) { lhs = static_cast<T>(rhs); }, *this, rhs);
    }

    template <typename T, Int kRank>
    constexpr T& Math::VectorN<T, kRank>::operator[](size_t index) noexcept
    {
        return reinterpret_cast<T*>(this)[index];
    }

    template <typename T, Int kRank>
    constexpr const T& Math::VectorN<T, kRank>::operator[](size_t index) const noexcept
    {
        return reinterpret_cast<const T*>(this)[index];
    }

    template <typename T, Int kRank>
    Math::VectorN<T, kRank>& Math::VectorN<T, kRank>::operator+=(T rhs)
    {
        LockstepApply([rhs](auto& lhs) { lhs += rhs; }, *this);

        return *this;
    }

    template <typename T, Int kRank>
    Math::VectorN<T, kRank>& Math::VectorN<T, kRank>::operator-=(T rhs)
    {
        LockstepApply([rhs](auto& lhs) { lhs -= rhs; }, *this);

        return *this;
    }

    template <typename T, Int kRank>
    Math::VectorN<T, kRank>& Math::VectorN<T, kRank>::operator*=(T rhs)
    {
        LockstepApply([rhs](auto& lhs) { lhs *= rhs; }, *this);

        return *this;
    }

    template <typename T, Int kRank>
    Math::VectorN<T, kRank>& Math::VectorN<T, kRank>::operator/=(T rhs)
    {
        LockstepApply([rhs](auto& lhs) { lhs /= rhs; }, *this);

        return *this;
    }

    template <typename T, Int kRank>
    Math::VectorN<T, kRank>& Math::VectorN<T, kRank>::operator%=(T rhs)
    {
        LockstepApply([rhs](auto& lhs) { lhs %= rhs; }, *this);

        return *this;
    }

    template <typename T, Int kRank>
    template <typename U>
    Math::VectorN<T, kRank>& Math::VectorN<T, kRank>::operator+=(const VectorN<U, kRank>& rhs)
    {
        LockstepApply([](auto& lhs, auto rhs) { lhs += static_cast<T>(rhs); }, *this, rhs);

        return *this;
    }

    template <typename T, Int kRank>
    template <typename U>
    Math::VectorN<T, kRank>& Math::VectorN<T, kRank>::operator-=(const VectorN<U, kRank>& rhs)
    {
        LockstepApply([](auto& lhs, auto rhs) { lhs -= static_cast<T>(rhs); }, *this, rhs);

        return *this;
    }

    template <typename T, Int kRank>
    template <typename U>
    Math::VectorN<T, kRank>& Math::VectorN<T, kRank>::operator*=(const VectorN<U, kRank>& rhs)
    {
        LockstepApply([](auto& lhs, auto rhs) {lhs *= static_cast<T>(rhs); }, *this, rhs);

        return *this;
    }

    template <typename T, Int kRank>
    template <typename U>
    Math::VectorN<T, kRank>& Math::VectorN<T, kRank>::operator/=(const VectorN<U, kRank>& rhs)
    {
        LockstepApply([](auto& lhs, auto rhs) {lhs /= static_cast<T>(rhs); }, *this, rhs);

        return *this;
    }

    template <typename T, Int kRank>
    template <typename U>
    Math::VectorN<T, kRank>& Math::VectorN<T, kRank>::operator%=(const VectorN<U, kRank>& rhs)
    {
        LockstepApply([](auto& lhs, auto rhs) { lhs = Mod(lhs, static_cast<T>(rhs)); }, *this, rhs);

        return *this;
    }

    // Non-member functions.

    template <Int I, typename T, Int kRank>
    constexpr T& Math::get(VectorN<T, kRank>& rhs) noexcept
    {
        return rhs[I];
    }

    template <Int I, typename T, Int kRank>
    constexpr T&& Math::get(VectorN<T, kRank>&& rhs) noexcept
    {
        return rhs[I];
    }

    template <Int I, typename T, Int kRank>
    constexpr const T& Math::get(const VectorN<T, kRank>& rhs) noexcept
    {
        return rhs[I];
    }

    template <Int I, typename T, Int kRank>
    constexpr const T&& Math::get(const VectorN<T, kRank>&& rhs) noexcept
    {
        return rhs[I];
    }

    template <typename T, Int kRank>
    inline T* Math::begin(VectorN<T, kRank>& rhs)
    {
        return &rhs[0];
    }

    template <typename T, Int kRank>
    inline T* Math::end(VectorN<T, kRank>& rhs)
    {
        return begin(rhs) + kRank;
    }

    template <typename T, Int kRank>
    inline const T* Math::begin(const VectorN<T, kRank>& rhs)
    {
        return &rhs[0];
    }

    template <typename T, Int kRank>
    inline const T* Math::end(const VectorN<T, kRank>& rhs)
    {
        return begin(rhs) + kRank;
    }

    template <typename T, Int kRank>
    inline const T* Math::cbegin(const VectorN<T, kRank>& rhs)
    {
        return &rhs[0];
    }

    template <typename T, Int kRank>
    inline const T* Math::cend(const VectorN<T, kRank>& rhs)
    {
        return cbegin(rhs) + kRank;
    }

    template <typename T, typename U, Int kRank>
    Bool Math::operator==(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        auto result = true;

        LockstepApply([&result](auto l, auto r) { result &= (l == r); }, lhs, rhs);         // Avoid branching.

        return result;
    }

    template <typename T, typename U, Int kRank>
    inline Bool Math::operator!=(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T, Int kRank>
    inline auto Math::operator+(const VectorN<T, kRank>& lhs, T rhs)
    {
        return VectorN<T, kRank>(lhs) += rhs;
    }

    template <typename T, Int kRank>
    inline auto Math::operator+(T lhs, const VectorN<T, kRank>& rhs)
    {
        return VectorN<T, kRank>(lhs) += rhs;
    }

    template <typename T, Int kRank>
    inline auto Math::operator-(const VectorN<T, kRank>& lhs, T rhs)
    {
        return VectorN<T, kRank>(lhs) -= rhs;
    }

    template <typename T, Int kRank>
    inline auto Math::operator-(T lhs, const VectorN<T, kRank>& rhs)
    {
        return VectorN<T, kRank>(lhs) -= rhs;
    }

    template <typename T, Int kRank>
    inline auto Math::operator*(const VectorN<T, kRank>& lhs, T rhs)
    {
        return VectorN<T, kRank>(lhs) *= rhs;
    }

    template <typename T, Int kRank>
    inline auto Math::operator*(T lhs, const VectorN<T, kRank>& rhs)
    {
        return VectorN<T, kRank>(lhs) *= rhs;
    }

    template <typename T, Int kRank>
    inline auto Math::operator/(const VectorN<T, kRank>& lhs, T rhs)
    {
        return VectorN<T, kRank>(lhs) /= rhs;
    }

    template <typename T, Int kRank>
    inline auto Math::operator/(T lhs, const VectorN<T, kRank>& rhs)
    {
        return VectorN<T, kRank>(lhs) /= rhs;
    }

    template <typename T, Int kRank>
    inline auto Math::operator%(const VectorN<T, kRank>& lhs, T rhs)
    {
        return VectorN<T, kRank>(lhs) %= rhs;
    }

    template <typename T, Int kRank>
    inline auto Math::operator%(T lhs, const VectorN<T, kRank>& rhs)
    {
        return VectorN<T, kRank>(lhs) %= rhs;
    }

    template <typename T, typename U, Int kRank>
    inline auto Math::operator+(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) += CommonVectorType(rhs);
    }

    template <typename T, typename U, Int kRank>
    inline auto Math::operator-(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) -= CommonVectorType(rhs);
    }

    template <typename T, Int kRank>
    inline auto Math::operator-(const VectorN<T, kRank>& lhs)
    {
        return VectorN<T, kRank>(T(0)) - lhs;
    }

    template <typename T, typename U, Int kRank>
    inline auto Math::operator*(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) *= CommonVectorType(rhs);
    }

    template <typename T, typename U, Int kRank>
    inline auto Math::operator/(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) /= CommonVectorType(rhs);
    }

    template <typename T, typename U, Int kRank>
    inline auto Math::operator%(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        using CommonVectorType = VectorN<std::common_type_t<T, U>, kRank>;

        return CommonVectorType(lhs) %= CommonVectorType(rhs);
    }

    template <Int kOutRank, Int kIndex, typename T, Int kInRank>
    inline const Math::VectorN<T, kOutRank>& Math::AsVector(const VectorN<T, kInRank>& rhs)
    {
        static_assert(kOutRank + kIndex <= kInRank, "Out rank must be equal or smaller than in rank.");

        return *reinterpret_cast<const VectorN<T, kOutRank>*>(&rhs[kIndex]);
    }

    template <Int kOutRank, Int kIndex, typename T, Int kInRank>
    inline Math::VectorN<T, kOutRank>& Math::AsVector(VectorN<T, kInRank>& rhs)
    {
        return ReadWrite(AsVector<kOutRank, kIndex>(ReadOnly(rhs)));
    }

    template <Int I, Int... Is, typename T, Int kRank>
    inline decltype(auto) Math::Shuffle(const VectorN<T, kRank>& rhs)
    {
        static_assert((I < kRank) && ((Is < kRank) && ...), "Invalid element index. I and Is must be in [0; kRank).");

        constexpr Int kOutRank = sizeof...(Is) + 1;

        if constexpr (!IsContiguousSequence<I, Is...>)
        {
            return VectorN<T, kOutRank>{rhs[I], rhs[Is]...};
        }
        else
        {
            return AsVector<kOutRank, I>(rhs);
        }
    }

    template <Int I, Int... Is, typename T, Int kRank>
    inline decltype(auto) Math::Shuffle(VectorN<T, kRank>& rhs)
    {
        return ReadWrite(Shuffle<I, Is...>(ReadOnly(rhs)));
    }

    template <typename T, Int kTRank, typename U, Int kURank>
    inline auto Math::Append(const VectorN<T, kTRank>& lhs, const VectorN<U, kURank>& rhs)
    {
        using CommonType = std::common_type_t<T, U>;

        auto out = VectorN<CommonType, kTRank + kURank>{};

        AsVector<kTRank, 0>(out) = VectorN<CommonType, kTRank>(lhs);
        AsVector<kURank, kTRank>(out) = VectorN<CommonType, kURank>(rhs);

        return out;
    }

    template <typename T, Int kRank, typename U>
    inline auto Math::Append(const VectorN<T, kRank>& lhs, U rhs)
    {
        return Append(lhs, Vector1<U>{rhs});
    }

    template <typename T, Int kRank, typename U>
    inline auto Math::Append(U lhs, const VectorN<T, kRank>& rhs)
    {
        return Append(Vector1<U>{lhs}, rhs);
    }

    template <typename TVector>
    inline decltype(auto) Math::XY(TVector&& rhs)
    {
        return Shuffle<0, 1>(Forward<TVector>(rhs));
    }

    template <typename TVector>
    inline decltype(auto) Math::ZW(TVector&& rhs)
    {
        return Shuffle<2, 3>(Forward<TVector>(rhs));
    }

    template <typename TVector>
    inline decltype(auto) Math::XYZ(TVector&& rhs)
    {
        return Shuffle<0, 1, 2>(Forward<TVector>(rhs));
    }

    template <typename T, typename U, Int kRank>
    inline auto Math::Dot(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        auto dot = std::common_type_t<T, U>{};

        LockstepApply([&dot](auto l, auto r) { dot += l * r; }, lhs, rhs);

        return dot;
    }

    template <typename T>
    inline Math::Vector2<T> Math::Cross(const Vector2<T>& rhs)
    {
        return { rhs.y_, -rhs.x };
    }

    template <typename T>
    inline Float Math::Cross(const Vector2<T>& lhs, const Vector2<T>& rhs)
    {
        return lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_;
    }

    template <typename T>
    inline Math::Vector3<T> Math::Cross(const Vector3<T>& lhs, const Vector3<T>& rhs)
    {
        return { lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_, lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_, lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_ };
    }

    template <typename T, Int kRank>
    inline Float Math::Length(const VectorN<T, kRank>& rhs)
    {
        return FastSqrt(LengthSqr(rhs));
    }

    template <typename T, Int kRank>
    inline Float Math::LengthSqr(const VectorN<T, kRank>& rhs)
    {
        return Dot(rhs, rhs);
    }

    template <typename T, Int kRank>
    T Math::ManhattanLength(const VectorN<T, kRank>& rhs)
    {
        auto length = T{};

        LockstepApply([&length](auto r) { length += Abs(r); }, rhs);

        return length;
    }

    template <typename T, Int kRank>
    inline Math::VectorN<T, kRank> Math::Normalize(const VectorN<T, kRank>& rhs)
    {
        return rhs / Length(rhs);
    }

    template <typename T, Int kRank>
    inline Bool Math::IsNormalized(const VectorN<T, kRank>& rhs, Float epsilon)
    {
        return Abs(LengthSqr(rhs)) < epsilon;
    }

    template <typename T, Int kRank>
    T Math::ChebyshevLength(const VectorN<T, kRank>& rhs)
    {
        auto length = T{};

        LockstepApply([&length](auto r) { length = std::max(length, Abs(r)); }, rhs);

        return length;
    }

    template <typename T, typename U, Int kRank>
    inline Float Math::Distance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return Length(lhs - rhs);
    }

    template <typename T, typename U, Int kRank>
    inline Float Math::DistanceSqr(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return LengthSqr(lhs - rhs);
    }

    template <typename T, typename U, Int kRank>
    inline auto Math::ManhattanDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return ManhattanLength(lhs - rhs);
    }

    template <typename T, typename U, Int kRank>
    inline auto Math::ChebyshevDistance(const VectorN<T, kRank>& lhs, const VectorN<U, kRank>& rhs)
    {
        return ChebyshevLength(lhs - rhs);
    }

    template <typename T, Int kRank>
    inline Math::VectorN<T, kRank> Math::Abs(const VectorN<T, kRank>& rhs)
    {
        auto result = rhs;

        LockstepApply([](auto& element) { element = Abs(element); }, result);

        return result;
    }

    template <typename T, Int kRank>
    inline Math::VectorN<T, kRank> Math::Wrap(const VectorN<T, kRank>& lhs, const VectorN<T, kRank>& rhs)
    {
        auto result = lhs;

        LockstepApply([](auto& element, auto& wrap) { element = Wrap(element, wrap); }, result, rhs);

        return result;
    }

    template <typename T, Int kRank>
    inline Math::VectorN<T, kRank> Math::Ceil(const VectorN<T, kRank>& rhs)
    {
        auto result = rhs;

        LockstepApply([](auto& element) { element = std::ceil(element); }, result);

        return result;
    }

    template <typename T, Int kRank>
    inline Math::VectorN<T, kRank> Math::Floor(const VectorN<T, kRank>& rhs)
    {
        auto result = rhs;

        LockstepApply([](auto& element) { element = std::floor(element); }, result);

        return result;
    }

    template <typename T, Int kRank>
    inline std::ostream& Math::operator<<(std::ostream& out, const VectorN<T, kRank>& vector)
    {
        for (auto&& element : vector)
        {
            out << element << " ";
        }

        return out;
    }

    template <typename T, Int kRank>
    inline std::istream& Math::operator>>(std::istream& in, VectorN<T, kRank>& vector)
    {
        for (auto&& element : vector)
        {
            in >> element;
        }

        return in;
    }

}
