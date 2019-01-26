
/// \file rotation.h
/// \brief This header is part of the syntropy math system. It contains classes used to handle rotations.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/diagnostics/assert.h"
#include "syntropy/math/math.h"
#include "syntropy/math/vector.h"
#include "syntropy/math/quaternion.h"
#include "syntropy/patterns/utility.h"

namespace syntropy
{

    /************************************************************************/
    /* ROTATION                                                             */
    /************************************************************************/

    /// \brief Represents a rotation in 3D space.
    /// \author Raffaele D. Facendola - January 2019
    struct Rotation
    {
        /// \brief Object that represents no rotation.
        static const Rotation kIdentity;

        /// \brief Create an identity rotation.
        Rotation() = default;

        /// \brief Create a rotation from an axis and an angle.
        Rotation(const Float3& axis, float angle);

        /// \brief Create a rotation from a quaternion.
        Rotation(const Quaternion& rhs);

        /// \brief Implicit conversion to a unit quaternion.
        operator const Quaternion&() const;

        /// \brief Combine two rotations.
        Rotation& operator*=(const Rotation& rhs);

        /// \brief Get the inverse of this rotation.
        Rotation operator-() const;

        /// \brief Rotate a 3-vector using this quaternion.
        template <typename T>
        Vector3<T> operator*(const Vector3<T>& rhs) const;

        /// \brief Check whether this rotation is an identity rotation.
        bool IsIdentity(float epsilon = 0.01f) const;

        /// \brief Get the angle of rotation.
        float GetAngle() const;

        /// \brief Get the axis of rotation.
        /// The behavior of this method is undefined if this rotation is an identity rotation.
        Float3 GetAxis() const;

    private:

        /// \brief Create an uninitialized rotation.
        Rotation(uninitialized_t);

        /// \brief Unit quaternion representing this rotation.
        Quaternion quaternion_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Combine two rotations.
    Rotation operator*(const Rotation& lhs, const Rotation& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline const Rotation Rotation::kIdentity = Quaternion();

    inline Rotation::Rotation(const Float3& axis, float angle)
        : quaternion_(std::sin(angle * 0.5f) * Normalize(axis), std::cos(angle * 0.5f))
    {

    }

    inline Rotation::Rotation(const Quaternion& rhs)
        : quaternion_(Normalize(rhs))
    {

    }

    inline Rotation::operator const Quaternion&() const
    {
        return quaternion_;
    }

    inline Rotation& Rotation::operator*=(const Rotation& rhs)
    {
        quaternion_ *= rhs.quaternion_;

        return *this;
    }

    inline Rotation Rotation::operator-() const
    {
        Rotation inverse(uninitialized);

        inverse.quaternion_ = Conjugate(quaternion_);

        return inverse;
    }

    template <typename T>
    inline Vector3<T> Rotation::operator*(const Vector3<T>& rhs) const
    {
        // Euler-Rodriguez formula

        auto t = 2.0f * Cross(quaternion_.xyz_, rhs);

        return rhs + (quaternion_.w_ * t) + Cross(quaternion_.xyz_, t);
    }

    inline float Rotation::GetAngle() const
    {
        return 2.0f * std::acosf(quaternion_.w_);
    }

    inline Float3 Rotation::GetAxis() const
    {
        return quaternion_.xyz_ * FastInvSqrt(1.0f - quaternion_.w_ * quaternion_.w_);      // sin(theta) = sqrt(1 - cos(theta)^2)
    }

    bool Rotation::IsIdentity(float epsilon) const
    {
        return FastAbs(quaternion_.w_) > (1.0f - epsilon);
    }

    inline Rotation::Rotation(uninitialized_t)
    {

    }

    inline Rotation operator*(const Rotation& lhs, const Rotation& rhs)
    {
        return Rotation(lhs) *= rhs;
    }

}
