
/// \file stream_writer.h
/// \brief This header is part of the Syntropy serialization module. It contains for stream writer classes.
///
/// \author Raffaele D. Facendola - 2020

#include "syntropy/core/types.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/serialization/output_stream.h"

namespace syntropy
{
    /************************************************************************/
    /* STREAM WRITER T <T ENCODER <>>                                       */
    /************************************************************************/

    /// \brief Exposes methods to sequentially write structured data to an output stream.
    /// \author Raffaele D. Facendola - June 2020.
    template <template <typename> typename TEncoder>
    class StreamWriterT
    {
    public:

        /// \brief Create a new writer bound to an output stream.
        StreamWriterT(OutputStream& output_stream);

        /// \brief Default copy constructor.
        StreamWriterT(const StreamWriterT&) = default;

        /// \brief Default move constructor.
        StreamWriterT(StreamWriterT&&) = default;

        /// \brief Default copy-assignment constructor.
        StreamWriterT& operator=(const StreamWriterT&) = default;

        /// \brief Default move-assignment constructor.
        StreamWriterT& operator=(StreamWriterT&&) = default;

        /// \brief Default destructor.
        ~StreamWriterT() = default;

        /// \brief Write data sequentially to the underlying stream.
        template <typename UType>
        StreamWriterT& operator<<(const UType& data);

    private:

        /// \brief Underlying output stream.
        ObserverPtr<OutputStream> output_stream_;

    };

    /************************************************************************/
    /* RAW STREAM ENCODER                                                   */
    /************************************************************************/

    /// \brief Represents a simple encoder that encodes values using their object-representation.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TType>
    struct RawStreamEncoder
    {
        /// \brief Encode rhs and write the result to an output stream.
        void operator()(OutputStream& lhs, const TType& rhs) const;
    };

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Type alias for a raw stream writer.
    using StreamWriter = StreamWriterT<RawStreamEncoder>;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StreamWriterT.

    template <template <typename> typename TEncoder>
    inline StreamWriterT<TEncoder>::StreamWriterT(OutputStream& output_stream)
        : output_stream_(&output_stream)
    {

    }

    template <template <typename> typename TEncoder>
    template <typename UType>
    inline StreamWriterT<TEncoder>& StreamWriterT<TEncoder>::operator<<(const UType& data)
    {
        TEncoder<UType>{}(*output_stream_, data);

        return *this;
    }

    // RawStreamEncoder.

    template <typename TType>
    inline void RawStreamEncoder<TType>::operator()(OutputStream& lhs, const TType& rhs) const
    {
        lhs.WriteSequential(MakeConstMemoryRange(rhs));
    }

}