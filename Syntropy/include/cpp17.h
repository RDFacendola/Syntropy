
/// \file cpp17.h
/// \brief Contains temporary utilities that are defined as C++17 standard but not yet fully supported by some compilers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

namespace std
{
    /// \brief If T is a specialization of std::in_place_type_t provides a member constant value equal to true, otherwise value is false.
    template <typename T>
    struct is_in_place_type : std::false_type {};

    /// \brief Template specialization for is_in_place_type when T is a specialization of in_place_type_t.
    template <typename T>
    struct is_in_place_type<in_place_type_t<T>> : std::true_type {};
}
