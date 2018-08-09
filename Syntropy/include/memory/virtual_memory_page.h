
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

        /// \brief Create an empty memory page.
        constexpr VirtualMemoryPage() = default;

        /// \brief Create a new virtual memory page.
        VirtualMemoryPage(MemoryAddress begin, MemoryAddress end);

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

        /// \brief Get the first address in the memory page.
        /// \return Returns the first address in the memory page.
        constexpr MemoryAddress Begin() const noexcept;

        /// \brief Get the one past the last address in the memory page.
        /// \return Returns one past the the last address in the memory page.
        constexpr MemoryAddress End() const noexcept;

        /// \brief Get the virtual memory page size.
        /// \return Returns the virtual memory page size, in bytes.
        constexpr Bytes GetSize() const noexcept;

        /// \brief Check whether an address falls within this memory page.
        /// \param address Address to check.
        /// \return Returns true if address is contained inside this memory page, returns false otherwise.
        constexpr bool Contains(MemoryAddress address) const noexcept;

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

    VirtualMemoryPage::VirtualMemoryPage(MemoryAddress begin, MemoryAddress end)
        : memory_range_(begin, end)
    {

    }

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

    constexpr MemoryAddress VirtualMemoryPage::Begin() const noexcept
    {
        return memory_range_.Begin();
    }

    constexpr MemoryAddress VirtualMemoryPage::End() const noexcept
    {
        return memory_range_.End();
    }
    
    constexpr Bytes VirtualMemoryPage::GetSize() const noexcept
    {
        return memory_range_.GetSize();
    }

    constexpr bool VirtualMemoryPage::Contains(MemoryAddress address) const noexcept
    {
        return memory_range_.Contains(address);
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
        return MemoryRange(lhs).Begin() > MemoryRange(rhs).Begin();
    }

    constexpr bool operator<(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).Begin() < MemoryRange(rhs).Begin();
    }

    constexpr bool operator>=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).Begin() >= MemoryRange(rhs).Begin();
    }

    constexpr bool operator<=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).Begin() <= MemoryRange(rhs).Begin();
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
        auto difference = MemoryRange(rhs).Begin() - MemoryRange(lhs).Begin();

        return difference / intptr_t(std::size_t(VirtualMemoryPage::GetSize()));
    }

}

