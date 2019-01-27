
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
        /// \brief Imaginary part of the quaternion.
        Float3 xyz_;

        /// \brief Real part of the quaternion.
        float w_{};

        /// \brief Create an identity quaternion.
        Quaternion() = default;

        /// \brief Create a quaternion from explicit values.
        Quaternion(float x, float y, float z, float w);

        /// \brief Create a quaternion from explicit values.
        Quaternion(const Float3& xyz, float w);

        /// \brief Sum a quaternion to this one.
        Quaternion& operator+=(const Quaternion& rhs);

        /// \brief Subtract a quaternion from this one.
        Quaternion& operator-=(const Quaternion& rhs);

        /// \brief Memberwise multiplication of this quaternion by a scalar.
        Quaternion& operator*=(float rhs);

        /// \brief Multiply this quaternion by another one.
        Quaternion& operator*=(const Quaternion& rhs);

        /// \brief Memberwise division of this quaternion by a scalar.
        Quaternion& operator/=(float rhs);

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
    Quaternion operator*(const Quaternion& lhs, float rhs);

    /// \brief Divide a quaternion by a scalar.
    Quaternion operator/(const Quaternion& lhs, float rhs);

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

    /// \brief Get a normalized quaternion.
    Quaternion Normalize(const Quaternion& rhs);

    /// \brief Check whether a quaternion is a unit quaternion.
    bool IsNormalized(const Quaternion& rhs, float epsilon = 0.01f);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline Quaternion::Quaternion(float x, float y, float z, float w)
        : Quaternion({ x, y, z }, w)
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

    inline Quaternion& Quaternion::operator-=(const Quaternion& rhs)
    {
        xyz_ -= rhs.xyz_;
        w_ -= rhs.w_;

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

    inline Quaternion& Quaternion::operator/=(float rhs)
    {
        xyz_ /= rhs;
        w_ /= rhs;

        return *this;
    }

    inline Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
    {
        return Quaternion(lhs) += rhs;
    }

    inline Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)
    {
        return Quaternion(lhs) -= rhs;
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

    inline Quaternion operator/(const Quaternion& lhs, float rhs)
    {
        return Quaternion(lhs) /= rhs;
    }

    inline Quaternion Conjugate(const Quaternion& rhs)
    {
        return { -rhs.xyz_, rhs.w_ };
    }

    inline Quaternion Inverse(const Quaternion& rhs)
    {
        return Conjugate(rhs) / SqrNorm(rhs);
    }

    inline float Norm(const Quaternion& rhs)
    {
        return FastSqrt(SqrNorm(rhs));
    }

   inline float SqrNorm(const Quaternion& rhs)
    {
        return Dot(rhs.xyz_, rhs.xyz_) + rhs.w_ * rhs.w_;
    }

   inline Quaternion Normalize(const Quaternion& rhs)
   {
       return rhs / Norm(rhs);
   }

   inline bool IsNormalized(const Quaternion& rhs, float epsilon)
   {
       return FastAbs(SqrNorm(rhs)) < epsilon;
   }

}
