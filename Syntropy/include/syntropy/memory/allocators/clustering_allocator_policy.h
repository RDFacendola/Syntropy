
/// \file clustering_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains different clustering allocator policies.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"
#include "syntropy/memory/memory_range.h"

namespace syntropy
{
    /************************************************************************/
    /* LINEAR CLUSTERING ALLOCATOR POLICY                                   */
    /************************************************************************/

    /// \brief A policy to be used with syntropy::ClusteringAllocator. Each cluster size grows linearly with the cluster index.
    /// \author Raffaele D. Facendola - August 2018
    struct LinearClusteringAllocatorPolicy
    {
        /// \brief Create a new policy from an explicit base size.
        LinearClusteringAllocatorPolicy(Bytes base_size = 8_Bytes, Bytes class_size = 8_Bytes);

        /// \brief Get the size of a cluster given its index.
        /// \param cluster_index Index of the cluster to get the size of.
        /// \return Returns the size of the requested cluster.
        Bytes GetSize(std::size_t cluster_index) const;

        /// \brief Get the index of the cluster able to fit a memory block given its size.
        /// \param size Size of the memory block.
        /// \return Returns the index of the cluster able to fit a block of the provided size.
        std::size_t GetIndex(Bytes size) const;

        /// \brief Get the index of the cluster able to fit an aligned memory block given its size.
        /// \param size Size of the memory block.
        /// \param alignment Block alignment.
        /// \return Returns the index of the cluster able to fit a block of the provided size with given alignment.
        std::size_t GetIndex(Bytes size, Alignment alignment) const;

    private:

        Bytes base_size_;           ///< \brief Maximum size that can be handled by the first cluster.

        Bytes class_size_;          ///< \brief Capacity increment from two consecutive clusters, in bytes.

    };

    /************************************************************************/
    /* EXPONENTIAL CLUSTERING ALLOCATOR POLICY                              */
    /************************************************************************/

    /// \brief A policy to be used with syntropy::ClusteringAllocator. Each cluster size grows exponentially with the cluster index, by doubling its capacity at each iteration.
    /// \author Raffaele D. Facendola - September 2018
    struct ExponentialClusteringAllocatorPolicy
    {
        /// \brief Create a new policy from an explicit base size.
        ExponentialClusteringAllocatorPolicy(Bytes base_size = 0_Bytes, Bytes class_size = 8_Bytes);

        /// \brief Get the size of a cluster given its index.
        /// \param cluster_index Index of the cluster to get the size of.
        /// \return Returns the size of the requested cluster.
        Bytes GetSize(std::size_t cluster_index) const;

        /// \brief Get the index of the cluster able to fit a memory block given its size.
        /// \param size Size of the memory block.
        /// \return Returns the index of the cluster able to fit a block of the provided size.
        std::size_t GetIndex(Bytes size) const;

        /// \brief Get the index of the cluster able to fit an aligned memory block given its size.
        /// \param size Size of the memory block.
        /// \param alignment Block alignment.
        /// \return Returns the index of the cluster able to fit a block of the provided size with given alignment.
        std::size_t GetIndex(Bytes size, Alignment alignment) const;

    private:

        Bytes base_size_;           ///< \brief Maximum size that can be handled by the first cluster. 

        Bytes class_size_;          ///< \brief Capacity increment from the first cluster to the second. This size is doubled for each successive cluster, in bytes.

    };

}

namespace syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline LinearClusteringAllocatorPolicy::LinearClusteringAllocatorPolicy(Bytes base_size, Bytes class_size)
        : base_size_(base_size)
        , class_size_(class_size)
    {

    }

    inline Bytes LinearClusteringAllocatorPolicy::GetSize(std::size_t cluster_index) const
    {
        return base_size_ + class_size_ * cluster_index;
    }

    inline std::size_t LinearClusteringAllocatorPolicy::GetIndex(Bytes size) const
    {
        if (size <= base_size_)
        {
            return 0u;
        }

        return ((size - base_size_ - 1_Bytes) / class_size_) + 1u;
    }

    inline std::size_t LinearClusteringAllocatorPolicy::GetIndex(Bytes size, Alignment /*alignment*/) const
    {
        return GetIndex(size);  // Assumes the underlying cluster is able to handle any alignment. Provide a different policy if required.
    }

    /************************************************************************/
    /* EXPONENTIAL CLUSTERING ALLOCATOR POLICY                              */
    /************************************************************************/

    inline ExponentialClusteringAllocatorPolicy::ExponentialClusteringAllocatorPolicy(Bytes base_size, Bytes class_size)
        : base_size_(base_size)
        , class_size_(class_size)
    {

    }

    inline Bytes ExponentialClusteringAllocatorPolicy::GetSize(std::size_t cluster_index) const
    {
        if (cluster_index == 0u)
        {
            return base_size_;
        }

        return base_size_ + class_size_ << (cluster_index - 1u);
    }

    inline std::size_t ExponentialClusteringAllocatorPolicy::GetIndex(Bytes size) const
    {
        if (size <= base_size_)
        {
            return 0u;
        }

        return Math::CeilLog2(((size - base_size_ - 1_Bytes) / class_size_) + 1u) + 1u;
    }

    inline std::size_t ExponentialClusteringAllocatorPolicy::GetIndex(Bytes size, Alignment /*alignment*/) const
    {
        return GetIndex(size);  // Assumes the underlying cluster is able to handle any alignment. Provide a different policy if required.
    }
}

