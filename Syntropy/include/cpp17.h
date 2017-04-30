
/// \file cpp17.h
/// \brief Contains temporary utilities that are defined as C++17 standard but not yet fully supported by some compilers.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

namespace std
{

    /*
    * From: http://en.cppreference.com/w/cpp/utility/in_place
    * std::in_place, std::in_place_type, and std::in_place_index are disambiguation tags that can be passed to the constructors of 
    * std::optional, std::variant, and std::any to indicate that the contained object should be constructed in-place, and (for the latter two) the type of the object to be constructed.
    * The corresponding type/type templates std::in_place_t, std::in_place_type_t and std::in_place_index_t can be used in the constructor's parameter list to match the intended tag.
    */

    template <class T> struct in_place_type_t
    {
        explicit in_place_type_t() = default;
    };

    /// \brief If T is a specialization of std::in_place_type_t provides a member constant value equal to true, otherwise value is false.
    template <typename T>
    struct is_in_place_type : std::false_type {};

    /// \brief Template specialization for is_in_place_type when T is a specialization of in_place_type_t.
    template <typename T>
    struct is_in_place_type<in_place_type_t<T>> : std::true_type {};

}
