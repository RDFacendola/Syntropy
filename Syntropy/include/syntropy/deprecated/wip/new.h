#pragma once


//     /// \brief New operator overload for custom allocators.
//     void* operator new (std::size_t size, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);
// 
//     /// \brief Delete expression for custom allocators.
//     /// Used to *deallocate* object allocated via custom allocators.
//     void operator delete (void* ptr, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);

//    void* operator new (std::size_t size, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
//    {
//        auto ptr = allocator.Allocate(syntropy::Bytes(size));
//
//        SYNTROPY_LOG((allocator), "Allocating ", size, " bytes. Address: ", ptr, ". Caller: ", stack_trace);
//
//        return ptr;
//    }
//
//    void operator delete (void* ptr, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
//    {
//        allocator.Free(ptr);
//
//        SYNTROPY_LOG((allocator), "Deallocating memory. Address: ", ptr, ". Caller: ", stack_trace);
//    }