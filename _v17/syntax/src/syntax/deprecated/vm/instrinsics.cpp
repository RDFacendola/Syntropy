#include "syntax/vm/intrinsics.h"

#include <string.h>

#include "syntropy/diagnostics/foundation/assert.h"
#include "syntropy/memory/memory_address.h"

namespace syntropy
{
    namespace syntax
    {

        /************************************************************************/
        /* VIRTUAL MACHINE INTRINSICS                                           */
        /************************************************************************/

        void VirtualMachineIntrinsics::Nop(VMExecutionContext& /*context*/)
        {

        }

        void VirtualMachineIntrinsics::Halt(VMExecutionContext& context)
        {
            auto& vm = context.GetVirtualMachine();

            vm.instruction_pointer_ = nullptr;                                                                          // nullptr means that the VM has no other instructions to execute.
        }

        void VirtualMachineIntrinsics::Jump(VMExecutionContext& context)
        {
            auto offset = context.GetNextImmediate<word_t>();

            auto& vm = context.GetVirtualMachine();

            vm.instruction_pointer_ = (MemoryAddress(vm.instruction_pointer_) + Bytes(offset)).As<instruction_t>();     // Jump forward or backward depending on the provided offset.
        }

        void VirtualMachineIntrinsics::Enter(VMExecutionContext& context)
        {
            auto local_storage = context.GetNextImmediate<storage_t>();

            auto& vm = context.GetVirtualMachine();

            *(vm.stack_pointer_++) = reinterpret_cast<word_t>(vm.base_pointer_);                                    // Save the caller's base pointer.

            vm.base_pointer_ = vm.stack_pointer_;                                                                   // Setup a new base pointer for the current frame.
            
            vm.stack_pointer_ = (MemoryAddress(vm.stack_pointer_) + Bytes(local_storage)).As<word_t>();             // Reserve space for local storage.
        }

        void VirtualMachineIntrinsics::Call(VMExecutionContext& /*context*/)
        {
            // #TODO It is not possible to construct a string directly from its hash (what if the hash no associated string?)

            SYNTROPY_ASSERT(false);

            //auto function_name_hash = context.GetNextImmediate<uint64_t>();

            //auto& vm = context.GetVirtualMachine();

            //auto function_pointer = vm.GetFunctionAddress(function_name_hash);                  // Resolve function name. 

            //*(vm.stack_pointer_++) = reinterpret_cast<uintptr_t>(vm.instruction_pointer_);      // Save the caller's instruction pointer. This actually points to the next instruction after "Call".

            //vm.instruction_pointer_ = reinterpret_cast<instruction_t*>(function_pointer);       // Grant control to the callee.
        }

        void VirtualMachineIntrinsics::Return(VMExecutionContext& context)
        {
            auto input_storage = context.GetNextImmediate<storage_t>();

            auto& vm = context.GetVirtualMachine();

            vm.stack_pointer_ = vm.base_pointer_;                                                           // Tear down the local storage by unwinding the stack pointer.

            vm.base_pointer_ = reinterpret_cast<word_t*>(*(--vm.stack_pointer_));                           // Restore the previous base pointer.

            vm.instruction_pointer_ = reinterpret_cast<instruction_t*>(*(--vm.stack_pointer_));             // Restore the previous instruction pointer and return the control to the caller.

            vm.stack_pointer_ = (MemoryAddress(vm.stack_pointer_) - Bytes(input_storage)).As<word_t>();     // Tear down input arguments storage.
        }

        void VirtualMachineIntrinsics::PushWord(VMExecutionContext& context)
        {
            auto source = context.GetNextArgument<word_t>();

            auto& vm = context.GetVirtualMachine();

            *(vm.stack_pointer_++) = *source;                               // Push a word on the stack.
        }

        void VirtualMachineIntrinsics::PushAddress(VMExecutionContext& context)
        {
            auto source = context.GetNextArgument<word_t>();

            auto& vm = context.GetVirtualMachine();

            *(vm.stack_pointer_++) = reinterpret_cast<word_t>(source);      // Push an address on the stack (as word).
        }

        void VirtualMachineIntrinsics::PopWord(VMExecutionContext& context)
        {
            auto destination = context.GetNextArgument<word_t>();

            auto& vm = context.GetVirtualMachine();

            *destination = *(--vm.stack_pointer_);                          // Pop a word from the stack.
        }

        void VirtualMachineIntrinsics::MoveImmediate(VMExecutionContext& context)
        {
            auto destination = context.GetNextArgument<word_t>();
            auto value = context.GetNextImmediate<word_t>();

            *destination = value;
        }

        void VirtualMachineIntrinsics::Move(VMExecutionContext& context)
        {
            auto destination = context.GetNextArgument<word_t>();
            auto source = context.GetNextArgument<word_t>();

            *destination = *source;
        }

        void VirtualMachineIntrinsics::MoveDstIndirect(VMExecutionContext& context)
        {
            auto destination = context.GetNextArgument<word_t*>();
            auto source = context.GetNextArgument<word_t>();

            **destination = *source;
        }

        void VirtualMachineIntrinsics::MoveSrcIndirect(VMExecutionContext& context)
        {
            auto destination = context.GetNextArgument<word_t>();
            auto source = context.GetNextArgument<word_t*>();

            *destination = **source;
        }

        void VirtualMachineIntrinsics::MoveSrcDstIndirect(VMExecutionContext& context)
        {
            auto destination = context.GetNextArgument<word_t*>();
            auto source = context.GetNextArgument<word_t*>();

            **destination = **source;
        }

        void VirtualMachineIntrinsics::MoveAddress(VMExecutionContext& context)
        {
            auto destination = context.GetNextArgument<word_t>();
            auto source = context.GetNextArgument<word_t>();

            *destination = reinterpret_cast<word_t>(source);
        }

        /************************************************************************/
        /* VIRTUAL MACHINE MATH                                                 */
        /************************************************************************/

        void VirtualMachineMath::AddInteger(VMExecutionContext& context)
        {
            auto result = context.GetNextArgument<word_t>();
            auto first = context.GetNextArgument<word_t>();
            auto second = context.GetNextArgument<word_t>();

            *result = *first + *second;
        }

    }
}

