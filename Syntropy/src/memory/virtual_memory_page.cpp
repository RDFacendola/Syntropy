#include "memory/virtual_memory_page.h"

#include "platform/os/os.h"

namespace syntropy
{
    /************************************************************************/
    /* VIRTUAL MEMORY PAGE                                                  */
    /************************************************************************/

    Bytes VirtualMemoryPage::GetSize()
    {
        return platform::PlatformMemory::GetPageSize();
    }

    Alignment VirtualMemoryPage::GetAlignment()
    {
        return platform::PlatformMemory::GetPageAlignment();
    }

    void VirtualMemoryPage::Commit()
    {
        auto result = platform::PlatformMemory::Commit(memory_range_);

        SYNTROPY_ASSERT(result);
    }

    void VirtualMemoryPage::Decommit()
    {
        auto result = platform::PlatformMemory::Decommit(memory_range_);

        SYNTROPY_ASSERT(result);
    }

}

