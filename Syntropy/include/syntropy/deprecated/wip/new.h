#pragma once


//     /// \brief New operator overload for custom allocators.
//     void* operator new (std::size_t size, Syntropy::Allocator& allocator, const Syntropy::diagnostics::StackTrace& stack_trace);
// 
//     /// \brief Delete expression for custom allocators.
//     /// Used to *deallocate* object allocated via custom allocators.
//     void operator delete (void* ptr, Syntropy::Allocator& allocator, const Syntropy::diagnostics::StackTrace& stack_trace);

//    void* operator new (std::size_t size, Syntropy::Allocator& allocator, const Syntropy::diagnostics::StackTrace& stack_trace)
//    {
//        auto ptr = allocator.Allocate(Syntropy::Bytes(size));
//
//        SYNTROPY_LOG((allocator), "Allocating ", size, " bytes. Address: ", ptr, ". Caller: ", stack_trace);
//
//        return ptr;
//    }
//
//    void operator delete (void* ptr, Syntropy::Allocator& allocator, const Syntropy::diagnostics::StackTrace& stack_trace)
//    {
//        allocator.Free(ptr);
//
//        SYNTROPY_LOG((allocator), "Deallocating memory. Address: ", ptr, ". Caller: ", stack_trace);
//    }