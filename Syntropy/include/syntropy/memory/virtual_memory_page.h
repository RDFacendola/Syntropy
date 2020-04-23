
/// \file virtual_memory_page.h
/// \brief This header is part of the syntropy memory management system. It contains classes and definition for virtual memory pages manipulation.
///
/// \author Raffaele D. Facendola - August 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/virtual_memory.h"

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
        VirtualMemoryPage() = default;

        /// \brief Create a new virtual memory page.
        /// \param page_address Initial address of the memory page. Assumes the page size is the same as VirtualMemory::GetPageSize().
        VirtualMemoryPage(MemoryAddress page_address);

        /// \brief Default copy-constructor.
        VirtualMemoryPage(const VirtualMemoryPage&) = default;

        /// \brief Default assignment operator.
        VirtualMemoryPage& operator=(const VirtualMemoryPage&) = default;

        /// \brief Get the underlying memory range this page refers to.
        /// \return Returns the underlying pointer.
        operator const MemoryRange&() const noexcept;

         /// \brief Advance the memory page forward.
         /// \param rhs Number of pages to move forward to.
         /// \return Returns a reference to this element.
         VirtualMemoryPage& operator+=(std::size_t rhs) noexcept;
 
         /// \brief Move the memory page backwards.
         /// \param rhs Number of pages to move backwards to.
         /// \return Returns a reference to this element.
         VirtualMemoryPage& operator-=(std::size_t rhs) noexcept;

        /// \brief Get the first address in the memory page.
        /// \return Returns the first address in the memory page.
        MemoryAddress Begin() const noexcept;

        /// \brief Get the one past the last address in the memory page.
        /// \return Returns one past the the last address in the memory page.
        MemoryAddress End() const noexcept;

        /// \brief Check whether an address falls within this memory page.
        /// \param address Address to check.
        /// \return Returns true if address is contained inside this memory page, returns false otherwise.
        bool Contains(MemoryAddress address) const noexcept;

    private:

        /// \brief Virtual memory range.
        MemoryRange memory_range_;

    };

    /// \brief Equality comparison for VirtualMemoryPage.
    /// \return Returns true if lhs and rhs refer to the same memory page, returns false otherwise.
    bool operator==(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Inequality comparison for VirtualMemoryPage.
    /// \return Returns true if lhs and rhs refer to different memory pages, returns false otherwise.
    bool operator!=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Greater-than comparison for VirtualMemoryPage.
    /// \return Returns true if lhs refers to a page whose address is strictly greater than rhs, returns false otherwise.
    bool operator>(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Less-than comparison for VirtualMemoryPage.
    /// \return Returns true if lhs refers to a page whose address is strictly less than rhs, returns false otherwise.
    bool operator<(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Greater-or-equal comparison for VirtualMemoryPage.
    /// \return Returns true if lhs refers to a page whose address is equal or greater than rhs, returns false otherwise.
    bool operator>=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Less-or-equal comparison for VirtualMemoryPage.
    /// \return Returns true if lhs refers to a page whose address is equal or less than rhs, returns false otherwise.
    bool operator<=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /// \brief Move a memory page forward.
    /// \return Returns a memory page which is equal to lhs moved forward by rhs pages.
    VirtualMemoryPage operator+(const VirtualMemoryPage& lhs, std::size_t rhs) noexcept;

    /// \brief Move a memory page backward.
    /// \return Returns a memory page which is equal to lhs moved backward by rhs pages.
    VirtualMemoryPage operator-(const VirtualMemoryPage& lhs, std::size_t rhs) noexcept;

    /// \brief Get the difference of two memory pages.
    /// \return Returns a the signed difference between lhs and rhs, in pages.
    ptrdiff_t operator-(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline VirtualMemoryPage::VirtualMemoryPage(MemoryAddress page_address)
        : memory_range_(page_address, page_address + VirtualMemory::GetPageSize())
    {
        SYNTROPY_ASSERT(page_address.IsAlignedTo(VirtualMemory::GetPageAlignment()));
    }

    inline VirtualMemoryPage::operator const MemoryRange&() const noexcept
    {
        return memory_range_;
    }

    inline VirtualMemoryPage& VirtualMemoryPage::operator+=(std::size_t rhs) noexcept
    {
        auto offset = memory_range_.GetSize() * rhs;

        memory_range_ = MemoryRange(memory_range_.Begin() + offset, memory_range_.End() + offset);

        return *this;
    }

    inline VirtualMemoryPage& VirtualMemoryPage::operator-=(std::size_t rhs) noexcept
    {
        auto offset = memory_range_.GetSize() * rhs;

        memory_range_ = MemoryRange(memory_range_.Begin() - offset, memory_range_.End() - offset);

        return *this;
    }

    inline MemoryAddress VirtualMemoryPage::Begin() const noexcept
    {
        return memory_range_.Begin();
    }

    inline MemoryAddress VirtualMemoryPage::End() const noexcept
    {
        return memory_range_.End();
    }

    inline bool VirtualMemoryPage::Contains(MemoryAddress address) const noexcept
    {
        return memory_range_.Contains(address);
    }

    inline bool operator==(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs) == MemoryRange(rhs);
    }

    inline bool operator!=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    inline bool operator>(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).Begin() > MemoryRange(rhs).Begin();
    }

    inline bool operator<(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).Begin() < MemoryRange(rhs).Begin();
    }

    inline bool operator>=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).Begin() >= MemoryRange(rhs).Begin();
    }

    inline bool operator<=(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        return MemoryRange(lhs).Begin() <= MemoryRange(rhs).Begin();
    }

    inline VirtualMemoryPage operator+(const VirtualMemoryPage& lhs, std::size_t rhs) noexcept
    {
        return VirtualMemoryPage(lhs) += rhs;
    }

    inline VirtualMemoryPage operator-(const VirtualMemoryPage& lhs, std::size_t rhs) noexcept
    {
        return VirtualMemoryPage(lhs) -= rhs;
    }

    inline ptrdiff_t operator-(const VirtualMemoryPage& lhs, const VirtualMemoryPage& rhs) noexcept
    {
        auto difference = MemoryRange(rhs).Begin() - MemoryRange(lhs).Begin();

        return difference / VirtualMemory::GetPageSize();
    }

}

