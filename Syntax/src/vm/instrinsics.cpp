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
            context.Halt();
        }

        void VirtualMachineIntrinsics::Jump(VMExecutionContext& context)
        {
            auto offset = context.GetNextArgument<word_t>();

            context.Jump(offset);
        }

        void VirtualMachineIntrinsics::Enter(VMExecutionContext& context)
        {
            auto local_storage = context.GetNextArgument<storage_t>();

            context.Enter(local_storage);
        }

        void VirtualMachineIntrinsics::Call(VMExecutionContext& context)
        {
            auto function = context.GetNextArgument<void*>();

            context.Call(function);
        }

        void VirtualMachineIntrinsics::Return(VMExecutionContext& context)
        {
            auto input_storage = context.GetNextArgument<storage_t>();

            context.Return(input_storage);
        }

        void VirtualMachineIntrinsics::PushWord(VMExecutionContext& context)
        {
            auto source_register = context.GetNextArgument<register_t>();
            
            auto source = context.GetRegister<word_t>(source_register);

            context.Push(source, sizeof(word_t));
        }

        void VirtualMachineIntrinsics::PushAddress(VMExecutionContext& context)
        {
            auto source_register = context.GetNextArgument<register_t>();

            auto source = context.GetRegister<word_t>(source_register);

            context.Push(&source, sizeof(source));
        }

        void VirtualMachineIntrinsics::PopWord(VMExecutionContext& context)
        {
            auto destination_register = context.GetNextArgument<register_t>();

            auto destination = context.GetRegister<word_t>(destination_register);

            context.Pop(destination, sizeof(word_t));
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

