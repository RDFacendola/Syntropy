/// \file any.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <typeinfo>

namespace syntropy {

	/// \brief 
	/// \author Kevlin Henney - "Valued Conversion"
	class Any {

	public:

		Any();

		~Any();

		Any(const Any& other);

		Any(Any&& other);

		template <typename TAny>
		Any(const TAny& value);
		
		Any& operator=(Any other);

		template <typename TAny>
		Any& operator=(const TAny& other);

		const std::type_info& GetType() const;

		template <typename TValue>
		const TValue* As() const;

		Any& swap(Any& other) noexcept;

	private:

		struct IAny {

		public:

			virtual ~IAny();

			virtual const std::type_info& GetType() const = 0;

			virtual std::unique_ptr<IAny> Clone() const = 0;

		};

		template <typename TAny>
		struct AnyT : public IAny {

		public:

			AnyT(const TAny& value);

			virtual const std::type_info& GetType() const override;

			virtual std::unique_ptr<IAny> Clone() const override;

			const TAny content_;					///< \brief Actual content.

		};

		std::unique_ptr<IAny> content_;				///< \brief Wraps the actual value.

	};

	//////////////// ANY ////////////////

	inline Any::Any() {}

	inline Any::~Any() {}

	inline Any::Any(const Any& other) :
		content_(other.content_ ? other.content_->Clone() : nullptr) {}
	
	inline Any::Any(Any&& other) :
		content_(std::move(other.content_)) {}

	template <typename TAny>
	inline Any::Any(const TAny& value) :
		content_(std::make_unique<AnyT<TAny>>(value)) {}

	inline Any& Any::operator=(Any other) {

		return Any(other).swap(*this);
		
	}

	template <typename TAny>
	inline Any& Any::operator=(const TAny& other) {

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
			   &(static_cast<AnyT<TValue>*>(content_.get())->content_) :
			   nullptr;

	}

	inline Any& Any::swap(Any& other) noexcept {

		std::swap(content_, other.content_);

		return *this;

	}

	//////////////// ANY :: IANY////////////////

	inline Any::IAny::~IAny() {}

	//////////////// ANY :: ANY T ////////////////
	
	template <typename TAny>
	inline Any::AnyT<TAny>::AnyT(const TAny& value) :
		content_(value) {}
	
	template <typename TAny>
	inline const std::type_info& Any::AnyT<TAny>::GetType() const {

		return typeid(TAny);

	}

	template <typename TAny>
	inline std::unique_ptr<Any::IAny> Any::AnyT<TAny>::Clone() const {

		return std::make_unique<AnyT<TAny>>(content_);

	}
	
}
