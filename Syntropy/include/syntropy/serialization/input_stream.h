
/// \file input_stream.h
/// \brief This header is part of the Syntropy serialization module. It contains definition for input stream.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/memory/memory_range.h"
#include "syntropy/language/type_traits.h"
#include "syntropy/memory/bytes.h"

namespace syntropy
{
    /************************************************************************/
    /* INPUT STREAM                                                         */
    /************************************************************************/

    /// \brief Interface for a class which binds to a input stream-like object and exposes sequential input functionalities.
    /// \author Raffaele D. Facendola - June 2020
    class InputStream
    {
    public:

        /// \brief Default virtual destructor.
        virtual ~InputStream() = default;

        /// \brief Read data from the input stream.
        template <typename TData>
        InputStream& operator>>(TData& data);

        /// \brief Consume data from the input stream.
        InputStream& operator>>(const MemoryRange& data);

        /// \brief Check whether the underlying stream is empty.
        virtual Bool IsEmpty() const = 0;

        /// \brief Get the stream content size, in bytes.
        virtual Bytes GetSize() const = 0;

    private:

        /// \brief Read and consume data sequentially from the stream.
        /// \return Returns the range containing read data.
        virtual MemoryRange Consume(const MemoryRange& data) = 0;

    };

    /************************************************************************/
    /* INPUT STREAM T <T STREAM>                                            */
    /************************************************************************/

    /// \brief Adapter class which binds to a input stream-like object and exposes input functionalities only.
    /// \author Raffaele D. Facendola - June 2020
    template <typename TStream>
    class InputStreamT : public InputStream
    {
    public:

        /// \brief Create a new output stream bound to a input stream object.
        /// The provided stream must outlive this object, otherwise the behavior of the class is undefined.
        InputStreamT(TStream& stream);

        /// \brief Default copy constructor.
        InputStreamT(const InputStreamT& other) = default;

        /// \brief Default move constructor.
        InputStreamT(InputStreamT&& other) = default;

        /// \brief Default copy-assignment operator.
        InputStreamT& operator=(const InputStreamT& other) = default;

        /// \brief Default move-assignment operator.
        InputStreamT& operator=(InputStreamT&& other) = default;

        /// \brief Default destructor.
        virtual ~InputStreamT() = default;

        virtual Bool IsEmpty() const override;

        virtual Bytes GetSize() const override;

    private:

        virtual MemoryRange Consume(const MemoryRange& data) override;

        /// \brief Underlying stream.
        ObserverPtr<TStream> stream_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create an input stream deducing templates from arguments.
    template <typename TStream>
    InputStreamT<TStream> MakeInputStream(TStream& stream);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // InputStream.

    template <typename TData>
    inline InputStream& InputStream::operator>>(TData& data)
    {
        return (*this) >> MakeMemoryRange(data);
    }

    inline InputStream& InputStream::operator>>(const MemoryRange& data)
    {
        Consume(data);

        return *this;
    }

    // InputStreamT<TStream>.

    template <typename TStream>
    inline InputStreamT<TStream>::InputStreamT(TStream& stream)
        : stream_(&stream)
    {

    }

    template <typename TStream>
    inline MemoryRange InputStreamT<TStream>::Consume(const MemoryRange& data)
    {
        return stream_->Consume(data);
    }

    template <typename TStream>
    inline Bool InputStreamT<TStream>::IsEmpty() const
    {
        return stream_->IsEmpty();
    }

    template <typename TStream>
    inline Bytes InputStreamT<TStream>::GetSize() const
    {
        return stream_->GetSize();
    }

    // Non-member functions.

    template <typename TStream>
    inline InputStreamT<TStream> MakeInputStream(TStream& stream)
    {
        return InputStreamT<TStream>(stream);
    }

}

