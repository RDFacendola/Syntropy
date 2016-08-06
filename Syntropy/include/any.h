/// \file any.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <typeinfo>

namespace syntropy {

	/// \brief Describes a type-safe container for single values of any type.
	/// \author Raffaele D. Facendola, based on "Valued Conversion" by Kevlin Henney
	class Any {

	public:

        /// \brief Create an empty instance.
		Any();

        /// \brief Destructor.
		~Any();

        /// \brief Copy constructor.
        /// \param other Instance to copy.
		Any(const Any& other);

        /// \brief Move constructor
        /// \param other Instance to move.
		Any(Any&& other);

        /// \brief Create an instance containing a specific object.
        /// \param value Value to store inside the instance.
		template <typename TValue>
		Any(const TValue& value);
		
        /// \brief Unified assignment operator.
        /// \param other Instance to assign from.
		Any& operator=(Any other);

        /// \brief Assign a new value to this instance.
        /// \param other Value to store inside the instance.
		template <typename TValue>
		Any& operator=(const TValue& other);

        /// \brief Get the underlying type of the contained value.
        /// \return Returns the type of the contained value.
		const std::type_info& GetType() const;

        /// \brief Get a typed pointer to the contained value.
        /// \return Returns a pointer to the contained value if the underlying type is exactly the one specified by the template parameters. Returns nullptr instead.
		template <typename TValue>
		const TValue* As() const;

        /// \brief Get a typed pointer to the contained value.
        /// \return Returns a pointer to the contained value if the underlying type is exactly the one specified by the template parameters. Returns nullptr instead.
        template <typename TValue>
        TValue* As();

        /// \brief Swaps two instances.
        /// \param other Object to swap with the current instance.
		Any& swap(Any& other) noexcept;

	private:

        /// \brief Base interface for the underlying content.
		struct IContent {

		public:

            /// \brief Virtual destructor.
			virtual ~IContent();

            /// \brief Get the type of the underlying contained value.
            /// \return Returns the type of the contained value.
			virtual const std::type_info& GetType() const = 0;

            /// \brief Clone the underlying value to another instance.
            /// \return Returns a pointer to the new copy of the value.
			virtual std::unique_ptr<IContent> Clone() const = 0;

		};

        /// \brief Strongly typed container for a single value.
        /// \tparam TValue Type of the contained value.
		template <typename TValue>
		struct Content : public IContent {

		public:

            /// \brief Create a new container for a single value.
            ///\ param value Value to store inside the container.
            Content(const TValue& value);

			virtual const std::type_info& GetType() const override;

			virtual std::unique_ptr<IContent> Clone() const override;

			TValue content_;					    ///< \brief Actual value.

		};

		std::unique_ptr<IContent> content_;         ///< \brief Wraps the actual value.

	};

	//////////////// ANY ////////////////

	inline Any::Any() {}

	inline Any::~Any() {}

	inline Any::Any(const Any& other) :
		content_(other.content_ ? other.content_->Clone() : nullptr) {}
	
	inline Any::Any(Any&& other) :
		content_(std::move(other.content_)) {}

	template <typename TValue>
	inline Any::Any(const TValue& value) :
		content_(std::make_unique<Content<TValue>>(value)) {}

	inline Any& Any::operator=(Any other) {

		return Any(other).swap(*this);
		
	}

	template <typename TValue>
	inline Any& Any::operator=(const TValue& other) {

		return Any(other).swap(*this);

	}
	
	inline const std::type_info& Any::GetType() const {

		return content_ ?
			   content_->GetType() :
			   typeid(void);

	}

	template <typename TValue>
	inline const TValue* Any::As() const {

		return (content_ && GetType() == typeid(TValue)) ?
			   &(static_cast<Content<TValue>*>(content_.get())->content_) :
			   nullptr;

	}

    template <typename TValue>
    inline TValue* Any::As() {

        return (content_ && GetType() == typeid(TValue)) ?
               &(static_cast<Content<TValue>*>(content_.get())->content_) :
               nullptr;

    }

	inline Any& Any::swap(Any& other) noexcept {

		std::swap(content_, other.content_);

		return *this;

	}

	//////////////// ANY :: ICONTENT ////////////////

	inline Any::IContent::~IContent() {}

	//////////////// ANY :: CONTENT ////////////////
	
	template <typename TValue>
	inline Any::Content<TValue>::Content(const TValue& value) :
		content_(value) {}
	
	template <typename TValue>
	inline const std::type_info& Any::Content<TValue>::GetType() const {

		return typeid(TValue);

	}

	template <typename TValue>
	inline std::unique_ptr<Any::IContent> Any::Content<TValue>::Clone() const {

		return std::make_unique<Content<TValue>>(content_);

	}
	
}
