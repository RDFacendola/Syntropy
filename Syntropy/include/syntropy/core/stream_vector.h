
/// \file stream_vector.h
/// \brief This header is part of the Syntropy containers module. It contains classes used to handle structure-of-arrays.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <cstdint>
#include <tuple>

#include "syntropy/language/language_types.h"
#include "syntropy/core/vector.h"
#include "syntropy/core/vector_view.h"
#include "syntropy/language/type_traits.h"

namespace Syntropy
{
    /************************************************************************/
    /* STREAM VECTOR                                                        */
    /************************************************************************/

    /// \brief Packs together multiple parallel vectors (streams).
    /// This vector is used to quickly iterate through streams, organizing data according to structure-of-arrays paradigm.
    /// \author Raffaele D. Facendola - December 2018
    template <typename... TStreams>
    class StreamVector
    {
        static_assert(sizeof...(TStreams) > 0);

    public:

        /// \brief Default constructor.
        StreamVector() = default;

        /// \brief Default copy constructor.
        StreamVector(const StreamVector& rhs) = default;

        /// \brief Default move constructor.
        StreamVector(StreamVector&& rhs) = default;

        /// \brief Default assignment operator.
        StreamVector& operator=(StreamVector& rhs) = default;

        /// \brief Default destructor.
        ~StreamVector() = default;

        /// \brief Returns a reference to the element at specified index in a given stream.
        /// \return Returns the reference to the requested element in the specified stream.
        template <Int kStream>
        auto& GetElementAt(Int index);

        /// \brief Returns a reference to the element at specified index in a given stream.
        /// \return Returns the reference to the requested element in the specified stream.
        template <Int kStream>
        const auto& GetElementAt(Int index) const;

        /// \brief Returns a reference to the element at specified index in a given stream type.
        /// \return Returns the reference to the requested element in the specified stream.
        template <typename TElement>
        TElement& GetElementAt(Int index);

        /// \brief Returns a reference to the element at specified index in a given stream type.
        /// \return Returns the reference to the requested element in the specified stream.
        template <typename TElement>
        const TElement& GetElementAt(Int index) const;

        /// \brief Returns a reference to the first element of a given stream, by stream index.
        /// \return Returns the reference to the first element in the specified stream.
        template <Int kStream>
        auto& GetFront();

        /// \brief Returns a reference to the first element of a given stream, by stream index.
        /// \return Returns the reference to the first element in the specified stream.
        template <Int kStream>
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
        template <Int kStream>
        auto& GetBack();

        /// \brief Returns a reference to the last element of a given stream.
        /// \return Returns the reference to the last element in the specified stream.
        template <Int kStream>
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
        Bool IsEmpty() const noexcept;

        /// \brief Get the number of elements in the container.
        /// \return Returns the number of elements in the container.
        Int GetSize() const noexcept;

        /// \brief Increase the capacity of the vector to a value that's greater or equal to capacity. If capacity is greater than the current GetCapacity(), new storage is allocated, otherwise the method does nothing.
        void Reserve(Int capacity);

        /// \brief Get the number of elements that can be contained in the current allocation storage.
        Int GetCapacity() const noexcept;

        /// \brief Reduce memory usage by reducing unused memory.
        void ShrinkToFit();

        /// \brief Erase all elements from the container.
        void Clear() noexcept;

        /// \brief Emplace an element at the end of the container.
        template <typename... TValues>
        void PushBack(TValues&&... values);

        /// \brief Remove the last element of the container.
        void PopBack();

        /// \brief Remove the index-th element of the container and replaces it with the last element in the container.
        /// The order of the container is not preserved as a result of this call.
        /// /// \param index Index of the element to remove.
        void EraseSwap(Int index);

        /// \brief Execute an operation on each element of the given streams, by stream index.
        template <Int... kStreams, typename TOperation>
        void ForEach(TOperation&& operation);

        /// \brief Execute an operation on each element of the given streams, by stream index.
        template <Int... kStreams, typename TOperation>
        void ForEach(TOperation&& operation) const;

        /// \brief Execute an operation on each element of the given streams, by stream type.
        template <typename... TStreamTypes, typename TOperation>
        void ForEach(TOperation&& operation);

        /// \brief Execute an operation on each element of the given streams, by stream type.
        template <typename... TStreamTypes, typename TOperation>
        void ForEach(TOperation&& operation) const;

        /// \brief Get a view into a stream by index.
        template <Int kStream>
        auto GetStream();

        /// \brief Get a view into a constant stream by index.
        template <Int kStream>
        auto GetStream() const;

        /// \brief Get a view into a constant stream by index.
        template <Int kStream>
        auto GetConstStream() const;

        /// \brief Get a view into a stream by type.
        template <typename TStream>
        VectorView<TStream> GetStream();

        /// \brief Get a view into a constant stream by type.
        template <typename TStream>
        VectorView<const TStream> GetStream() const;

        /// \brief Get a view into a constant stream by type.
        template <typename TStream>
        VectorView<const TStream> GetConstStream() const;

    private:

        /// \brief Appends the given element values to the end of a given stream.
        template <Int kStream, typename TValue, typename... TValues>
        void PushBack(TValue&& value, TValues&&... values);

        /// \brief Appends the given element value to the end of a given stream.
        template <Int kStream>
        void PushBack();

        /// \brief Execute an operation on each stream.
        template <typename TOperation>
        void ForEachStream(TOperation&& operation);

        /// \brief Execute an operation on each indexed stream.
        template <typename TOperation, Int... kStreamIndexes>
        void ForEachStream(TOperation&& operation, std::index_sequence<kStreamIndexes...>);

        /// \brief Vector streams.
        /// \remarks Constness is removed to cope with standard requirements. The stream is immutable from outside perspective, though.
        std::tuple<Array<RemoveConstT<TStreams>>...> streams_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StreamVector<T...>.

    template <typename... TStreams>
    template <Int kStream>
    inline auto& StreamVector<TStreams...>::GetElementAt(Int index)
    {
        using TElement = std::tuple_element_t<kStream, std::tuple<TStreams...>>;

        return static_cast<TElement&>(std::get<kStream>(streams_)[index]);
    }

    template <typename... TStreams>
    template <Int kStream>
    inline const auto& StreamVector<TStreams...>::GetElementAt(Int index) const
    {
        return std::get<kStream>(streams_)[index];
    }

    template <typename... TStreams>
    template <typename TElement>
    inline TElement& StreamVector<TStreams...>::GetElementAt(Int index)
    {
        return GetElementAt<TupleElementIndexV<TElement, std::tuple<TStreams...>>>(index);
    }

    template <typename... TStreams>
    template <typename TElement>
    inline const TElement& StreamVector<TStreams...>::GetElementAt(Int index) const
    {
        return GetElementAt<TupleElementIndexV<TElement, std::tuple<TStreams...>>>(index);
    }

    template <typename... TStreams>
    template <Int kStream>
    inline auto& StreamVector<TStreams...>::GetFront()
    {
        using TElement = std::tuple_element_t<kStream, std::tuple<TStreams...>>;

        return static_cast<TElement&>(std::get<kStream>(streams_).front());
    }

    template <typename... TStreams>
    template <Int kStream>
    inline const auto& StreamVector<TStreams...>::GetFront() const
    {
        return std::get<kStream>(streams_).front();
    }

    template <typename... TStreams>
    template <typename TElement>
    inline TElement& StreamVector<TStreams...>::GetFront()
    {
        return GetFront<TupleElementIndexV<TElement, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    template <typename TElement>
    inline const TElement& StreamVector<TStreams...>::GetFront() const
    {
        return GetFront<TupleElementIndexV<TElement, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    template <Int kStream>
    inline auto& StreamVector<TStreams...>::GetBack()
    {
        using TElement = std::tuple_element_t<kStream, std::tuple<TStreams...>>;

        return static_cast<TElement&>(std::get<kStream>(streams_).back());
    }

    template <typename... TStreams>
    template <Int kStream>
    inline const auto& StreamVector<TStreams...>::GetBack() const
    {
        return std::get<kStream>(streams_).back();
    }

    template <typename... TStreams>
    template <typename TElement>
    inline TElement& StreamVector<TStreams...>::GetBack()
    {
        return GetBack<TupleElementIndexV<TElement, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    template <typename TElement>
    inline const TElement& StreamVector<TStreams...>::GetBack() const
    {
        return GetBack<TupleElementIndexV<TElement, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    inline Bool StreamVector<TStreams...>::IsEmpty() const noexcept
    {
        return std::get<0>(streams_).empty();
    }

    template <typename... TStreams>
    inline Int StreamVector<TStreams...>::GetSize() const noexcept
    {
        return std::get<0>(streams_).size();
    }

    template <typename... TStreams>
    inline void StreamVector<TStreams...>::Reserve(Int capacity)
    {
        ForEachStream([capacity](auto& stream)
        {
            stream.reserve(capacity);
        });
    }

    template <typename... TStreams>
    inline Int StreamVector<TStreams...>::GetCapacity() const noexcept
    {
        return std::get<0>(streams_).capacity();
    }

    template <typename... TStreams>
    inline void StreamVector<TStreams...>::ShrinkToFit()
    {
        ForEachStream([](auto& stream)
        {
            stream.shrink_to_fit();
        });
    }

    template <typename... TStreams>
    inline void StreamVector<TStreams...>::Clear() noexcept
    {
        ForEachStream([](auto& stream)
        {
            stream.clear();
        });
    }

    template <typename... TStreams>
    template <typename... TValues>
    inline void StreamVector<TStreams...>::PushBack(TValues&&... values)
    {
        static_assert(sizeof...(TValues) == sizeof...(TStreams), "Invalid argument count.");

        PushBack<0>(std::forward<TValues>(values)...);
    }

    template <typename... TStreams>
    inline void StreamVector<TStreams...>::PopBack()
    {
        ForEachStream([](auto& stream)
        {
            stream.pop_back();
        });
    }

    template <typename... TStreams>
    inline void StreamVector<TStreams...>::EraseSwap(Int index)
    {
        ForEachStream([index](auto& stream)
        {
            if (index < static_cast<Int>(stream.size()) - 1)
            {
                stream[index] = std::move(stream.back());
            }

            stream.pop_back();
        });
    }

    template <typename... TStreams>
    template <Int... kStreams, typename TOperation>
    inline void StreamVector<TStreams...>::ForEach(TOperation&& operation)
    {
        for (auto index = 0; index < GetSize(); ++index)
        {
            operation(GetElementAt<kStreams>(index)...);
        }
    }

    template <typename... TStreams>
    template <Int... kStreams, typename TOperation>
    inline void StreamVector<TStreams...>::ForEach(TOperation&& operation) const
    {
        for (auto index = 0; index < GetSize(); ++index)
        {
            operation(GetElementAt<kStreams>(index)...);
        }
    }

    template <typename... TStreams>
    template <typename... TStreamTypes, typename TOperation>
    inline void StreamVector<TStreams...>::ForEach(TOperation&& operation)
    {
        ForEach<TupleElementIndexV<TStreamTypes, std::tuple<TStreams...>>...>(std::forward<TOperation>(operation));
    }

    template <typename... TStreams>
    template <typename... TStreamTypes, typename TOperation>
    inline void StreamVector<TStreams...>::ForEach(TOperation&& operation) const
    {
        ForEach<TupleElementIndexV<TStreamTypes, std::tuple<TStreams...>>...>(std::forward<TOperation>(operation));
    }

    template <typename... TStreams>
    template <Int kStream>
    inline auto StreamVector<TStreams...>::GetStream()
    {
        using TElement = std::tuple_element_t<kStream, std::tuple<TStreams...>>;

        return VectorView<TElement>{std::get<kStream>(streams_)};
    }

    template <typename... TStreams>
    template <Int kStream>
    inline auto StreamVector<TStreams...>::GetStream() const
    {
        return GetConstStream<kStream>();
    }

    template <typename... TStreams>
    template <Int kStream>
    inline auto StreamVector<TStreams...>::GetConstStream() const
    {
        return MakeConstVectorView(std::get<kStream>(streams_));
    }

    template <typename... TStreams>
    template <typename TStream>
    inline VectorView<TStream> StreamVector<TStreams...>::GetStream()
    {
        return GetStream<TupleElementIndexV<TStream, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    template <typename TStream>
    inline VectorView<const TStream> StreamVector<TStreams...>::GetStream() const
    {
        return GetConstStream<TStream>();
    }

    template <typename... TStreams>
    template <typename TStream>
    inline VectorView<const TStream> StreamVector<TStreams...>::GetConstStream() const
    {
        return GetConstStream<TupleElementIndexV<TStream, std::tuple<TStreams...>>>();
    }

    template <typename... TStreams>
    template <Int kStream, typename TValue, typename... TValues>
    inline void StreamVector<TStreams...>::PushBack(TValue&& value, TValues&&... values)
    {
        std::get<kStream>(streams_).emplace_back(std::forward<TValue>(value));

        PushBack<kStream + 1>(std::forward<TValues>(values)...);
    }

    template <typename... TStreams>
    template <Int kStream>
    inline void StreamVector<TStreams...>::PushBack()
    {

    }

    template <typename... TStreams>
    template <typename TOperation>
    inline void StreamVector<TStreams...>::ForEachStream(TOperation&& operation)
    {
        ForEachStream(std::forward<TOperation>(operation), std::index_sequence_for<TStreams...>{});
    }

    template <typename... TStreams>
    template <typename TOperation, Int... kStreamIndexes>
    inline void StreamVector<TStreams...>::ForEachStream(TOperation&& operation, std::index_sequence<kStreamIndexes...>)
    {
        (operation(std::get<kStreamIndexes>(streams_)), ...);
    }

}
