
/// \file class_interfaces.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <functional>
#include <type_traits>

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
            Constructible(std::identity<TClass>);

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

        /// \brief Functor object used to assign the interface Constructible<> to classes.
        /// \author Raffaele D. Facendola - April 2017
        struct DefaultConstruct
        {

            /// \brief Add a DefaultConstructible interface to the provided class.
            /// \param class_definition Definition of the class the interface will be added to.
            template <typename TClass>
            void operator()(ClassDefinitionT<TClass>& class_definition) const;

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
        Constructible<TConstructorArguments...>::Constructible(std::identity<TClass>)
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

        /************************************************************************/
        /* DEFAULT CONSTRUCT                                                    */
        /************************************************************************/

        template <typename TClass>
        void DefaultConstruct::operator()(ClassDefinitionT<TClass>& class_definition) const
        {
            class_definition.AddInterface<Constructible<>>(std::identity<TClass>{});
        }

    }
}
