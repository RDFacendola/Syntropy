
/// \file quaternion.h
/// \brief This header is part of the Syntropy math module. It contains definitions for quaternions.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/diagnostics/foundation/assert.h"
#include "syntropy/math/math.h"
#include "syntropy/math/numeric.h"
#include "syntropy/math/vector.h"

namespace Syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Exposes quaternion-related classes and operations.
    /// \author Raffaele D. Facendola - 2019.
    namespace Math
    {
        /************************************************************************/
        /* QUATERNION                                                           */
        /************************************************************************/

        /// \brief A quaternion used to represent rotations in 3D space.
        /// \author Raffaele D. Facendola - January 2019
        struct Quaternion
        {
            /// \brief Imaginary part of the quaternion.
            Float3 xyz_;

            /// \brief IsReal part of the quaternion.
            Float w_{};

            /// \brief Create an identity quaternion.
            Quaternion() = default;

            /// \brief Create a quaternion from explicit values.
            Quaternion(Float x, Float y, Float z, Float w);

            /// \brief Create a quaternion from explicit values.
            Quaternion(const Float3& xyz, Float w);

            /// \brief Sum a quaternion to this one.
            Quaternion& operator+=(const Quaternion& rhs);

            /// \brief Subtract a quaternion from this one.
            Quaternion& operator-=(const Quaternion& rhs);

            /// \brief Member-wise multiplication of this quaternion by a scalar.
            Quaternion& operator*=(Float rhs);

            /// \brief Multiply this quaternion by another one.
            Quaternion& operator*=(const Quaternion& rhs);

            /// \brief Member-wise division of this quaternion by a scalar.
            Quaternion& operator/=(Float rhs);

        };

        /************************************************************************/
        /* NON-MEMBER FUNCTIONS                                                 */
        /************************************************************************/

        /// \brief Sum two quaternions.
        Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs);

        /// \brief Difference between two quaternions.
        Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs);

        /// \brief Multiply two quaternions.
        Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);

        /// \brief Multiply a quaternion by a scalar.
        Quaternion operator*(const Quaternion& lhs, Float rhs);

        /// \brief Divide a quaternion by a scalar.
        Quaternion operator/(const Quaternion& lhs, Float rhs);

        /// \brief Multiply a scalar by a quaternion.
        Quaternion operator*(Float lhs, const Quaternion& rhs);

        /// \brief Get the conjugate of a quaternion.
        Quaternion Conjugate(const Quaternion& rhs);

        /// \brief Get the inverse of a quaternion.
        Quaternion Inverse(const Quaternion& rhs);

        /// \brief Get the norm of a quaternion.
        Float Norm(const Quaternion& rhs);

        /// \brief Get the squared norm of a quaternion.
        Float SqrNorm(const Quaternion& rhs);

        /// \brief Get a normalized quaternion.
        Quaternion Normalize(const Quaternion& rhs);

        /// \brief Check whether a quaternion is a unit quaternion.
        Bool IsNormalized(const Quaternion& rhs, Float epsilon = 0.01f);

    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Quaternion.

    inline Math::Quaternion::Quaternion(Float x, Float y, Float z, Float w)
        : Quaternion({ x, y, z }, w)
    {

    }

    inline Math::Quaternion::Quaternion(const Float3& xyz, Float w)
        : xyz_(xyz)
        , w_(w)
    {

    }

    inline Math::Quaternion& Math::Quaternion::operator+=(const Quaternion& rhs)
    {
        xyz_ += rhs.xyz_;
        w_ += rhs.w_;

        return *this;
    }

    inline Math::Quaternion& Math::Quaternion::operator-=(const Quaternion& rhs)
    {
        xyz_ -= rhs.xyz_;
        w_ -= rhs.w_;

        return *this;
    }

    inline Math::Quaternion& Math::Quaternion::operator*=(Float rhs)
    {
        xyz_ *= rhs;
        w_ *= rhs;

        return *this;
    }

    inline Math::Quaternion& Math::Quaternion::operator*=(const Quaternion& rhs)
    {
        xyz_ = Cross(xyz_, rhs.xyz_) + rhs.w_ * xyz_ + w_ * rhs.xyz_;
        w_ = w_ * rhs.w_ - Dot(xyz_, rhs.xyz_);

        return *this;
    }

    inline Math::Quaternion& Math::Quaternion::operator/=(Float rhs)
    {
        xyz_ /= rhs;
        w_ /= rhs;

        return *this;
    }

    // Non-member functions.

    inline Math::Quaternion Math::operator+(const Quaternion& lhs, const Quaternion& rhs)
    {
        return Quaternion(lhs) += rhs;
    }

    inline Math::Quaternion Math::operator-(const Quaternion& lhs, const Quaternion& rhs)
    {
        return Quaternion(lhs) -= rhs;
    }

    inline Math::Quaternion Math::operator*(const Quaternion& lhs, const Quaternion& rhs)
    {
        return Quaternion(lhs) *= rhs;
    }

    inline Math::Quaternion Math::operator*(const Quaternion& lhs, Float rhs)
    {
        return Quaternion(lhs) *= rhs;
    }

    inline Math::Quaternion Math::operator*(Float lhs, const Quaternion& rhs)
    {
        return Quaternion(rhs) *= lhs;
    }

    inline Math::Quaternion Math::operator/(const Quaternion& lhs, Float rhs)
    {
        return Quaternion(lhs) /= rhs;
    }

    inline Math::Quaternion Math::Conjugate(const Quaternion& rhs)
    {
        return { -rhs.xyz_, rhs.w_ };
    }

    inline Math::Quaternion Math::Inverse(const Quaternion& rhs)
    {
        return Conjugate(rhs) / SqrNorm(rhs);
    }

    inline Float Math::Norm(const Quaternion& rhs)
    {
        return Math::FastSqrt(SqrNorm(rhs));
    }

   inline Float Math::SqrNorm(const Quaternion& rhs)
    {
        return Dot(rhs.xyz_, rhs.xyz_) + rhs.w_ * rhs.w_;
    }

   inline Math::Quaternion Math::Normalize(const Quaternion& rhs)
   {
       return rhs / Norm(rhs);
   }

   inline Bool Math::IsNormalized(const Quaternion& rhs, Float epsilon)
   {
       return SqrNorm(rhs) > (1.0f - epsilon);
   }

}
