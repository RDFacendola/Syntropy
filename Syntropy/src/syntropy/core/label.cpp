#include "syntropy/core/label.h"

#include "syntropy/core/map.h"

#include "syntropy/allocators/linear_virtual_memory_resource.h"
#include "syntropy/allocators/fallback_memory_resource.h"
#include "syntropy/allocators/system_memory_resource.h"
#include "syntropy/allocators/memory_resource.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/memory.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/memory_address.h"

#include "syntropy/math/hash.h"

namespace syntropy
{
    using namespace Literals;

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
        using TMemoryResource = MemoryResourceT<FallbackMemoryResource<LinearVirtualMemoryResource, SystemMemoryResource>>;

        /// \brief Type of a label hash.
        using THash = Int;

        /// \brief Private constructor.
        Registry();

        /// \brief Allocate a new entry.
        const TChar* Allocate(const ConstMemoryRange& string_range);

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
        : memory_resource_(std::forward_as_tuple(128_MiBytes, 64_KiBytes), kDefaultConstruct)
        , labels_(memory_resource_)
    {
        // #TODO Configure memory resource from data.
    }

    const Label::TChar* Label::Registry::Insert(const Label::TStringView& string)
    {
        using std::begin;
        using std::end;

        auto string_range = MakeMemoryRange(begin(string), end(string));

        auto label_hash = Hash::FNV1a64(string_range);

        if (auto label_iterator = labels_.lower_bound(label_hash); label_iterator != labels_.end())
        {
            // The label already exists.

            return label_iterator->second;
        }
        else
        {
            // The label doesn't exist: allocate a new one.

            auto label = Allocate(string_range);

            labels_.insert(label_iterator, std::make_pair(label_hash, label));

            return label;
        }
    }

    const Label::TChar* Label::Registry::Allocate(const ConstMemoryRange& string_range)
    {
        // One extra byte accounts for the null terminator.

        auto label_storage = memory_resource_.Allocate(string_range.GetSize() + 1_Bytes, AlignmentOf<Label::TChar*>());

        auto label = label_storage.Begin().As<const TChar>();

        Memory::Zero(label_storage);
        Memory::Copy(label_storage.PopBack(), string_range);

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