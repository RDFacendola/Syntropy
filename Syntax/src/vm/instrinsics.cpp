#include "vm/intrinsics.h"

#include <string.h>

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

            vm.instruction_pointer_ = nullptr;                                                  // nullptr means that the VM has no other instructions to execute.
        }

        void VirtualMachineIntrinsics::Jump(VMExecutionContext& context)
        {
            auto offset = context.GetNextArgument<word_t>();

            auto& vm = context.GetVirtualMachine();

            vm.instruction_pointer_ = Memory::Offset(vm.instruction_pointer_, offset);          // Jump forward or backward depending on the provided offset.
        }

        void VirtualMachineIntrinsics::Enter(VMExecutionContext& context)
        {
            auto local_storage = context.GetNextArgument<storage_t>();

            auto& vm = context.GetVirtualMachine();

            *(vm.stack_pointer_++) = reinterpret_cast<word_t>(vm.base_pointer_);                // Save the caller's base pointer.

            vm.base_pointer_ = vm.stack_pointer_;                                               // Setup a new base pointer for the current frame.
            
            vm.stack_pointer_ = Memory::Offset(vm.stack_pointer_, local_storage);               // Reserve space for local storage.
        }

        void VirtualMachineIntrinsics::Call(VMExecutionContext& context)
        {
            auto function_pointer = context.GetNextArgument<void*>();

            auto& vm = context.GetVirtualMachine();

            *(vm.stack_pointer_++) = reinterpret_cast<uintptr_t>(vm.instruction_pointer_);      // Save the caller's instruction pointer. This actually points to the next instruction after "Call".

            vm.instruction_pointer_ = reinterpret_cast<instruction_t*>(function_pointer);       // Grant control to the callee.
        }

        void VirtualMachineIntrinsics::Return(VMExecutionContext& context)
        {
            auto input_storage = context.GetNextArgument<storage_t>();

            auto& vm = context.GetVirtualMachine();

            vm.stack_pointer_ = vm.base_pointer_;                                                           // Tear down the local storage by unwinding the stack pointer.

            vm.base_pointer_ = reinterpret_cast<word_t*>(*(--vm.stack_pointer_));                           // Restore the previous base pointer.

            vm.instruction_pointer_ = reinterpret_cast<instruction_t*>(*(--vm.stack_pointer_));             // Restore the previous instruction pointer and return the control to the caller.

            vm.stack_pointer_ = Memory::Offset(vm.stack_pointer_, -static_cast<int64_t>(input_storage));    // Tear down input arguments storage.
        }

        void VirtualMachineIntrinsics::PushWord(VMExecutionContext& context)
        {
            auto source_register = context.GetNextArgument<register_t>();
            
            auto source = context.GetRegister<word_t>(source_register);

            auto& vm = context.GetVirtualMachine();

            *(vm.stack_pointer_++) = *source;                               // Push a word on the stack.
        }

        void VirtualMachineIntrinsics::PushAddress(VMExecutionContext& context)
        {
            auto source_register = context.GetNextArgument<register_t>();

            auto source = context.GetRegister<word_t>(source_register);

            auto& vm = context.GetVirtualMachine();

            *(vm.stack_pointer_++) = reinterpret_cast<word_t>(source);      // Push an address on the stack (as word).
        }

        void VirtualMachineIntrinsics::PopWord(VMExecutionContext& context)
        {
            auto destination_register = context.GetNextArgument<register_t>();

            auto destination = context.GetRegister<word_t>(destination_register);

            auto& vm = context.GetVirtualMachine();

            *destination = *(--vm.stack_pointer_);                          // Pop a word from the stack.
        }

        void VirtualMachineIntrinsics::MoveImmediate(VMExecutionContext& context)
        {
            auto destination_register = context.GetNextArgument<register_t>();
            auto value = context.GetNextArgument<word_t>();
            
            auto destination = context.GetRegister<word_t>(destination_register);

            *destination = value;
        }

        void VirtualMachineIntrinsics::Move(VMExecutionContext& context)
        {
            auto destination_register = context.GetNextArgument<register_t>();
            auto source_register = context.GetNextArgument<register_t>();

            auto destination = context.GetRegister<word_t>(destination_register);
            auto source = context.GetRegister<word_t>(source_register);

            *destination = *source;
        }

        void VirtualMachineIntrinsics::MoveDstIndirect(VMExecutionContext& context)
        {
            auto destination_register = context.GetNextArgument<register_t>();
            auto source_register = context.GetNextArgument<register_t>();

            auto destination = context.GetRegister<word_t*>(destination_register);
            auto source = context.GetRegister<word_t>(source_register);

            **destination = *source;
        }

        void VirtualMachineIntrinsics::MoveSrcIndirect(VMExecutionContext& context)
        {
            auto destination_register = context.GetNextArgument<register_t>();
            auto source_register = context.GetNextArgument<register_t>();

            auto destination = context.GetRegister<word_t>(destination_register);
            auto source = context.GetRegister<word_t*>(source_register);

            *destination = **source;
        }

        void VirtualMachineIntrinsics::MoveSrcDstIndirect(VMExecutionContext& context)
        {
            auto destination_register = context.GetNextArgument<register_t>();
            auto source_register = context.GetNextArgument<register_t>();

            auto destination = context.GetRegister<word_t*>(destination_register);
            auto source = context.GetRegister<word_t*>(source_register);

            **destination = **source;
        }

        void VirtualMachineIntrinsics::MoveAddress(VMExecutionContext& context)
        {
            auto destination_register = context.GetNextArgument<register_t>();
            auto source_register = context.GetNextArgument<register_t>();

            auto destination = context.GetRegister<word_t>(destination_register);
            auto source = context.GetRegister<word_t>(source_register);

            *destination = reinterpret_cast<word_t>(source);
        }

        /************************************************************************/
        /* VIRTUAL MACHINE MATH                                                 */
        /************************************************************************/

        void VirtualMachineMath::AddInteger(VMExecutionContext& context)
        {
            auto result_register = context.GetNextArgument<register_t>();
            auto first_register = context.GetNextArgument<register_t>();
            auto second_register = context.GetNextArgument<register_t>();

            auto result = context.GetRegister<word_t>(result_register);
            auto first = context.GetRegister<word_t>(first_register);
            auto second = context.GetRegister<word_t>(second_register);

            *result = *first + *second;
        }

    }
}

