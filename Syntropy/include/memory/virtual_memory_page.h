
/// \file virtual_memory_page.h
/// \brief This header is part of the syntropy memory management system. It contains classes and definition for virtual memory pages manipulation.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY PAGE                                                  */
    /************************************************************************/

    /// \brief Represents a virtual memory page.
    /// \author Raffaele D. Facendola - August 2018
    class VirtualMemoryPage
    {
    public:

        /// \brief Get the virtual memory page size.
        /// \return Returns the virtual memory page size, in bytes.
        static Bytes GetSize();

        /// \brief Get the virtual memory page alignment.
        /// \return Returns the virtual memory page alignment, in bytes.
        static Alignment GetAlignment();

        /// \brief Create a en empty address.
        constexpr VirtualMemoryPage() = default;

        /// \brief Default copy-constructor.
        VirtualMemoryPage(const VirtualMemoryPage&) = default;

        /// \brief Default assignment operator.
        VirtualMemoryPage& operator=(const VirtualMemoryPage&) = default;

        /// \brief Get the underlying memory range this page refers to.
        /// \return Returns the underlying pointer.
        constexpr operator const MemoryRange&() const noexcept;

        /// \brief Advance the memory page forward.
        /// \param rhs Number of pages to move forward to.
        /// \return Returns a reference to this element.
        constexpr VirtualMemoryPage& operator+=(std::size_t rhs) noexcept;

        /// \brief Move the memory page backwards.
        /// \param rhs Number of pages to move backwards to.
        /// \return Returns a reference to this element.
        constexpr VirtualMemoryPage& operator-=(std::size_t rhs) noexcept;

        /// \brief Commit the memory page, making it accessible by the application.
        /// \remarks When committing more than one memory page prefer VirtualMemoryRange::Commit() to this method.
        void Commit();

        /// \brief Decommit the memory page, making it inaccessible by the application.
        /// \remarks When decommitting more than one memory page prefer VirtualMemoryRange::Decommit() to this method.
        void Decommit();

    private:

        MemoryRange memory_range_;              ///< \brief Virtual memory range.

    };

    /// \brief Equality comparison for VirtualMemoryPage.
    /// \return Returns true if lhs and rhs refer to the same memory page, returns false otherwise.
    constexpr bool operator==(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Inequality comparison for VirtualMemoryPage.
    /// \return Returns true if lhs and rhs refer to different memory pages, returns false otherwise.
    constexpr bool operator!=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Greater-than comparison for VirtualMemoryPage.
    /// \return Returns true if lhs refers to a page whose address is strictly greater than rhs, returns false otherwise.
    constexpr bool operator>(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Less-than comparison for VirtualMemoryPage.
    /// \return Returns true if lhs refers to a page whose address is strictly less than rhs, returns false otherwise.
    constexpr bool operator<(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Greater-or-equal comparison for VirtualMemoryPage.
    /// \return Returns true if lhs refers to a page whose address is equal or greater than rhs, returns false otherwise.
    constexpr bool operator>=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Less-or-equal comparison for MemoryAddress.
    /// \return Returns true if lhs refers to a page whose address is equal or less than rhs, returns false otherwise.
    constexpr bool operator<=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Move a memory page forward.
    /// \return Returns a memory page which is equal to lhs moved forward by rhs pages.
    constexpr VirtualMemoryPage operator+(const VirtualMemoryPage& lhs, std::size_t rhs) noexcept;

    /// \brief Move a memory page backward.
    /// \return Returns a memory page which is equal to lhs moved backward by rhs pages.
    constexpr VirtualMemoryPage operator-(const VirtualMemoryPage& lhs, std::size_t rhs) noexcept;

    /// \brief Get the difference of two memory pages.
    /// \return Returns a the signed difference between lhs and rhs, in pages.
    constexpr ptrdiff_t operator-(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    constexpr VirtualMemoryPage::operator const MemoryRange&() const noexcept
    {
        return memory_range_;
    }

    constexpr VirtualMemoryPage& VirtualMemoryPage::operator+=(std::size_t rhs) noexcept
    {
        memory_range_ += GetSize() * rhs;
        return *this;
    }

    constexpr VirtualMemoryPage& VirtualMemoryPage::operator-=(std::size_t rhs) noexcept
    {
        memory_range_ -= GetSize() * rhs;
        return *this;
    }

    constexpr bool operator==(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs) = MemoryRange(rhs);
    }

    constexpr bool operator!=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator>(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).GetBase() > MemoryRange(rhs).GetBase();
    }

    constexpr bool operator<(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).GetBase() < MemoryRange(rhs).GetBase();
    }

    constexpr bool operator>=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).GetBase() >= MemoryRange(rhs).GetBase();
    }

    constexpr bool operator<=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).GetBase() <= MemoryRange(rhs).GetBase();
    }

    constexpr VirtualMemoryPage operator+(const VirtualMemoryPage& lhs, std::size_t rhs) noexcept
    {
        return VirtualMemoryPage(lhs) += rhs;
    }

    constexpr VirtualMemoryPage operator-(const VirtualMemoryPage& lhs, std::size_t rhs) noexcept
    {
        return VirtualMemoryPage(lhs) -= rhs;
    }

    constexpr ptrdiff_t operator-(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        auto difference = MemoryRange(rhs).GetBase() - MemoryRange(lhs).GetBase();

        return difference / intptr_t(std::size_t(VirtualMemoryPage::GetSize()));
    }

}

