
/// \file observer_ptr.h
/// \brief This header is part of syntropy types. It contains classes used to wrap raw pointers.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <cstddef>
#include <functional>

#include "syntropy/type_traits.h"

namespace syntropy
{
    /************************************************************************/
    /* OBSERVER PTR                                                         */
    /************************************************************************/

    /// \brief Represents a non-owing pointer to an object.
    /// Based on experimental std::experimental::observer_ptr.
    /// \author Raffaele D. Facendola = April 2020.
    template <typename TType>
    class ObserverPtr
    {
    public:

        /// \brief Create an empty observer pointer.
        constexpr ObserverPtr() noexcept = default;

        /// \brief Create an empty observer pointer.
        constexpr ObserverPtr(std::nullptr_t) noexcept;

        /// \brief Create an observer pointer pointing to an object.
        explicit ObserverPtr(TType* pointer) noexcept;

        /// \brief Create an observer pointer that watch another observer pointer pointee.
        template <typename UType, typename = std::enable_if_t<std::is_convertible_v<UType, TType>>>
        ObserverPtr(ObserverPtr<UType> other) noexcept;

        /// \brief Default copy constructor.
        ObserverPtr(const ObserverPtr& other) = default;

        /// \brief Default move constructor.
        ObserverPtr(ObserverPtr&& other) = default;
 
        /// \brief Default destructor.
        ~ObserverPtr() = default;

        /// \brief Default copy-assignment operator.
        ObserverPtr& operator=(const ObserverPtr& other) = default;

        /// \brief Default move-assignment operator.
        ObserverPtr& operator=(ObserverPtr&& other) = default;

        /// \brief Release the pointed object, returning it.
        constexpr TType* Release() noexcept;

        /// \brief Reset the observer pointer and update the observed object.
        constexpr void Reset(TType* rhs) noexcept;

        /// \brief Swaps pointer object with another observer ptr.
        constexpr void Swap(ObserverPtr& rhs) noexcept;

        /// \brief Access the underlying pointer.
        constexpr TType* Get() const noexcept;

        /// \brief Check whether the pointer points to something.
        constexpr explicit operator bool() const noexcept;

        /// \brief Access the object pointed by this observer pointer.
        constexpr std::add_lvalue_reference_t<TType> operator*() const;

        /// \brief Access the object pointed by this observer pointer.
        constexpr TType* operator->() const noexcept;

        /// \brief Convert the observer pointer to the stored raw pointer.
        constexpr explicit operator TType*() const noexcept;

    private:

        /// \brief Pointer to the object.
        TType* pointer_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create an observer pointer by deducting the type from the function argument.
    template <typename TType>
    ObserverPtr<TType> MakeObserver(TType* pointer) noexcept;

    /// \brief Equality comparison.
    template <typename TType, typename UType>
    bool operator==(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept;

    /// \brief Inequality comparison.
    template <typename TType, typename UType>
    bool operator!=(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept;

    /// \brief Equality comparison.
    template <typename TType>
    bool operator==(const ObserverPtr<TType>& lhs, std::nullptr_t) noexcept;

    /// \brief Equality comparison.
    template <typename TType>
    bool operator==(std::nullptr_t, const ObserverPtr<TType>& rhs) noexcept;

    /// \brief Inequality comparison.
    template <typename TType>
    bool operator!=(const ObserverPtr<TType>& lhs, std::nullptr_t) noexcept;

    /// \brief Inequality comparison.
    template <typename TType>
    bool operator!=(std::nullptr_t, const ObserverPtr<TType>& rhs) noexcept;

    /// \brief Check whether lhs is less than rhs.
    template <typename TType, typename UType>
    bool operator<(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept;

    /// \brief Check whether lhs is greater than rhs.
    template <typename TType, typename UType>
    bool operator>(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept;

    /// \brief Check whether lhs is less-than or equal-to rhs.
    template <typename TType, typename UType>
    bool operator<=(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept;

    /// \brief Check whether lhs is greater-than or equal-to rhs.
    template <typename TType, typename UType>
    bool operator>=(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept;

    /// \brief Swap the content of two observer pointers.
    template <typename TType>
    void Swap(ObserverPtr<TType>& lhs, ObserverPtr<TType>& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ObserverPtr<TType>.

    template <typename TType>
    constexpr ObserverPtr<TType>::ObserverPtr(std::nullptr_t) noexcept
        : pointer_(nullptr)
    {

    }

    template <typename TType>
    ObserverPtr<TType>::ObserverPtr(TType* pointer) noexcept
        : pointer_(pointer)
    {

    }

    template <typename TType>
    template <typename UType, typename>
    inline ObserverPtr<TType>::ObserverPtr(ObserverPtr<UType> other) noexcept
        : pointer_(other.pointer_)
    {

    }

    template <typename TType>
    constexpr TType* ObserverPtr<TType>::Release() noexcept
    {
        auto pointer = pointer_;

        pointer_ = nullptr;

        return pointer;
    }

    template <typename TType>
    constexpr void ObserverPtr<TType>::Reset(TType* rhs) noexcept
    {
        pointer_ = rhs;
    }

    template <typename TType>
    constexpr void ObserverPtr<TType>::Swap(ObserverPtr& rhs) noexcept
    {
        using std::swap;

        swap(pointer_, rhs.pointer_);
    }

    template <typename TType>
    constexpr TType* ObserverPtr<TType>::Get() const noexcept
    {
        return pointer_;
    }

    template <typename TType>
    constexpr ObserverPtr<TType>::operator bool() const noexcept
    {
        return (pointer_ != nullptr);
    }

    template <typename TType>
    constexpr std::add_lvalue_reference_t<TType> ObserverPtr<TType>::operator*() const
    {
        return *pointer_;
    }

    template <typename TType>
    constexpr TType* ObserverPtr<TType>::operator->() const noexcept
    {
        return pointer_;
    }

    template <typename TType>
    constexpr ObserverPtr<TType>::operator TType*() const noexcept
    {
        return pointer_;
    }

    // Non-member functions.

    template <typename TType>
    inline ObserverPtr<TType> MakeObserver(TType* pointer) noexcept
    {
        return ObserverPtr<TType>(pointer);
    }

    template <typename TType, typename UType>
    inline bool operator==(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept
    {
        return lhs.Get() == rhs.Get();
    }

    template <typename TType, typename UType>
    inline bool operator!=(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename TType>
    inline bool operator==(const ObserverPtr<TType>& lhs, std::nullptr_t) noexcept
    {
        return !lhs;
    }

    template <typename TType>
    inline bool operator==(std::nullptr_t, const ObserverPtr<TType>& rhs) noexcept
    {
        return !rhs;
    }

    template <typename TType>
    inline bool operator!=(const ObserverPtr<TType>& lhs, std::nullptr_t) noexcept
    {
        return static_cast<bool>(lhs);
    }

    template <typename TType>
    inline bool operator!=(std::nullptr_t, const ObserverPtr<TType>& rhs) noexcept
    {
        return static_cast<bool>(rhs);
    }

    template <typename TType, typename UType>
    inline bool operator<(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept
    {
        using VType = std::common_type_t<TType, UType>;

        return std::less<VType>()(lhs.Get(), rhs.Get());
    }

    template <typename TType, typename UType>
    inline bool operator>(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept
    {
        return rhs < lhs;
    }

    template <typename TType, typename UType>
    inline bool operator<=(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept
    {
        return !(rhs < lhs);
    }

    template <typename TType, typename UType>
    inline bool operator>=(const ObserverPtr<TType>& lhs, const ObserverPtr<UType>& rhs) noexcept
    {
        return !(lhs < rhs);
    }

    template <typename TType>
    inline void Swap(ObserverPtr<TType>& lhs, ObserverPtr<TType>& rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}