
/// \file tags.h
/// \brief This header is part of Syntropy language module. It contains definitions for tag dispatching and tag types.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

namespace Syntropy::Tags
{
    /************************************************************************/
    /* TYPE                                                                 */
    /************************************************************************/

    /// \brief Generic tag type used to type used to propagate a type when explicit type is not supported (e.g. empty template constructors).
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TType>
    struct Type
    {
        /// \brief Default constructor.
        explicit Type() = default;
    };

    /// \brief Constant of the tag type Type used to propagate types.
    template <typename TType>
    inline constexpr Type<TType> kType = Type<TType>{};

    /************************************************************************/
    /* DEFAULT CONSTRUCT                                                    */
    /************************************************************************/

    /// \brief Tag type used to disambiguate between functions that forward tuples to construct objects.
    /// \author Raffaele D. Facendola - April 2020.
    struct DefaultConstruct
    {
        /// \brief Default constructor.
        explicit DefaultConstruct() = default;
    };

    /// \brief Constant of the tag type DefaultConstruct used to create objects via default-constructor.
    inline constexpr DefaultConstruct kDefaultConstruct = DefaultConstruct{};

}