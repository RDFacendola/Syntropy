
/// \file output_stream_buffer.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for output streams.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/type_traits.h"
#include "syntropy/serialization/memory_stream_buffer.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/memory/bytes.h"

namespace syntropy
{
    /************************************************************************/
    /* OUTPUT STREAM                                                        */
    /************************************************************************/

    /// \brief Interface for a class which binds to an output stream-like object and exposes sequential output functionalities only.
    /// \author Raffaele D. Facendola - June 2020
    class OutputStream
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~OutputStream() = default;

        /// \brief Write data sequentially to the stream, causing it to grow.
        /// \return Returns the range containing unwritten data.
        virtual ConstMemoryRange WriteSequential(const ConstMemoryRange& data) = 0;

        /// \brief Increase the underlying buffer allocation size.
        /// This method may cause buffer reallocation.
        /// If the underlying stream doesn't support this operation, this method behaves as no-op.
        virtual void Reserve(Bytes capacity) = 0;

        /// \brief Shrink the allocation size up to the current buffer size.
        /// This method preserve the stream content and may reallocate the underlying buffer.
        /// If the underlying stream doesn't support this operation, this method behaves as no-op.
        virtual void Shrink() = 0;

        /// \brief Discard stream content.
        /// If the underlying stream doesn't support this operation, this method behaves as no-op.
        virtual void Discard() = 0;

    };

    /************************************************************************/
    /* OUTPUT STREAM T <T STREAM>                                           */
    /************************************************************************/

    /// \brief Adapter class which binds to an output stream-like object and exposes output functionalities only.
    /// \author Raffaele D. Facendola - June 2020
    template <typename TStream>
    class OutputStreamT : public OutputStream
    {
    public:

        /// \brief Create a new output stream bound to a stream object.
        /// The provided stream must outlive this object, otherwise the behavior of the class is undefined.
        OutputStreamT(TStream& stream);

        /// \brief Default copy constructor.
        OutputStreamT(const OutputStreamT& other) = default;

        /// \brief Default move constructor.
        OutputStreamT(OutputStreamT&& other) = default;

        /// \brief Default copy-assignment operator.
        OutputStreamT& operator=(const OutputStreamT& other) = default;

        /// \brief Default move-assignment operator.
        OutputStreamT& operator=(OutputStreamT&& other) = default;

        /// \brief Default destructor.
        virtual ~OutputStreamT() = default;

        virtual ConstMemoryRange WriteSequential(const ConstMemoryRange& data) override;

        virtual void Reserve(Bytes capacity) override;

        virtual void Shrink() override;

        virtual void Discard() override;

    private:

        /// \brief Predicate used to detect whether the underlying stream supports the "Reserve(Byte)" method.
        template <typename TStream>
        using IsReserveSupported = decltype(std::declval<TStream>().Reserve(Bytes{}));

        /// \brief Predicate used to detect whether the underlying stream supports the "Shrink()" method.
        template <typename TStream>
        using IsShrinkSupported = decltype(std::declval<TStream>().Shrink());

        /// \brief Predicate used to detect whether the underlying stream supports the "Discard()" method.
        template <typename TStream>
        using IsDiscardSupported = decltype(std::declval<TStream>().Discard());

        /// \brief Underlying stream.
        ObserverPtr<TStream> stream_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create an output stream deducing templates from arguments.
    template <typename TStream>
    OutputStreamT<TStream> MakeOutputStream(TStream& stream);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // OutputStreamT<TStreamBuffer>.

    template <typename TStream>
    inline OutputStreamT<TStream>::OutputStreamT(TStream& stream)
        : stream_(&stream)
    {

    }

    template <typename TStream>
    inline ConstMemoryRange OutputStreamT<TStream>::WriteSequential(const ConstMemoryRange& data)
    {
        return stream_->WriteSequential(data);
    }

    template <typename TStream>
    inline void OutputStreamT<TStream>::Reserve(Bytes capacity)
    {
        if constexpr (IsValidExpressionV<IsReserveSupported, TStream>)
        {
            stream_->Reserve(capacity);
        }
    }

    template <typename TStream>
    inline void OutputStreamT<TStream>::Shrink()
    {
        if constexpr (IsValidExpressionV<IsShrinkSupported, TStream>)
        {
            stream_->Shrink();
        }
    }

    template <typename TStream>
    inline void OutputStreamT<TStream>::Discard()
    {
        if constexpr (IsValidExpressionV<IsDiscardSupported, TStream>)
        {
            stream_->Discard();
        }
    }

    // Non-member functions.

    template <typename TStream>
    inline OutputStreamT<TStream> MakeOutputStream(TStream& stream)
    {
        return OutputStreamT<TStream>(stream);
    }

}

