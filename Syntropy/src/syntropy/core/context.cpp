#include "syntropy/core/context.h"

#include <tuple>

#include "syntropy/core/map.h"

#include "syntropy/allocators/virtual_stack_allocator.h"
#include "syntropy/allocators/fallback_allocator.h"
#include "syntropy/allocators/system_allocator.h"
#include "syntropy/allocators/allocator.h"

namespace Syntropy
{
    using namespace Literals;

    /************************************************************************/
    /* CONTEXT :: REGISTRY                                                  */
    /************************************************************************/

    /// \brief Singleton class used to store contexts.
    /// \author Raffaele D. Facendola - May 2020.
    class Context::Registry
    {
    public:

        /// \brief Get the singleton instance.
        static Registry& GetSingleton();

        /// \brief Get a context by name.
        const Context* GetOuterContext(const Context::TStringView& context_name);

    private:

        /// \brief Allocate a new entry.
        Pointer<Context> Allocate(const Context::TStringView& context_name);

        /// \brief Type of the memory resource used to store context. Contexts are never deallocated.
        /// In the unlikely event the virtual memory range is exhausted, the system memory resource is used as a last resort.
        using TMemoryResource = AllocatorT<FallbackAllocator<VirtualStackAllocator, SystemAllocator>>;

        /// \brief Private constructor.
        Registry();

        /// \brief Memory resource used for dynamic memory allocation.
        TMemoryResource memory_resource_;

        /// \brief Root context.
        Context root_context_;

        /// \brief Context registered so far, indexed by full name.
        Map<Label, const Context*> contexts_;

    };

    /************************************************************************/

    Context::Registry& Context::Registry::GetSingleton()
    {
        static auto singleton = Registry();

        return singleton;
    }

    Context::Registry::Registry()
        : memory_resource_(std::forward_as_tuple(1_MiBytes, 64_KiBytes), Tags::kDefaultConstruct)
        , contexts_(memory_resource_)
    {
        // #TODO Configure memory resource from data.
    }

    const Context* Context::Registry::GetOuterContext(const Context::TStringView& context_name)
    {
        // Find or allocate an outer context.

        if (auto separator_index = context_name.find_first_of(Context::kSeparator); separator_index != Context::TStringView::npos)
        {
            // Outer name - discard until a separator is found.

            auto outer_context_name = context_name;

            outer_context_name.remove_prefix(separator_index + 1);

            auto outer_context_label = Label(outer_context_name);

            if (auto outer_context_iterator = contexts_.find(outer_context_label); outer_context_iterator != contexts_.end())
            {
                // The outer context already exists.

                return outer_context_iterator->second;
            }
            else
            {
                // The outer doesn't exist: add it now. Allocation will cause recursive outer allocation if necessary.

                auto outer_context = Allocate(outer_context_name);

                contexts_.insert(std::make_pair(outer_context_name, outer_context));

                return outer_context;
            }
        }

        // The outermost context of any other context is the root.

        return &root_context_;
    }

    Pointer<Context> Context::Registry::Allocate(const Context::TStringView& context_name)
    {
        auto storage = reinterpret_cast<RWPointer<Context>>(memory_resource_.Allocate(Memory::SizeOf<Context>(), Memory::AlignmentOf<Context>()).GetData());

        new (storage) Context(context_name);

        return storage;
    }

    /************************************************************************/
    /* CONTEXT                                                              */
    /************************************************************************/

    Context::Context(const TStringView& name)
        : name_(name)
        , outer_(Registry::GetSingleton().GetOuterContext(name))
    {

    }

}
