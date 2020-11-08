#include "syntropy/core/strings/label.h"

#include "syntropy/core/data_structures/map.h"

#include "syntropy/allocators/virtual_stack_allocator.h"
#include "syntropy/allocators/fallback_allocator.h"
#include "syntropy/allocators/system_allocator.h"
#include "syntropy/allocators/allocator.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory.h"
#include "syntropy/memory/byte_span.h"

#include "syntropy/math/hash.h"

namespace Syntropy
{
    using namespace Memory::Literals;

    /************************************************************************/
    /* LABEL :: REGISTRY                                                    */
    /************************************************************************/

    /// \brief Singleton class used to store labels.
    /// \author Raffaele D. Facendola - April 2020.
    class Label::Registry
    {
    public:

        /// \brief Get the singleton instance.
        static Registry& GetSingleton();

        /// \brief Insert a string in memory and return a unique location for it.
        /// \remarks Two equal strings are guaranteed to point to the same exact memory location.
        const TChar* Insert(const TStringView& string);

    private:

        /// \brief Type of the memory resource used to store labels. Labels are never deallocated.
        /// In the unlikely event the virtual memory range is exhausted, the system memory resource is used as a last resort.
        using TMemoryResource = AllocatorT<FallbackAllocator<VirtualStackAllocator, SystemAllocator>>;

        /// \brief Type of a label hash.
        using THash = Int;

        /// \brief Private constructor.
        Registry();

        /// \brief Allocate a new entry.
        const TChar* Allocate(const Memory::ByteSpan& string_range);

        /// \brief Memory resource used for dynamic memory allocation.
        TMemoryResource memory_resource_;

        /// \brief Labels registered so far, each associated to its hash.
        Map<THash, const TChar*> labels_;

    };

    /************************************************************************/

    Label::Registry& Label::Registry::GetSingleton()
    {
        static auto singleton = Registry();

        return singleton;
    }

    Label::Registry::Registry()
        : memory_resource_(std::forward_as_tuple(128_MiBytes, 64_KiBytes), FallbackAllocator<VirtualStackAllocator, SystemAllocator>::DefaultConstruct{})
        , labels_(memory_resource_)
    {
        // #TODO Configure memory resource from data.
    }

    const Label::TChar* Label::Registry::Insert(const Label::TStringView& string)
    {
        using std::begin;
        using std::end;

        auto string_range = Memory::ByteSpan(Memory::ToBytePtr(string.data()), ToInt(string.length() * Memory::SizeOf<TChar>()));

        auto label_hash = Hash::FNV1a64(string_range);

        if(auto label_iterator = labels_.find(label_hash); label_iterator != labels_.end())
        {
            // The label already exists.

            return label_iterator->second;
        }
        else
        {
            // The label doesn't exist: allocate a new one.

            auto label = Allocate(string_range);

            labels_.insert(std::make_pair(label_hash, label));

            return label;
        }
    }

    const Label::TChar* Label::Registry::Allocate(const Memory::ByteSpan& string_range)
    {
        // One extra byte accounts for the null terminator.

        auto label_storage = memory_resource_.Allocate(Memory::Size(string_range) + 1_Bytes, Memory::AlignmentOf<Label::TChar*>());

        auto label = reinterpret_cast<Pointer<TChar>>(label_storage.GetData());

        Memory::Zero(label_storage);

        Memory::Copy(PopBack(label_storage), string_range);

        return label;
    }

    /************************************************************************/
    /* LABEL                                                                */
    /************************************************************************/

    Label::Label(const Label::TStringView& string)
        : characters_(Registry::GetSingleton().Insert(string))
    {

    }

}