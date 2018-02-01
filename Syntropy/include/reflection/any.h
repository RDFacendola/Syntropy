/// \file instance.h
/// \brief This header is part of the syntropy reflection system. It classes used to wrap values of any type.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <initializer_list>
#include <typeinfo>

#include "type_traits.h"

#include "diagnostics/assert.h"

#include "reflection/type.h"

namespace syntropy::reflection
{
    /// The class any describes a type-safe container for single values of any type.
    /// This class is similar to std::any, but takes advantage of syntropy reflection system to allow polymorphic conversions of values.
    /// \author Raffaele D. Facendola - April 2017
    class Any
    {
    public:

        /// \brief Constructs an empty object.
        constexpr Any() noexcept
            : holder_(nullptr)
        {

        }

        /// \brief Copy constructor.
        /// Copies the content of other into a new instance, so that any content is equivalent in both type and value to those of other prior to the constructor call, or empty if other is empty.
        /// \param other Instance to copy.
        Any(const Any& other);

        /// \brief Move constructor.
        /// Moves the content of other into a new instance, so that any content is equivalent in both type and value to those of other prior to the constructor call, or empty if other is empty.
        /// \param other Instance to move.
        Any(Any&& other) noexcept;

        /// \brief Create a non-empty object from a value.
        /// Constructs an object with initial content an object of type std::decay_t<TValue>, direct-initialized from std::forward<TValue>(value). 
        /// If std::is_copy_constructible<std::decay_t<TValue>>::value is false, the program is ill-formed. 
        /// This overload only participates in overload resolution if std::decay_t<TValue> is not the same type as any nor a specialization of std::in_place_type_t.
        template <typename TValue, typename = std::enable_if_t<!std::is_same<std::decay_t<TValue>, Any>::value && !is_in_place_type<std::decay_t<TValue>>::value>>
        Any(TValue&& value)
            : holder_(new HolderT<std::decay_t<TValue>>(std::forward<TValue>(value)))
        {
            static_assert(std::is_copy_constructible<std::decay_t<TValue>>::value, "std::decay<TValue> must be copy-constructible.");
        }

        /// \brief Create a non-empty object in place.
        /// Constructs an object with initial content an object of type std::decay_t<TValue>, direct-non-list-initialized from std::forward<TArguments>(arguments).... 
        template <typename TValue, typename... TArguments>
        explicit Any(std::in_place_type_t<TValue>, TArguments&&... arguments)
            : holder_(new HolderT<std::decay_t<TValue>>(std::in_place_type_t<TValue>{}, std::forward<TArguments>(arguments)...))
        {
            static_assert(std::is_constructible<std::decay_t<TValue>, TArguments...>::value, "std::decay<TValue> must be constructible from TArguments... .");
            static_assert(std::is_copy_constructible<std::decay_t<TValue>>::value, "std::decay<TValue> must be copy-constructible.");
        }

        /// \brief Create a non-empty object in place.
        /// Constructs an object with initial content an object of type std::decay_t<TValue>, direct-non-list-initialized from initializer_list, std::forward<TArguments>(arguments)... .
        template< typename TValue, typename TInitializerList, typename... TArguments >
        explicit Any(std::in_place_type_t<TValue>, std::initializer_list<TInitializerList> initializer_list, TArguments&&... arguments)
            : holder_(new HolderT<std::decay_t<TValue>>(std::in_place_type_t<TValue>{}, initializer_list, std::forward<TArguments>(arguments)...))
        {
            static_assert(std::is_constructible<std::decay_t<TValue>, std::initializer_list<TInitializerList>&, TArguments...>::value, "std::decay<TValue> must be constructible from std::initializer_list<TInitializerList>& and TArguments... .");
            static_assert(std::is_copy_constructible<std::decay_t<TValue>>::value, "std::decay<TValue> must be copy-constructible.");
        }

        /// \brief Copy-assignment operator.
        /// \param other Other instance to copy.
        Any& operator=(const Any& other);

        /// \brief Move-assignment operator.
        /// \param other Other instance to move.
        Any& operator=(Any&& other) noexcept;

        /// \brief Assign a new value to the object.
        /// \param other Object to assign.
        template<typename TValue, typename = std::enable_if_t<!std::is_same<std::decay_t<TValue>, Any>::value>>
        Any& operator=(TValue&& other)
        {
            static_assert(std::is_copy_constructible<std::decay_t<TValue>>::value, "std::decay<TValue> must be copy-constructible.");

            Any(std::forward<TValue>(other)).Swap(*this);

            return *this;
        }

        /// \brief Destructor.
        ~Any();

        /// \brief Constructs an object of type std::decay_t<TValue>, direct-non-list-initialized from std::forward<TArguments>(arguments)... . 
        template<typename TValue, typename... TArguments>
        std::decay_t<TValue>& Emplace(TArguments&&... arguments)
        {
            Reset();

            Any(std::in_place_type_t<TValue>{}, std::forward<TArguments>(arguments)...).Swap(*this);

            SYNTROPY_ASSERT(holder_);
            SYNTROPY_ASSERT(holder_->GetTypeInfo() == typeid(std::decay_t<TValue>));

            return static_cast<Any::HolderT<std::decay_t<TValue>>*>(holder_)->value_;
        }

        /// \brief Constructs an object of type std::decay_t<TValue>, direct-non-list-initialized from initializer_list, std::forward<TArguments>(arguments)... .
        template< typename TValue, typename TInitializerList, typename... TArguments >
        std::decay_t<TValue>& Emplace(std::initializer_list<TInitializerList> initializer_list, TArguments&&... arguments)
        {
            Reset();

            Any(std::in_place_type_t<TValue>{}, initializer_list, std::forward<TArguments>(arguments)...).Swap(*this);

            SYNTROPY_ASSERT(holder_);
            SYNTROPY_ASSERT(holder_->GetTypeInfo() == typeid(std::decay_t<TValue>));

            return static_cast<Any::HolderT<std::decay_t<TValue>>*>(holder_)->value_;
        }

        /// \brief Destroys the contained object.
        void Reset() noexcept;

        /// \brief Swaps to object instances.
        /// \param other Other instance to swap.
        void Swap(Any& other) noexcept;

        /// \brief Check whether the object holds a value.
        /// \return Returns true if the object holds a value, returns false otherwise.
        bool HasValue() const noexcept;

        /// \brief Get the type of the held object, if any.
        /// \return Returns the type of the contained value if non-empty. Returns the type of void, otherwise.
        const Type& GetType() const noexcept;

        /// \brief Get the typeid of the held object, if any.
        /// \return Returns the type of the contained value if non-empty. Returns the type of void, otherwise.
        const type_info& GetTypeInfo() const noexcept;

    private:

        template<class TValue>
        friend TValue AnyCast(const Any& operand);

        template<class TValue>
        friend TValue AnyCast(Any& operand);

        template<class TValue>
        friend TValue AnyCast(Any&& operand);

        template<class TValue>
        friend const TValue* AnyCast(const Any* operand) noexcept;

        template<class TValue>
        friend TValue* AnyCast(Any* operand) noexcept;

        struct Holder
        {

            virtual ~Holder() = default;

            virtual const Type& GetType() const noexcept = 0;

            virtual const type_info& GetTypeInfo() const noexcept = 0;

            virtual Holder* Clone() const noexcept = 0;

        };

        template <typename TContent>
        struct HolderT : Holder
        {
            template <typename TValue>
            HolderT(TValue&& value)
                : value_(std::forward<TValue>(value))
            {

            }

            template <typename TValue, typename... TArguments>
            HolderT(std::in_place_type_t<TValue>, TArguments&&... arguments)
                : value_(std::forward<TValue>(arguments)...)
            {

            }

            template< typename TValue, typename TInitializerList, typename... TArguments>
            HolderT(std::in_place_type_t<TValue>, std::initializer_list<TInitializerList> initializer_list, TArguments&&... arguments)
                : value_(std::move(initializer_list), std::forward<TValue>(arguments)...)
            {

            }

            const Type& Any::HolderT<TContent>::GetType() const noexcept
            {
                return TypeOf<TContent>();
            }

            const type_info& Any::HolderT<TContent>::GetTypeInfo() const noexcept
            {
                return typeid(TContent);
            }

            Any::Holder* Any::HolderT<TContent>::Clone() const noexcept
            {
                return new HolderT<TContent>(value_);
            }

            TContent value_;

        };

        Holder* holder_;        ///< \brief Holds the contained value. May be nullptr.

    };

    // Any cast

    template<class TValue>
    TValue AnyCast(const Any& operand)
    {
        using U = std::remove_cv_t<std::remove_reference_t<TValue>>;

        static_assert(std::is_constructible<TValue, const U&>::value, "TValue must be constructible by const reference.");

        return static_cast<TValue>(*AnyCast<U>(&operand));
    }

    template<class TValue>
    TValue AnyCast(Any& operand)
    {
        using U = std::remove_cv_t<std::remove_reference_t<TValue>>;

        static_assert(std::is_constructible<TValue, U&>::value, "TValue must be constructible by reference.");

        return static_cast<TValue>(*AnyCast<U>(&operand));
    }

    template<class TValue>
    TValue AnyCast(Any&& operand)
    {
        using U = std::remove_cv_t<std::remove_reference_t<TValue>>;

        static_assert(std::is_constructible<TValue, U>::value, "TValue must be constructible from value.");

        return static_cast<TValue>(std::move(*AnyCast<U>(&operand)));
    }

    template<class TValue>
    const TValue* AnyCast(const Any* operand) noexcept
    {
        static_assert(std::is_copy_constructible<std::decay_t<TValue>>::value, "std::decay<TValue> must be copy-constructible.");

        return operand && operand->GetType().IsConvertibleTo(TypeOf<const TValue>()) ?
            std::addressof(static_cast<Any::HolderT<const TValue>*>(operand->holder_)->value_) :
            nullptr;
    }

    template<class TValue>
    TValue* AnyCast(Any* operand) noexcept
    {
        static_assert(std::is_copy_constructible<std::decay_t<TValue>>::value, "std::decay<TValue> must be copy-constructible.");

        return operand && operand->GetType().IsConvertibleTo(TypeOf<TValue>()) ?
            std::addressof(static_cast<Any::HolderT<TValue>*>(operand->holder_)->value_) :
            nullptr;
    }

    // Make any

    template<typename TValue, typename... TArguments>
    Any MakeAny(TArguments&&... arguments)
    {
        return Any(std::in_place_type_t<TValue>{}, std::forward<TArguments>(arguments)...);
    }

    template<typename TValue, typename TInitializerList, typename... TArguments>
    Any MakeAny(std::initializer_list<TInitializerList> initializer_list, TArguments&&... arguments)
    {
        return Any(std::in_place_type_t<TValue>{}, initializer_list, std::forward<TArguments>(arguments)...);
    }
}

namespace std
{
    /// \brief Overloads the std::swap algorithm for syntropy::reflection::Any. Swaps the content of two any objects by calling first.Swap(second)
    void swap(syntropy::reflection::Any& first, syntropy::reflection::Any& second) noexcept;
}