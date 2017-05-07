
/// \file class_interfaces.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "type_traits.h"

#include "reflection/class.h"
#include "reflection/any.h"

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
            Any operator()(TArguments&&... arguments) const;

        private:

            /// \brief Instantiate a new object.
            /// \param arguments Arguments to pass to the constructor.
            template <typename TClass>
            static Any Instantiate(TConstructorArguments... arguments);

			Any(*instancer_)(TConstructorArguments...);						/// \brief Functor used to instantiate the class.

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
		Any Constructible<TConstructorArguments...>::operator()(TArguments&&... arguments) const
        {
            return (*instancer_)(std::forward<TArguments>(arguments)...);
        }

         template <typename... TConstructorArguments>
         template <typename TClass>
		 Any Constructible<TConstructorArguments...>::Instantiate(TConstructorArguments... arguments)
         {
			 // TODO: if the caller doesn't take owneship of this object it will leak. Use std::unique_ptr<T> instead...
             return new TClass(std::forward<TConstructorArguments>(arguments)...);
         }

    }
}
