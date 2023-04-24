
// ################################################################################

// @author Raffaele D. Facendola - April 2023

// ################################################################################

module syntropy.buffer;

// ################################################################################

// ================================================================================
// BUFFER
// ================================================================================

sy::Buffer::Buffer(Allocator& allocator)
    : _allocator{ allocator }
{

}

sy::Buffer::Buffer(Bytes size, Allocator& allocator)
    : _allocator{ allocator }
    , _data{ _allocator.Allocate(size, GetMaxAlignment()) }
    , _alignment{ GetMaxAlignment() }
{

}

sy::Buffer::Buffer(Bytes size, Alignment alignment, Allocator& allocator)
    : _allocator{ allocator }
    , _data{ _allocator.Allocate(size, alignment) }
    , _alignment{ alignment }
{

}

sy::Buffer::~Buffer()
{
    if (_data.GetCount() > 0)
    {
        _allocator.Deallocate(_data, _alignment);
    }
}

sy::Buffer::operator sy::ByteSpan() const
{
    return _data;
}

sy::Buffer::operator sy::RWByteSpan() const
{
    return _data;
}

sy::Allocator& sy::Buffer::GetAllocator() const
{
    return _allocator;
}

sy::Bytes sy::Buffer::GetSize() const
{
    return Bytes{ _data.GetCount() };
}

sy::Alignment sy::Buffer::GetAlignment() const
{
    return _alignment;
}

// ################################################################################
