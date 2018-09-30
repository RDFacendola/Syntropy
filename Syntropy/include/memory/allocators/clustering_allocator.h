
/// \file clustering_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains an allocator used to cluster allocations according to different sizes.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"
#include "memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* CLUSTERING ALLOCATOR                                                 */
    /************************************************************************/

    /// \brief Segregating allocator that organizes allocations according to clusters of different sizes. Each size is handled by a specific sub-allocator TClusterAllocator.
    /// Clusters share a common underlying allocator of type TAllocator.
    ///
    /// Based on "Building a low-fragmentation memory system for 64-bit games" by Aaron MacDougall - GDC16
    /// 
    /// \tparam TAllocator Type of the underlying allocator.
    /// \tparam TClusterAllocator Type of each cluster allocator.
    /// \tparam TPolicy Policy used to map each size to a cluster index and vice-versa.
    /// 
    /// \author Raffaele D. Facendola - December 2016, September 2018
    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    class ClusteringAllocator
    {
    public:

        /// \brief Create a new allocator.
        /// \param order Number of allocation classes handled by the allocator.
        /// \param cluster_constructor Functor used to construct a class allocator. Must be of the form: (TAllocator&, Size) -> TClusterAllocator.
        /// \param policy Allocator policy.
        /// \param arguments Arguments to pass to the underlying allocator.
        template <typename TClusterAllocatorConstructor, typename... TArguments>
        ClusteringAllocator(size_t order, TClusterAllocatorConstructor&& cluster_constructor, TPolicy policy, TArguments&&... arguments);

        /// \brief No copy constructor.
        ClusteringAllocator(const ClusteringAllocator&) = delete;

        /// \brief Move constructor.
        ClusteringAllocator(ClusteringAllocator&& rhs) noexcept;

        /// \brief Destructor.
        ~ClusteringAllocator();

        /// \brief Unified assignment operator.
        ClusteringAllocator& operator=(ClusteringAllocator rhs) noexcept;

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate.
        /// \return Returns a range representing the requested memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size) noexcept;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate.
        /// \param alignment Block alignment.
        /// \return Returns a range representing the requested aligned memory block. If no allocation could be performed returns an empty range.
        MemoryRange Allocate(Bytes size, Alignment alignment) noexcept;

        /// \brief Deallocate a memory block.
        /// \param block Block to deallocate.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size).
        void Deallocate(const MemoryRange& block);

        /// \brief Deallocate an aligned memory block.
        /// \param block Block to deallocate. Must refer to any allocation performed via Allocate(size, alignment).
        /// \param alignment Block alignment.
        /// \remarks The behavior of this function is undefined unless the provided block was returned by a previous call to ::Allocate(size, alignment).
        void Deallocate(const MemoryRange& block, Alignment alignment);

        /// \brief Check whether this allocator owns the provided memory block.
        /// \param block Block to check the ownership of.
        /// \return Returns true if the provided memory range was allocated by this allocator, returns false otherwise.
        bool Owns(const MemoryRange& block) const noexcept;

        /// \brief Get the maximum allocation size that can be handled by this allocator.
        /// The returned value shall not be used to determine whether a call to "Allocate" will fail.
        /// \return Returns the maximum allocation size that can be handled by this allocator.
        Bytes GetMaxAllocationSize() const noexcept;

        /// \brief Swap this allocator with the provided instance.
        void Swap(ClusteringAllocator& rhs) noexcept;

    private:

        /// \brief Get a cluster allocator from an index.
        /// \param cluster_index Index of the cluster allocator.
        TClusterAllocator* GetCluster(std::size_t cluster_index) noexcept;

        /// \brief Get a cluster allocator able to handle the provided allocation request.
        /// \param size Size of the allocation to handle.
        TClusterAllocator* GetCluster(Bytes size) noexcept;

        /// \brief Get a cluster allocator able to handle the provided aligned allocation request.
        /// \param size Size of the allocation to handle.
        /// \param alignment Alignment of the allocation to handle.
        TClusterAllocator* GetCluster(Bytes size, Alignment alignment) noexcept;

        TAllocator allocator_;                                  ///< \brief Underlying allocator shared among the cluster allocators.

        MemoryRange clusters_;                                  ///< \brief Cluster allocators. Allocators are stored contiguously.

        std::size_t order_{ 0u };                               ///< \brief Order of the allocator, determines the number of sub-allocators.

        TPolicy policy_;                                        ///< \brief Policy used to map orders to cluster sizes and vice-versa.
    };

}

namespace syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    template <typename TClusterAllocatorConstructor, typename... TArguments>
    inline ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::ClusteringAllocator(size_t order, TClusterAllocatorConstructor&& cluster_constructor, TPolicy policy, TArguments&&... arguments)
        : allocator_(std::forward<TArguments>(arguments)...)
        , clusters_(allocator_.Allocate(order * Bytes(sizeof(TClusterAllocator))))
        , order_(order)
        , policy_(std::move(policy))
    {
        for (auto cluster_index = 0u; cluster_index < order_; ++cluster_index)
        {
            new (GetCluster(cluster_index)) TClusterAllocator(cluster_constructor(allocator_, policy_.GetSize(cluster_index)));
        }
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::ClusteringAllocator(ClusteringAllocator&& rhs) noexcept
        : allocator_(std::move(rhs.allocator_))
        , clusters_(std::move(rhs.clusters_))
        , order_(std::move(rhs.order_))
        , policy_(std::move(rhs.policy_))
    {
        rhs.clusters_ = MemoryRange();
        rhs.order_ = 0u;
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::~ClusteringAllocator()
    {
        // Destroy each cluster allocator.

        for(auto order = 0u; order < order_; ++order)
        {
            GetCluster(order)->~TClusterAllocator();
        }

        // Deallocate from the underlying memory range.

        if (clusters_)
        {
            allocator_.Deallocate(clusters_);
        }
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>& ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::operator=(ClusteringAllocator rhs) noexcept
    {
        rhs.Swap(*this);
        return *this;
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline MemoryRange ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::Allocate(Bytes size) noexcept
    {
        if (auto cluster = GetCluster(size))
        {
            return cluster->Allocate(size);
        }

        return {};
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline MemoryRange ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::Allocate(Bytes size, Alignment alignment) noexcept
    {
        if (auto cluster = GetCluster(size, alignment))
        {
            return cluster->Allocate(size, alignment);
        }

        return {};
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline void ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::Deallocate(const MemoryRange& block)
    {
        auto size = block.GetSize();

        if (auto cluster = GetCluster(size))
        {
            cluster->Deallocate(block);
        }
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline void ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::Deallocate(const MemoryRange& block, Alignment alignment)
    {
        auto size = block.GetSize();

        if (auto cluster = GetCluster(size, alignment))
        {
            cluster->Deallocate(block, alignment);
        }
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline bool ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::Owns(const MemoryRange& block) const noexcept
    {
        return allocator_.Owns(block);
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline Bytes ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::GetMaxAllocationSize() const noexcept
    {
        return policy_.GetSize(order_);
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline void ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::Swap(ClusteringAllocator& rhs) noexcept
    {
        using std::swap;

        swap(allocator_, rhs.allocator_);
        swap(clusters_, rhs.clusters_);
        swap(order_, rhs.order_);
        swap(policy_, rhs.policy_);
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline TClusterAllocator* ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::GetCluster(std::size_t cluster_index) noexcept
    {
        if (cluster_index < order_)
        {
            return &(clusters_.Begin().As<TClusterAllocator>()[cluster_index]);
        }

        return nullptr;
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline TClusterAllocator* ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::GetCluster(Bytes size) noexcept
    {
        return GetCluster(policy_.GetIndex(size));
    }

    template <typename TAllocator, typename TClusterAllocator, typename TPolicy>
    inline TClusterAllocator* ClusteringAllocator<TAllocator, TClusterAllocator, TPolicy>::GetCluster(Bytes size, Alignment alignment) noexcept
    {
        return GetCluster(policy_.GetIndex(size, alignment));
    }

}

