
/// \file quaternion.h
/// \brief This header is part of the syntropy math system. It contains classes used to handle quaternions.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/diagnostics/assert.h"
#include "syntropy/math/math.h"
#include "syntropy/math/vector.h"
#include "syntropy/patterns/utility.h"

namespace syntropy
{

    /************************************************************************/
    /* QUATERNION                                                           */
    /************************************************************************/

    /// \brief A quaternion used to represent rotation in 3D space.
    /// \author Raffaele D. Facendola - January 2019
    struct Quaternion
    {
        /// \brief Quaternion which represents no rotation.
        static const Quaternion kIdentity;

        /// \brief Imaginary part of the quaternion.
        Float3 xyz_;

        /// \brief Real part of the quaternion.
        float w_;

        /// \brief Create an uninitialized quaternion.
        Quaternion(uninitialized_t);

        /// \brief Create an identity quaternion.
        Quaternion();

        /// \brief Create a quaternion from explicit values.
        Quaternion(const Float3& xyz, float w);

        /// \brief Sum a quaternion to this one.
        Quaternion& operator+=(const Quaternion& rhs);

        /// \brief Multiply this quaternion by a scalar.
        Quaternion& operator*=(float rhs);

        /// \brief Multiply this quaternion by another one.
        Quaternion& operator*=(const Quaternion& rhs);

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Sum two quaternions.
    Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs);

    /// \brief Multiply two quaternions.
    Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);

    /// \brief Multiply a quaternion by a scalar.
    Quaternion operator*(const Quaternion& lhs, float rhs);

    /// \brief Multiply a scalar by a quaternion.
    Quaternion operator*(float lhs, const Quaternion& rhs);

    /// \brief Get the conjugate of a quaternion.
    Quaternion Conjugate(const Quaternion& rhs);

    /// \brief Get the inverse of a quaternion.
    Quaternion Inverse(const Quaternion& rhs);

    /// \brief Get the norm of a quaternion.
    float Norm(const Quaternion& rhs);

    /// \brief Get the squared norm of a quaternion.
    float SqrNorm(const Quaternion& rhs);

    /// \brief Get a quaternion from an axis of rotation and an angle.
    Quaternion MakeRotation(const Float3& axis, float angle);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline const Quaternion Quaternion::kIdentity = Quaternion(Float3::kZero, 0.0f);

    inline Quaternion::Quaternion(uninitialized_t)
        : xyz_(uninitialized)
    {

    }

    inline Quaternion::Quaternion()
        : Quaternion(kIdentity)
    {

    }

    inline Quaternion::Quaternion(const Float3& xyz, float w)
        : xyz_(xyz)
        , w_(w)
    {

    }

    inline Quaternion& Quaternion::operator+=(const Quaternion& rhs)
    {
        xyz_ += rhs.xyz_;
        w_ += rhs.w_;

        return *this;
    }

    inline Quaternion& Quaternion::operator*=(float rhs)
    {
        xyz_ *= rhs;
        w_ *= rhs;

        return *this;
    }

    inline Quaternion& Quaternion::operator*=(const Quaternion& rhs)
    {
        xyz_ = Cross(xyz_, rhs.xyz_) + rhs.w_ * xyz_ + w_ * rhs.xyz_;
        w_ = w_ * rhs.w_ - Dot(xyz_, rhs.xyz_);

        return *this;
    }

    inline Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
    {
        return Quaternion(lhs) += rhs;
    }

    inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
    {
        return Quaternion(lhs) *= rhs;
    }

    inline Quaternion operator*(const Quaternion& lhs, float rhs)
    {
        return Quaternion(lhs) *= rhs;
    }

    inline Quaternion operator*(float lhs, const Quaternion& rhs)
    {
        return Quaternion(rhs) *= lhs;
    }

    inline Quaternion Conjugate(const Quaternion& rhs)
    {
        return { -rhs.xyz_, rhs.w_ };
    }

    inline Quaternion Inverse(const Quaternion& rhs)
    {
        return (1.0f / SqrNorm(rhs)) * Conjugate(rhs);
    }

    inline float Norm(const Quaternion& rhs)
    {
        return FastSqrt(SqrNorm(rhs));
    }

   inline float SqrNorm(const Quaternion& rhs)
    {
        return Dot(rhs.xyz_, rhs.xyz_) + rhs.w_ * rhs.w_;
    }

   inline Quaternion MakeRotation(const Float3& axis, float angle)
   {
       auto theta = angle * 0.5f;

       return { std::sin(theta) * Normalize(axis), std::cos(theta) };
   }

}
