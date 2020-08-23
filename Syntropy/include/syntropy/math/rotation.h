
/// \file rotation.h
/// \brief This header is part of the Syntropy math module. It contains definitions for rotations.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include "syntropy/language/fundamentals.h"
#include "syntropy/math/math.h"
#include "syntropy/math/vector.h"
#include "syntropy/math/quaternion.h"

namespace Syntropy
{
    /************************************************************************/
    /* MATH                                                                 */
    /************************************************************************/

    /// \brief Exposes rotation-related classes and operations.
    /// \author Raffaele D. Facendola - 2019.
    namespace Math
    {
        /************************************************************************/
        /* ROTATION                                                             */
        /************************************************************************/

        /// \brief Represents a rotation in 3D space.
        /// \author Raffaele D. Facendola - January 2019
        struct Rotation
        {
            /// \brief Create an identity rotation.
            Rotation() = default;

            /// \brief Create a rotation from an axis and an angle.
            Rotation(const Float3& axis, Float angle);

            /// \brief Create a rotation from a quaternion.
            Rotation(const Quaternion& rhs);

            /// \brief Implicit conversion to a unit quaternion.
            operator const Quaternion&() const;

            /// \brief Access the underlying unit quaternion.
            const Quaternion& ToQuaternion() const;

            /// \brief Combine two rotations.
            Rotation& operator*=(const Rotation& rhs);

            /// \brief Get the inverse of this rotation.
            Rotation operator-() const;

            /// \brief Check whether this rotation is an identity rotation.
            Bool IsIdentity(Float epsilon = 0.01f) const;

            /// \brief Get the angle of rotation.
            Float GetAngle() const;

            /// \brief Get the axis of rotation.
            /// The behavior of this method is undefined if this rotation is an identity rotation.
            Float3 GetAxis() const;

        private:

            /// \brief Unit quaternion representing this rotation.
            Quaternion quaternion_;

        };

        /************************************************************************/
        /* NON-MEMBER FUNCTIONS                                                 */
        /************************************************************************/

        /// \brief Combine two rotations.
        Rotation operator*(const Rotation& lhs, const Rotation& rhs);

        /// \brief Rotate a 3-vector.
        template <typename T>
        Vector3<T> operator*(const Vector3<T>& lhs, const Rotation& rhs);

    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Rotation.

    inline Math::Rotation::Rotation(const Float3& axis, Float angle)
        : quaternion_(std::sin(angle * 0.5f) * Normalize(axis), std::cos(angle * 0.5f))
    {

    }

    inline Math::Rotation::Rotation(const Quaternion& rhs)
        : quaternion_(Normalize(rhs))
    {

    }

    inline Math::Rotation::operator const Math::Quaternion&() const
    {
        return quaternion_;
    }

    inline const Math::Quaternion& Math::Rotation::ToQuaternion() const
    {
        return quaternion_;
    }

    inline Math::Rotation& Math::Rotation::operator*=(const Rotation& rhs)
    {
        quaternion_ *= rhs.quaternion_;

        return *this;
    }

    inline Math::Rotation Math::Rotation::operator-() const
    {
        auto inverse = Rotation{};

        inverse.quaternion_ = Conjugate(quaternion_);

        return inverse;
    }

    inline Float Math::Rotation::GetAngle() const
    {
        return 2.0f * std::acosf(quaternion_.w_);
    }

    inline Math::Float3 Math::Rotation::GetAxis() const
    {
        return quaternion_.xyz_ * FastInvSqrt(1.0f - quaternion_.w_ * quaternion_.w_);      // sin(theta) = sqrt(1 - cos(theta)^2)
    }

    inline Bool Math::Rotation::IsIdentity(Float epsilon) const
    {
        return Abs(quaternion_.w_) > (1.0f - epsilon);
    }

    inline Math::Rotation Math::operator*(const Rotation& lhs, const Rotation& rhs)
    {
        return Rotation(lhs) *= rhs;
    }

    template <typename T>
    inline Math::Vector3<T> Math::operator*(const Vector3<T>& lhs, const Rotation& rhs)
    {
        // Euler-Rodriguez formula

        auto& quaternion = rhs.ToQuaternion();

        auto t = 2.0f * Cross(quaternion.xyz_, lhs);

        return lhs + (quaternion.w_ * t) + Cross(quaternion.xyz_, t);
    }

}
