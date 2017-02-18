#include "vm/virtual_machine.h"

#include <string.h>

#include "vm/intrinsics.h"

namespace syntropy
{
    namespace syntax
    {
        // !!!!TEST!!!!
        template <typename THead, typename... TRest>
        void __VMArgs(void*& buffer, const THead& head, TRest&&... rest)
        {
            *reinterpret_cast<THead*>(buffer) = head;           // Write the next argument

            buffer = Memory::Offset(buffer, sizeof(THead));     // Advance

            __VMArgs(buffer, std::forward<TRest>(rest)...);     // Write the rest
        }

        void __VMArgs(void*& /*buffer*/)
        {
            // Do nothing
        }

        template <typename... TArgs>
        instruction_t* __VM(void*& buffer, instruction_t instruction, TArgs&&... arguments)
        {
            auto r = buffer;

            *reinterpret_cast<instruction_t*>(buffer) = instruction;    // Write the instruction

            buffer = Memory::Offset(buffer, sizeof(instruction));       // Advance

            __VMArgs(buffer, std::forward<TArgs>(arguments)...);                // Write the arguments
            
            return reinterpret_cast<instruction_t*>(r);
        }

        //////////////// VIRTUAL MACHINE ////////////////

        VirtualMachine::VirtualMachine(size_t code_size, size_t stack_size)
            : code_buffer_(new char[code_size], code_size)
            , stack_buffer_(new char[stack_size], stack_size)
            , execution_context_(*this)
            , instruction_pointer_(nullptr)
            , base_pointer_(nullptr)
            , stack_pointer_(nullptr)
        {
            // !!!!TEST!!!!
            
            instruction_pointer_ = reinterpret_cast<instruction_t*>(code_buffer_[0]);
            base_pointer_ = reinterpret_cast<word_t*>(stack_buffer_[0]);
            stack_pointer_ = base_pointer_;

            void* code = instruction_pointer_;

            // F1
            auto f1 = __VM(code, &VirtualMachineIntrinsics::Enter, (storage_t)8);

            __VM(code, &VirtualMachineIntrinsics::MoveImmediate, (register_t)0, (word_t)888);                       // Loc0 = 888
            __VM(code, &VirtualMachineIntrinsics::MoveSrcIndirect, (register_t)0, (register_t)-24);                 // Loc0 = *Arg0

            __VM(code, &VirtualMachineMath::AddInteger, (register_t)0, (register_t)0, (register_t)-32);             // Loc0 = Loc0 + Arg1
            __VM(code, &VirtualMachineMath::AddInteger, (register_t)0, (register_t)0, (register_t)-40);             // Loc0 = Loc0 + Arg2

            __VM(code, &VirtualMachineIntrinsics::MoveDstIndirect, (register_t)-24, (register_t)0);                 // *Arg0 = Loc0

            __VM(code, &VirtualMachineIntrinsics::Return, (storage_t)24);


            // MAIN
            auto main = __VM(code, &VirtualMachineIntrinsics::Enter, (storage_t)24);

            __VM(code, &VirtualMachineIntrinsics::MoveImmediate, (register_t)0, (word_t)47);                        // Loc0 = 47
            __VM(code, &VirtualMachineIntrinsics::MoveImmediate, (register_t)8, (word_t)10);                        // Loc1 = 10
            __VM(code, &VirtualMachineIntrinsics::MoveImmediate, (register_t)16, (word_t)25);                       // Loc2 = 25

            __VM(code, &VirtualMachineIntrinsics::PushWord, (register_t)16);                                        // Push(Loc2)
            __VM(code, &VirtualMachineIntrinsics::PushWord, (register_t)8);                                         // Push(Loc1)
            __VM(code, &VirtualMachineIntrinsics::PushAddress, (register_t)0);                                      // Push(&Loc0) PUSH_ADDRESS(Loc0)

            __VM(code, &VirtualMachineIntrinsics::Call, f1);                                                        // F1(&Loc0, Loc1, Loc2);
            
            __VM(code, &VirtualMachineIntrinsics::Halt);

            instruction_pointer_ = main;

        }

        VirtualMachine::~VirtualMachine()
        {
            delete[] code_buffer_[0];
            delete[] stack_buffer_[0];
        }

        void VirtualMachine::ExecuteNext()
        {
            (*(instruction_pointer_++))(execution_context_);
        }

        bool VirtualMachine::IsRunning() const
        {
            return !!instruction_pointer_;
        }

        //////////////// VM EXECUTION CONTEXT ////////////////

        VMExecutionContext::VMExecutionContext(VirtualMachine& virtual_machine)
            : virtual_machine_(virtual_machine)
        {

        }

        void VMExecutionContext::Halt()
        {
            virtual_machine_.instruction_pointer_ = nullptr;
        }

        void VMExecutionContext::Jump(int64_t offset)
        {
            virtual_machine_.instruction_pointer_ = Memory::Offset(virtual_machine_.instruction_pointer_, offset);
        }

        void VMExecutionContext::Push(void* source, size_t size)
        {
            // Copy the block
            memmove_s(virtual_machine_.stack_pointer_, size, source, size);

            // Move the head of the stack
            virtual_machine_.stack_pointer_ = Memory::Offset(virtual_machine_.stack_pointer_, size);
        }

        void VMExecutionContext::Pop(void* destination, size_t size)
        {
            // Move the head of the stack
            virtual_machine_.stack_pointer_ = Memory::Offset(virtual_machine_.stack_pointer_, -static_cast<int64_t>(size));

            // Copy the block
            memmove_s(destination, size, virtual_machine_.stack_pointer_, size);
        }

        void VMExecutionContext::Call(void* function_pointer_)
        {
            // Save the caller instruction pointer.
            *virtual_machine_.stack_pointer_ = reinterpret_cast<uintptr_t>(virtual_machine_.instruction_pointer_);

            ++virtual_machine_.stack_pointer_;

            // Returns the control to the callee.
            virtual_machine_.instruction_pointer_ = reinterpret_cast<instruction_t*>(function_pointer_);
        }

        void VMExecutionContext::Enter(size_t local_storage_size)
        {
            // Save the caller base pointer.
            *virtual_machine_.stack_pointer_ = reinterpret_cast<word_t>(virtual_machine_.base_pointer_);

            ++virtual_machine_.stack_pointer_;

            // Setup a new base pointer.
            virtual_machine_.base_pointer_ = virtual_machine_.stack_pointer_;

            // Reserve space for the local storage.
            virtual_machine_.stack_pointer_ = Memory::Offset(virtual_machine_.stack_pointer_, local_storage_size);
        }

        void VMExecutionContext::Return(size_t input_arguments_size)
        {
            // Tear down local storage.
            virtual_machine_.stack_pointer_ = virtual_machine_.base_pointer_;

            // Restore the previous base pointer.
            --virtual_machine_.stack_pointer_;

            virtual_machine_.base_pointer_ = reinterpret_cast<word_t*>(*virtual_machine_.stack_pointer_);

            // Restore the previous instruction pointer and returns the control to the caller.
            --virtual_machine_.stack_pointer_;

            virtual_machine_.instruction_pointer_ = reinterpret_cast<instruction_t*>(*virtual_machine_.stack_pointer_);

            // Tear down input arguments storage.
            virtual_machine_.stack_pointer_ = Memory::Offset(virtual_machine_.stack_pointer_, -static_cast<int64_t>(input_arguments_size));
        }

    }
}
