
/// \file meta_class.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <sstream>

#include "any.h"
#include "syntropy.h"
#include "hashed_string.h"
#include "type_traits.h"

namespace syntropy {

    // Forward declarations


    class MetaFactory;

    class MetaClass;
        
    class MetaClassProperty;

    class MetaClassMethod;


    
}

namespace syntropy {

    

	template <typename TProperty, typename = void>
    struct MetaClassPropertyParser {

        using TParser = std::function<bool(Any, std::stringstream&)>;

        TParser operator() () const {

            return[](Any, std::stringstream&) -> bool {

                return false;

            };

        }

		template <typename _>
		TParser operator() (_) const {

			return (*this)();

		}

    };

	template <typename TProperty>
	struct MetaClassPropertyParser<TProperty,
								   std::enable_if_t<is_stream_extractable<std::stringstream, 
																		  TProperty>::value>>{

		using TParser = std::function<bool(Any, std::stringstream&)>;

		template <typename TClass, typename TProperty>
		TParser operator() (TProperty TClass::* property) const {

			return[property](Any instance, std::stringstream& sstream) -> bool{

				auto instance_ptr = instance.As<std::add_pointer_t<TClass>>();

				if (instance_ptr) {

					sstream >> ((*instance_ptr)->*property);

				}

				return instance_ptr &&
					   !sstream.fail();

			};

		}

		template <typename TClass, typename TProperty>
		TParser operator() (void (TClass::* setter)(TProperty)) const {

			return[setter](Any instance, std::stringstream& sstream) -> bool {

				auto instance_ptr = instance.As<std::add_pointer_t<TClass>>();

				if (instance_ptr) {

					std::remove_cv_t<std::remove_reference_t<TProperty>> property_value;

					sstream >> property_value;

					if (!sstream.fail()) {

						((*instance_ptr)->*setter)(property_value);

					}

				}

				return instance_ptr &&
					   !sstream.fail();

			};

		}

		template <typename TClass, typename TProperty>
		TParser operator() (TProperty& (TClass::* setter)()) const {

			return[setter](Any instance, std::stringstream& sstream) -> bool {

				auto instance_ptr = instance.As<std::add_pointer_t<TClass>>();

				if (instance_ptr) {

					sstream >> ((*instance_ptr)->*setter)();

				}

				return instance_ptr &&
					   !sstream.fail();

			};

		}

	};

    template <typename TValue, typename = void>
    struct MetaClassPropertyInterpreter {

        template <typename TInstance>
        bool operator()(const MetaClassPropertyParser<void>::TParser&, TInstance&, const TValue&, std::ios_base::fmtflags) {

            // The provided type does not support inward interpretation

            return false;

        }

    };

    template <typename TValue>
    struct MetaClassPropertyInterpreter<TValue,
                                        std::enable_if_t<is_stream_insertable<std::stringstream, TValue>::value>>{

        template <typename TInstance>
        bool operator()(const MetaClassPropertyParser<void>::TParser& parser, TInstance& instance, const TValue& value, std::ios_base::fmtflags flags) {

            std::stringstream sstream;

            sstream.setf(flags);

            sstream << value;

            return parser(std::addressof(instance),
                          sstream);

        }

    };

}