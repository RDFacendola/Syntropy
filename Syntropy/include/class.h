
/// \file class.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <unordered_map>
#include <memory>

#include "hashed_string.h"
#include "property.h"
#include "method.h"

namespace syntropy {

    namespace reflection {

		class IClassDefinition;

        template <typename TClass>
        class ClassDefinition;

        template <typename TClass>
        struct ClassDeclaration;

        // Forward declarations

        class ClassFactory;

    }

}

namespace syntropy {

    namespace reflection {

        /// \brief Describes a class type.
        /// \author Raffaele D. Facendola - 2016
        class Class {

        public:

            /// \brief No copy constructor.
            Class(const Class&) = delete;

            /// \brief No assignment operator.
            Class& operator=(const Class&) = delete;

            /// \brief Virtual destructor.
            virtual ~Class() = default;

            template <typename TClass>
            static Class& GetClass();

            /// \brief Get the name of the class.
            /// \return Returns the type string of the class.
            const HashedString& GetName() const noexcept;

            /// \brief Get the list of classes that are derived by this class.
            /// \return Returns the list of classes that are derived by this class.
            const std::vector<Class*>& GetBaseClasses() const noexcept;

            /// \brief Get a factory for this class.
            /// \return Returns an factory for this class if applicable. Returns nullptr otherwise.
            const ClassFactory* GetFactory() const noexcept;

            /// \brief Get a class property by name.
            /// \param property_name Name of the property to get.
            /// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
            const Property* GetProperty(const HashedString& property_name) const noexcept;

            /// \brief Get a class method by name.
            /// \param method_name Name of the method to get.
            /// \return Returns a pointer tot he requested method if any. Returns nullptr otherwise.
            const Method* GetMethod(const HashedString& method_name) const noexcept;

            /// \brief Get the class properties list.
            /// \return Returns the class properties list.
            const std::unordered_map<size_t, Property>& GetProperties() const noexcept;

            /// \brief Check whether this class is a base class for the specified one.
            /// \return Returns true if the class described by this instance is a base for the specified one or is the same type, returns false otherwise.
            bool IsBaseOf(const Class& other) const noexcept;

        private:

            /// \brief Create a new class via explicit class definition.
            Class(std::unique_ptr<IClassDefinition> definition);
			
            size_t class_id_;                                       ///< \brief Unique id. 
                                                                    ///< \remarks Can be different at each execution.
            
			std::unique_ptr<IClassDefinition> definition_;			///< \brief Class definition.


        };

		/// \brief Interface for class definition.
		/// \author Raffaele D. Facendola - 2016
		class IClassDefinition {

		public:

			/// \brief Virtual destructor.
			virtual ~IClassDefinition() = default;

			/// \brief Get the name of the class.
			/// \return Returns the type string of the class.
			virtual const HashedString& GetName() const noexcept = 0;

			/// \brief Get the list of classes that are derived by this class.
			/// \return Returns the list of classes that are derived by this class.
			virtual const std::vector<Class*>& GetBaseClasses() const noexcept = 0;

			/// \brief Get a factory for this class.
			/// \return Returns an factory for this class if applicable. Returns nullptr otherwise.
			virtual const ClassFactory* GetFactory() const noexcept = 0;

			/// \brief Get a class property by name.
			/// \param property_name Name of the property to get.
			/// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
			virtual const Property* GetProperty(const HashedString& property_name) const noexcept = 0;

			/// \brief Get a class method by name.
			/// \param method_name Name of the method to get.
			/// \return Returns a pointer tot he requested method if any. Returns nullptr otherwise.
			virtual const Method* GetMethod(const HashedString& method_name) const noexcept = 0;

			/// \brief Get the class properties list.
			/// \return Returns the class properties list.
			virtual const std::unordered_map<size_t, Property>& GetProperties() const noexcept = 0;

		};

        /// \brief Concrete class definition.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass>
        class ClassDefinition : public IClassDefinition {

        public:

            /// \brief Copy constructor
            ClassDefinition(const ClassDefinition<TClass>& other) noexcept;

            /// \brief Move constructor.
            ClassDefinition(ClassDefinition<TClass>&& other) noexcept;

            /// \brief Create a named class definition.
            /// \param name name of the class.
            ClassDefinition(const HashedString& name) noexcept;

			/// \brief Default destructor.
			~ClassDefinition() = default;
            
			virtual const HashedString& GetName() const noexcept override;

			virtual const std::vector<Class*>& GetBaseClasses() const noexcept override;

			virtual const ClassFactory* GetFactory() const noexcept override;

			virtual const Property* GetProperty(const HashedString& property_name) const noexcept override;

			virtual const Method* GetMethod(const HashedString& method_name) const noexcept override;

			virtual const std::unordered_map<size_t, Property>& GetProperties() const noexcept override;

            template <typename TBaseClass>
            void DefineBaseClass() noexcept;

            /// Properties and getter of the form
            /// TProperty TClass::*
            /// TProperty (TClass::*)() const
            template <typename TProperty>
            void DefineProperty(const HashedString& name, TProperty&& property) noexcept;

            /// Getters and setters of the form
            /// TProperty (TClass::*)() const / void (TClass::*)(TProperty)
            /// const TProperty& (TClass::*)() const / TProperty& (TClass::*)()
            template <typename TGetter, typename TSetter>
            void DefineProperty(const HashedString& name, TGetter&& getter, TSetter&& setter) noexcept;

            //template <typename TMethod>
            //void DefineMethod(const HashedString& method_name, TMethod&& method) noexcept;

        private:

            HashedString name_;                                     ///< \brief Unique name.

            std::vector<Class*> base_classes_;                      ///< \brief List of all classes that are base of this one.

            std::unordered_map<size_t, Property> properties_;       ///< \brief List of all the supported properties.

            std::unordered_map<size_t, Method> methods_;            ///< \brief List of all the supported methods.

        };

        /// \brief Functor used to fill a class definition.
        /// Specialize this functor for each class requiring reflection.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass>
        struct ClassDeclaration {

            /// \brief Fill the provided class definition.
            /// \param definition Definition to fill.
            std::unique_ptr<ClassDefinition<TClass>> operator()() const {

                return std::make_unique<ClassDefinition<TClass>>(typeid(TClass).name());

            }

        };

    }

}

namespace syntropy {

    namespace reflection {

        // Implementation

        //////////////// CLASS ////////////////

        template <typename TClass>
        inline static Class& Class::GetClass() {

            static Class instance(ClassDeclaration<TClass>{}());

            return instance;

        }

        inline const HashedString& Class::GetName() const noexcept {

            return definition_->GetName();

        }
    
        inline const std::vector<Class*>& Class::GetBaseClasses() const noexcept {

            return definition_->GetBaseClasses();

        }

        inline const ClassFactory* Class::GetFactory() const noexcept {

            return definition_->GetFactory();

        }

        inline const Property* Class::GetProperty(const HashedString& property_name) const noexcept {

			return definition_->GetProperty(property_name);
                
        }

        inline const Method* Class::GetMethod(const HashedString& method_name) const noexcept {

			return definition_->GetMethod(method_name);

        }

        inline const std::unordered_map<size_t, Property>& Class::GetProperties() const noexcept {

            return definition_->GetProperties();

        }

        //////////////// CLASS DEFINITION ////////////////
            
		template <typename TClass>
		inline ClassDefinition<TClass>::ClassDefinition(const ClassDefinition<TClass>& other) noexcept
			: name_(other.name_)
			, base_classes_(other.base_classes_)
			, properties_(other.properties_)
			, methods_(other.methods_) {}
		
		template <typename TClass>
        inline ClassDefinition<TClass>::ClassDefinition(ClassDefinition<TClass>&& other) noexcept
            : name_(std::move(other.name_))
            , base_classes_(std::move(other.base_classes_))
            , properties_(std::move(other.properties_))
            , methods_(std::move(other.methods_)){}

        template <typename TClass>
        inline ClassDefinition<TClass>::ClassDefinition(const HashedString& name) noexcept
            : name_(name) {}

		template <typename TClass>
		inline const HashedString& ClassDefinition<TClass>::GetName() const noexcept {

            return name_;

        }
    
		template <typename TClass>
        inline const std::vector<Class*>& ClassDefinition<TClass>::GetBaseClasses() const noexcept {

            return base_classes_;

        }

		template <typename TClass>
        inline const ClassFactory* ClassDefinition<TClass>::GetFactory() const noexcept {

            // TODO: implement me!
            return nullptr;

        }

		template <typename TClass>
        inline const Property* ClassDefinition<TClass>::GetProperty(const HashedString& property_name) const noexcept {

            auto it = properties_.find(std::hash<HashedString>()(property_name));

            return it != properties_.end() ?
                   std::addressof(it->second) :
                   nullptr;
                
        }

		template <typename TClass>
        inline const Method* ClassDefinition<TClass>::GetMethod(const HashedString& method_name) const noexcept {

            auto it = methods_.find(std::hash<HashedString>()(method_name));

            return it != methods_.end() ?
                   std::addressof(it->second) :
                   nullptr;
        
        }

		template <typename TClass>
        inline const std::unordered_map<size_t, Property>& ClassDefinition<TClass>::GetProperties() const noexcept {

            return properties_;

        }

        template <typename TClass>
        template <typename TBaseClass>
        inline void ClassDefinition<TClass>::DefineBaseClass() noexcept {

            static_assert(std::is_base_of_v<TBaseClass, TClass>, "The class being defined does not derive from TBaseClass");
            static_assert(!std::is_same<TBaseClass, TClass>::value, "A class cannot derive from itself");

            base_classes_.push_back(std::addressof(Class::GetClass<TBaseClass>()));

        }

        template <typename TClass>
        template <typename TProperty>
        void ClassDefinition<TClass>::DefineProperty(const HashedString& name, TProperty&& property) noexcept {

            properties_.emplace(std::make_pair(std::hash<HashedString>()(name),
                                               Property(name, 
                                                        std::forward<TProperty>(property))));

        }

        template <typename TClass>
        template <typename TGetter, typename TSetter>
        void ClassDefinition<TClass>::DefineProperty(const HashedString& name, TGetter&& getter, TSetter&& setter) noexcept {

            properties_.emplace(std::make_pair(std::hash<HashedString>()(name),
                                               Property(name, 
                                                        std::forward<TGetter>(getter),
                                                        std::forward<TSetter>(setter))));

        }

    }

}