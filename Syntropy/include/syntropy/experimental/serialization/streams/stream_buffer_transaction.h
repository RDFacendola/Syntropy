
/// \file stream_buffer_transaction.h
/// \brief This header is part of the Syntropy serialization module. It contains definitions for a stream buffer transactions.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation.h"

#include "syntropy/experimental/serialization/streams/stream_buffer.h"

namespace Syntropy
{
    /************************************************************************/
    /* STREAM BUFFER TRANSACTION                                            */
    /************************************************************************/

    /// \brief Represents a RAII transactional context in which append\consume operations performed on a stream buffer are either committed or rollback'd all at once.
    /// Transactions are automatically committed on transaction destruction, unless rollback'd.
    /// Transactions can be nested but overlapping results in undefined behavior.
    /// \author Raffaele D. Facendola - June 2020.
    class StreamBufferTransaction
    {
    public:

        /// \brief No default constructor.
        StreamBufferTransaction() = delete;

        /// \brief No copy constructor.
        StreamBufferTransaction(const StreamBufferTransaction&) = delete;

        /// \brief No assignment operator.
        StreamBufferTransaction & operator=(const StreamBufferTransaction&) = delete;

        /// \brief Move constructor. The original copy gets invalidated.
        StreamBufferTransaction(StreamBufferTransaction&& other);

        /// \brief Starts a new transaction.
        StreamBufferTransaction(StreamBuffer& stream_buffer);

        /// \brief Destroy this transaction, causing either data to be committed or rollback'd unless invalidated.
        ~StreamBufferTransaction();

        /// \brief Rollback the transaction.
        void Rollback();

    private:

        /// \brief Stream buffer. This variable is nullptr if the transaction is invalidated.
        RWPointer<StreamBuffer> stream_buffer_{ nullptr };

        /// \brief Parent transaction.
        RWPointer<StreamBufferTransaction> parent_{ nullptr };

        /// \brief Whether the transaction was rollback'd.
        Bool rollback_{ false };

        /// \brief Size of appended bytes when the transaction was created.
        Bytes append_size_;

        /// \brief Size of consumed bytes when the transaction was created.
        Bytes consume_size_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // StreamBufferTransaction.

    inline StreamBufferTransaction::StreamBufferTransaction(StreamBufferTransaction&& other)
        : stream_buffer_(other.stream_buffer_)
        , parent_(other.parent_)
        , rollback_(other.rollback_)
        , append_size_(other.append_size_)
        , consume_size_(other.consume_size_)
    {
        other.stream_buffer_ = nullptr;
        other.parent_ = nullptr;
        other.append_size_ = {};
        other.consume_size_ = {};
    }

    inline StreamBufferTransaction::StreamBufferTransaction(StreamBuffer& stream_buffer)
        : stream_buffer_(&stream_buffer)
        , parent_(stream_buffer.transaction_)
        , append_size_(stream_buffer.append_size_)
        , consume_size_(stream_buffer.consume_size_)
    {
        stream_buffer.transaction_ = this;
    }

    inline StreamBufferTransaction::~StreamBufferTransaction()
    {
        if (stream_buffer_)
        {
            SYNTROPY_ASSERT(stream_buffer_->transaction_ == this);          // Only the innermost transaction can be destroyed.

            stream_buffer_->transaction_ = parent_;                         // Set parent transaction as the active one.

            if (rollback_)
            {
                stream_buffer_->Rollback(append_size_, consume_size_);      // Rollback the innermost transaction.
            }
            else if (!parent_)
            {
                stream_buffer_->Commit(append_size_, consume_size_);        // Commit the outermost transaction.
            }
        }
    }

    inline void StreamBufferTransaction::Rollback()
    {
        rollback_ = true;
    }

}

