
/// \file stream_vector_view.h
/// \brief This header is part of the syntropy containers. It contains classes used to handle views to stream vectors.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <cstdint>
#include <tuple>
#include <vector>
#include <type_traits>
#include <algorithm>

#include "syntropy/containers/stream_vector.h"
#include "syntropy/containers/vector_view.h"

#include "syntropy/type_traits.h"

namespace syntropy
{
    /************************************************************************/
    /* STREAM VECTOR VIEW                                                   */
    /************************************************************************/

    /// \brief Packs together multiple parallel vectors views, each referring to a single attribute of an element.
    /// This vector is used to quickly iterate through an element attribute, allowing the data to be organized according to the structure-of-arrays paradigm.
    /// \author Raffaele D. Facendola - January 2020.
    template <typename... TStreams>
    class StreamVectorView
    {
        static_assert(sizeof...(TStreams) > 0);

    public:

        /// \brief Create a stream vector view from explicit streams.
        template <typename... UStreams>
        StreamVectorView(VectorView<UStreams>... streams);

        /// \brief Default copy constructor.
        StreamVectorView(const StreamVectorView& rhs) = default;

        /// \brief Default move constructor.
        StreamVectorView(StreamVectorView&& rhs) = default;

        /// \brief Default assignment operator.
        StreamVectorView& operator=(StreamVectorView& rhs) = default;

        /// \brief Default destructor.
        ~StreamVectorView() = default;

        /// \brief Returns a reference to the element at specified index in a given stream.
        /// \return Returns the reference to the requested element in the specified stream.
        template <std::int64_t kStream>
        auto& GetElementAt(std::int64_t index);

        /// \brief Returns a reference to the element at specified index in a given stream.
        /// \return Returns the reference to the requested element in the specified stream.
        template <std::int64_t kStream>
        const auto& GetElementAt(std::int64_t index) const;

        /// \brief Returns a reference to the element at specified index in a given stream type.
        /// \return Returns the reference to the requested element in the specified stream.
        template <typename TElement>
        TElement& GetElementAt(std::int64_t index);

        /// \brief Returns a reference to the element at specified index in a given stream type.
        /// \return Returns the reference to the requested element in the specified stream.
        template <typename TElement>
        const TElement& GetElementAt(std::int64_t index) const;

        /// \brief Returns a reference to the first element of a given stream.
        /// \return Returns the reference to the first element in the specified stream.
        template <std::int64_t kStream>
        auto& GetFront();

        /// \brief Returns a reference to the first element of a given stream.
        /// \return Returns the reference to the first element in the specified stream.
        template <std::int64_t kStream>
        const auto& GetFront() const;

        /// \brief Returns a reference to the first element of a given stream, by stream type.
        /// \return Returns the reference to the first element in the specified stream.
        template <typename TElement>
        TElement& GetFront();

        /// \brief Returns a reference to the first element of a given stream, by stream type.
        /// \return Returns the reference to the first element in the specified stream.
        template <typename TElement>
        const TElement& GetFront() const;

        /// \brief Returns a reference to the last element of a given stream.
        /// \return Returns the reference to the last element in the specified stream.
        template <std::int64_t kStream>
        auto& GetBack();

        /// \brief Returns a reference to the last element of a given stream.
        /// \return Returns the reference to the last element in the specified stream.
        template <std::int64_t kStream>
        const auto& GetBack() const;

        /// \brief Returns a reference to the last element of a given stream, by stream type.
        /// \return Returns the reference to the last element in the specified stream.
        template <typename TElement>
        TElement& GetBack();

        /// \brief Returns a reference to the last element of a given stream, by stream type.
        /// \return Returns the reference to the last element in the specified stream.
        template <typename TElement>
        const TElement& GetBack() const;

        /// \brief Check whether the container is empty.
        /// \return Returns true if the container is empty, returns false otherwise.
        bool IsEmpty() const noexcept;

        /// \brief Get the number of elements in the shortest stream among those with the specified indices.
        /// \return Returns the number of elements in the shortest stream among those with the specified indices.
        template <std::int64_t... kStreams>
        std::int64_t GetSize() const noexcept;

        /// \brief Get the number of elements in the shortest stream among those with the specified indices.
        /// \return Returns the number of elements in the shortest stream among those with the specified indices.
        template <typename... UStreams>
        std::int64_t GetSize() const noexcept;

        /// \brief Get the number of elements in the shortest stream.
        /// \return Returns the number of elements in the shortest stream.
        std::int64_t GetSize() const noexcept;

        /// \brief Execute an operation on each element of the given streams.
        template <std::int64_t... kStreams, typename TOperation>
        void ForEach(TOperation&& operation);

        /// \brief Execute an operation on each element of the given streams.
        template <std::int64_t... kStreams, typename TOperation>
        void ForEach(TOperation&& operation) const;

        /// \brief Execute an operation on each element of the given streams, by stream type.
        template <typename... TStreamTypes, typename TOperation>
        void ForEach(TOperation&& operation);

        /// \brief Execute an operation on each element of the given streams, by stream type.
        template <typename... TStreamTypes, typename TOperation>
        void ForEach(TOperation&& operation) const;

        /// \brief Get a view into a stream by index.
        template <std::int64_t kStream>
        auto GetStream() const;

        /// \brief Get a view into a constant stream by index.
        template <std::int64_t kStream>
        auto GetConstStream() const;

        /// \brief Get a view into a stream by type.
        template <typename TStream>
        VectorView<TStream> GetStream() const;

        /// \brief Get a view into a constant stream by type.
        template <typename TStream>
        VectorView<const TStream> GetConstStream() const;

    private:

        /// \brief Vector stream views.
        std::tuple<VectorView<TStreams>...> streams_;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a stream vector view by projecting streams from a stream vector.
    template <typename... TStreams, typename... UStreams>
    StreamVectorView<TStreams...> Project(StreamVector<UStreams...>& vector);

    /// \brief Create a stream vector view by projecting streams from a stream vector.
    template <typename... TStreams, typename... UStreams>
    StreamVectorView<const TStreams...> Project(const StreamVector<UStreams...>& vector);

    /// \brief Create a stream vector view by projecting streams from a stream vector view.
    template <typename... TStreams, typename... UStreams>
    StreamVectorView<TStreams...> Project(const StreamVectorView<UStreams...>& vector_view);

    /// \brief Create a const stream vector view by projecting streams from a stream vector.
    template <typename... TStreams, typename... UStreams>
    StreamVectorView<const TStreams...> ConstProject(const StreamVector<UStreams...>& vector);

    /// \brief Create a const stream vector view by projecting streams from a stream vector view.
    template <typename... TStreams, typename... UStreams>
    StreamVectorView<const TStreams...> ConstProject(const StreamVectorView<UStreams...>& vector_view);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StreamVectorView<T...>.

    template <typename... TStreams>
    template <typename... UStreams>
    inline StreamVectorView<TStreams...>::StreamVectorView(VectorView<UStreams>... streams)
        : streams_(streams...)
    {

    }

    template <typename... TStreams>
    template <std::int64_t kStream>
    inline auto& StreamVectorView<TStreams...>::GetElementAt(std::int64_t index)
    {
        return std::get<kStream>(streams_)[index];
    }

    template <typename... TStreams>
    template <std::int64_t kStream>
    inline const auto& StreamVectorView<TStreams...>::GetElementAt(std::int64_t index) const
    {
        return std::get<kStream>(streams_)[index];
    }

    template <typename... TStreams>
    template <typename TElement>
    inline TElement& StreamVectorView<TStreams...>::GetElementAt(std::int64_t index)
    {
        return GetElementAt<tuple_element_index_v<TElement, std::tuple<TStreams...>>>(index);
    }

    template <typename... TStreams>
    template <typename TElement>
    inline const TElement& StreamVectorView<TStreams...>::GetElementAt(std::int64_t index) const
    {
        return GetElementAt<tuple_element_index_v<TElement, std::tuple<TStreams...>>>(index);
    }

    template <typename... TStreams>
    template <std::int64_t kStream>
    inline auto& StreamVectorView<TStreams...>::GetFront()
    {
        return std::get<kStream>(streams_).GetFront();
    }

    template <typename... TStreams>
    template <std::int64_t kStream>
    inline const auto& StreamVectorView<TStreams...>::GetFront() const
    {
        return std::get<kStream>(streams_).GetFront();
    }

    template <typename... TStreams>
    template <typename TElement>
    inline TElement& StreamVectorView<TStreams...>::GetFront()
    {
        return GetFront<tuple_element_index_v<TElement, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    template <typename TElement>
    inline const TElement& StreamVectorView<TStreams...>::GetFront() const
    {
        return GetFront<tuple_element_index_v<TElement, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    template <std::int64_t kStream>
    inline auto& StreamVectorView<TStreams...>::GetBack()
    {
        return std::get<kStream>(streams_).GetBack();
    }

    template <typename... TStreams>
    template <std::int64_t kStream>
    inline const auto& StreamVectorView<TStreams...>::GetBack() const
    {
        return std::get<kStream>(streams_).GetBack();
    }

    template <typename... TStreams>
    template <typename TElement>
    inline TElement& StreamVectorView<TStreams...>::GetBack()
    {
        return GetBack<tuple_element_index_v<TElement, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    template <typename TElement>
    inline const TElement& StreamVectorView<TStreams...>::GetBack() const
    {
        return GetBack<tuple_element_index_v<TElement, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    inline bool StreamVectorView<TStreams...>::IsEmpty() const noexcept
    {
        return GetSize() == 0;
    }

    template <typename... TStreams>
    template <std::int64_t... kStreams>
    inline std::int64_t StreamVectorView<TStreams...>::GetSize() const noexcept
    {
        return std::min({ std::get<kStreams>(streams_).GetSize()... });
    }

    template <typename... TStreams>
    template <typename... UStreams>
    inline std::int64_t StreamVectorView<TStreams...>::GetSize() const noexcept
    {
        return GetSize<tuple_element_index_v<UStreams, std::tuple<TStreams...>>...>();
    }

    template <typename... TStreams>
    inline std::int64_t StreamVectorView<TStreams...>::GetSize() const noexcept
    {
        return GetSize<TStreams...>();
    }

    template <typename... TStreams>
    template <std::int64_t... kStreams, typename TOperation>
    inline void StreamVectorView<TStreams...>::ForEach(TOperation&& operation)
    {
        auto size = GetSize<kStreams...>();

        for (auto index = 0u; index < size; ++index)
        {
            operation(GetElementAt<kStreams>(index)...);
        }
    }

    template <typename... TStreams>
    template <std::int64_t... kStreams, typename TOperation>
    inline void StreamVectorView<TStreams...>::ForEach(TOperation&& operation) const
    {
        auto size = GetSize<kStreams>();

        for (auto index = 0u; index < size; ++index)
        {
            operation(GetElementAt<kStreams>(index)...);
        }
    }

    template <typename... TStreams>
    template <typename... TStreamTypes, typename TOperation>
    inline void StreamVectorView<TStreams...>::ForEach(TOperation&& operation)
    {
        ForEach<tuple_element_index_v<TStreamTypes, std::tuple<TStreams...>>...>(std::forward<TOperation>(operation));
    }

    template <typename... TStreams>
    template <typename... TStreamTypes, typename TOperation>
    inline void StreamVectorView<TStreams...>::ForEach(TOperation&& operation) const
    {
        ForEach<tuple_element_index_v<TStreamTypes, std::tuple<TStreams...>>...>(std::forward<TOperation>(operation));
    }

    template <typename... TStreams>
    template <std::int64_t kStream>
    inline auto StreamVectorView<TStreams...>::GetStream() const
    {
        return std::get<kStream>(streams_);
    }

    template <typename... TStreams>
    template <std::int64_t kStream>
    inline auto StreamVectorView<TStreams...>::GetConstStream() const
    {
        return MakeConstVectorView(std::get<kStream>(streams_));
    }

    template <typename... TStreams>
    template <typename TStream>
    inline VectorView<TStream> StreamVectorView<TStreams...>::GetStream() const
    {
        return GetStream<tuple_element_index_v<TStream, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    template <typename TStream>
    inline VectorView<const TStream> StreamVectorView<TStreams...>::GetConstStream() const
    {
        return GetConstStream<tuple_element_index_v<TStream, std::tuple<TStreams...>>>();
    }

    // Non-member functions.

    template <typename... TStreams, typename... UStreams>
    inline StreamVectorView<TStreams...> Project(StreamVector<UStreams...>& vector)
    {
        return StreamVectorView<TStreams...>(vector.GetStream<TStreams>()...);
    }

    template <typename... TStreams, typename... UStreams>
    inline StreamVectorView<const TStreams...> Project(const StreamVector<UStreams...>& vector)
    {
        return StreamVectorView<const TStreams...>(vector.GetStream<TStreams>()...);
    }

    template <typename... TStreams, typename... UStreams>
    inline StreamVectorView<TStreams...> Project(const StreamVectorView<UStreams...>& vector_view)
    {
        return StreamVectorView<TStreams...>(vector_view.GetStream<TStreams>()...);
    }

    template <typename... TStreams, typename... UStreams>
    inline StreamVectorView<const TStreams...> ConstProject(const StreamVector<UStreams...>& vector)
    {
        return StreamVectorView<const TStreams...>(vector.GetConstStream<TStreams>()...);
    }

    template <typename... TStreams, typename... UStreams>
    inline StreamVectorView<const TStreams...> ConstProject(const StreamVectorView<UStreams...>& vector_view)
    {
        return StreamVectorView<const TStreams...>(vector_view.GetConstStream<TStreams>()...);
    }

}