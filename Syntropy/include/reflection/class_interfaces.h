
/// \file class_interfaces.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <functional>
#include <type_traits>

#include "type_traits.h"

#include "reflection/instance.h"
#include "reflection/class.h"

namespace syntropy
{
    namespace reflection
    {

        /// \brief Class interface used to instantiate new objects via explicit constructor.
        /// \tparam TConstructorArguments Type of the arguments passed to the class constructor.
        /// \author Raffaele D. Facendola - April 2017
        template <typename... TConstructorArguments>
        class Constructible
        {
        public:

            /// \brief Create a new interface.
            template <typename TClass>
            Constructible(tag_t<TClass>);

            /// \brief Instantiate a new object.
            /// \param arguments Arguments to pass to the constructor.
            template <typename... TArguments>
            Instance operator()(TArguments&&... arguments) const;

        private:

            /// \brief Instantiate a new object.
            /// \param arguments Arguments to pass to the constructor.
            template <typename TClass>
            static Instance Instantiate(TConstructorArguments... arguments);

            Instance(*instancer_)(TConstructorArguments...);                    /// \brief Functor used to instantiate the class.

        };

    }
}

// Implementation

namespace syntropy
{
    namespace reflection
    {

        /************************************************************************/
        /* CONSTRUCTIBLE                                                        */
        /************************************************************************/

        template <typename... TConstructorArguments>
        template <typename TClass>
        Constructible<TConstructorArguments...>::Constructible(tag_t<TClass>)
            : instancer_(&Instantiate<TClass>)
        {
            static_assert(std::is_constructible<TClass, TConstructorArguments...>::value, "TClass is not constructible with arguments TConstructorArguments.");
        }

        template <typename... TConstructorArguments>
        template <typename... TArguments>
        Instance Constructible<TConstructorArguments...>::operator()(TArguments&&... arguments) const
        {
            return (*instancer_)(std::forward<TArguments>(arguments)...);
        }

         template <typename... TConstructorArguments>
         template <typename TClass>
         Instance Constructible<TConstructorArguments...>::Instantiate(TConstructorArguments... arguments)
         {
             return *(new TClass(std::forward<TConstructorArguments>(arguments)...));
         }

    }
}
