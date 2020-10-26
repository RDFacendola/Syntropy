/// \file tuple.h
/// \brief This header is part of Syntropy core module. It contains definitions for tuples.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/templates.h"

#include "syntropy/experimental/core/foundation/details/tuple_details.h"

// ===========================================================================

namespace Syntropy::Experimental::Templates
{
    /************************************************************************/
    /* TUPLE ELEMENT                                                        */
    /************************************************************************/

    /// \brief Provides indexed access to tuple elements' types.
    template <Int VIndex, typename TTuple>
    using TupleElement = Details::TupleElement<VIndex, TTuple>;

    /************************************************************************/
    /* TUPLE POP FRONT                                                      */
    /************************************************************************/

    /// \brief Discards the first VCount elements in a tuple and provides a type alias equal to a tuple with the remaining elements.
    template <Int VCount, typename TTuple>
    using TuplePopFront = Details::TuplePopFront<VCount, TTuple>;

    /************************************************************************/
    /* TUPLE SIZE                                                           */
    /************************************************************************/

    /// \brief Constant equal to the size (rank) of a tuple.
    template <typename TTuple>
    inline constexpr Int TupleSize = Details::TupleSize<TTuple>;
}

// ===========================================================================

namespace Syntropy::Experimental
{
    /************************************************************************/
    /* TUPLE <ELEMENTS...>                                                  */
    /************************************************************************/

    // !IMPORTANT! In VS2019 apparently "explicit" keyword is not considered when 
    //             constructors are not declared and defined at the same time.

     /// \brief Represents a fixed-size collection of heterogeneous elements.
     /// \author Raffaele D. Facendola - September 2020.
    template <typename... TTypes>
    struct Tuple;

    /// \brief Recursive tuple definition.
    template <typename TType, typename... TTypes>
    struct Tuple<TType, TTypes...> : private Tuple<TTypes...>
    {
        template <Int VIndex, typename... TTypes>
        friend constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(Tuple<TTypes...>& tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(Tuple<TTypes...>&& tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(const Tuple<TTypes...>& tuple) noexcept;

        template <Int VIndex, typename... TTypes>
        friend constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(const Tuple<TTypes...>&& tuple) noexcept;

    public:

        /// \brief Type of the base class.
        using TBaseClass = Tuple<TTypes...>;

        /// \brief Types of tuple elements.
        using TTypeList = Syntropy::Templates::TypeList<TType, TTypes...>;

        /// \brief Tag type used to construct a tuple element-wise.
        struct ElementwiseConstructor {};

        /// \brief Tuple default constructor. Enabled if all elements are default-constructible.
        template<typename UType = TType, Details::EnableIfTupleDefaultConstructor<Syntropy::Templates::TypeList<UType, TTypes...>> = nullptr>
        explicit (Details::ExplicitIfTupleDefaultConstructor<UType, TTypes...>)
            constexpr Tuple() noexcept
            : TBaseClass{}
            , element_{}
        {

        }

        /// \brief Tuple direct constructor. Enabled if all elements are copy-constructible.
        template<typename UType = TType, Details::EnableIfTupleDirectConstructor<Syntropy::Templates::TypeList<UType, TTypes...>> = nullptr>
        explicit (Details::ExplicitIfTupleDirectConstructor<UType, TTypes...>)
            constexpr Tuple(const TType& element, const TTypes&... elements) noexcept
            : Tuple(ElementwiseConstructor{}, Forward<TType>(element), Forward<TTypes>(elements)...)
        {

        }

        /// \brief Tuple converting constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingConstructor<TTypeList, UType, UTypes...>)
            constexpr Tuple(UType&& element, UTypes&&... elements) noexcept
            : Tuple(ElementwiseConstructor{}, Forward<UType>(element), Forward<UTypes>(elements)...)
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingCopyConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingCopyConstructor<TTypeList, UType, UTypes...>)
            constexpr Tuple(const Tuple<UType, UTypes...>& rhs) noexcept
        {

        }

        /// \brief Tuple converting copy constructor. Enabled if all tuple elements are copy-constructible.
        template<typename UType, typename... UTypes, Details::EnableIfTupleConvertingMoveConstructor<TTypeList, UType, UTypes...> = nullptr>
        explicit (Details::ExplicitIfTupleConvertingMoveConstructor<TTypeList, UType, UTypes...>)
            constexpr Tuple(Tuple<UType, UTypes...>&& rhs) noexcept
        {

        }

        /// \brief Construct a tuple forwarding explicit arguments.
        template<typename UType, typename... UTypes>
        explicit (Details::ExplicitIfTupleConvertingConstructor<TTypeList, UType, UTypes...>)
            constexpr Tuple(ElementwiseConstructor, UType&& element, UTypes&&... elements) noexcept
            : TBaseClass(std::forward<UTypes>(elements)...)
            , element_(Forward<UType>(element))
        {

        }

        /// \brief Default copy-constructor.
        constexpr Tuple(const Tuple& other) = default;

        /// \brief Default move-constructor.
        constexpr Tuple(Tuple&& other) = default;

        /// \brief Head element.
        TType element_;
    };

    /// \brief Empty tuple.
    /// \author Raffaele D. Facendola - September 2020.
    template <>
    struct Tuple<>
    {
        /// \brief Default constructor.
        constexpr Tuple() noexcept = default;

        /// \brief Default copy constructor.
        constexpr Tuple(const Tuple&) noexcept = default;

        /// \brief Default copy-assignment.
        constexpr Tuple& operator=(const Tuple&) noexcept = default;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks VIndex must be in the range [0, sizeof(TTypes...)).
    template <Int VIndex, typename... TTypes>
    constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(Tuple<TTypes...>& tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks VIndex must be in the range [0, sizeof(TTypes...)).
    template <Int VIndex, typename... TTypes>
    constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(Tuple<TTypes...>&& tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks VIndex must be in the range [0, sizeof(TTypes...)).
    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(const Tuple<TTypes...>& tuple) noexcept;

    /// \brief Access the VIndex-th element in a tuple.
    /// \remarks VIndex must be in the range [0, sizeof(TTypes...)).
    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(const Tuple<TTypes...>&& tuple) noexcept;

}

// ===========================================================================

namespace Syntropy::Experimental
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <Int VIndex, typename... TTypes>
    constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(Tuple<TTypes...>& tuple) noexcept
    {
        using TTuple = Templates::TuplePopFront<VIndex, Tuple<TTypes...>>;

        return static_cast<TTuple&>(tuple).element_;
    }

    template <Int VIndex, typename... TTypes>
    constexpr Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(Tuple<TTypes...>&& tuple) noexcept
    {
        using TTuple = Templates::TuplePopFront<VIndex, Tuple<TTypes...>>;
        using TElement = Templates::TupleElement<VIndex, Tuple<TTypes...>>;

        return static_cast<TElement&&>(static_cast<TTuple&>(tuple).element_);
    }

    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>& Get(const Tuple<TTypes...>& tuple) noexcept
    {
        using TTuple = Templates::TuplePopFront<VIndex, Tuple<TTypes...>>;

        return static_cast<const TTuple&>(tuple).element_;
    }

    template <Int VIndex, typename... TTypes>
    constexpr const Templates::TupleElement<VIndex, Tuple<TTypes...>>&& Get(const Tuple<TTypes...>&& tuple) noexcept
    {
        using TTuple = Templates::TuplePopFront<VIndex, Tuple<TTypes...>>;
        using TElement = Templates::TupleElement<VIndex, Tuple<TTypes...>>;

        return static_cast<const TElement&&>(static_cast<const TTuple&>(tuple).element_);
    }

}
