#include "vm/virtual_machine.h"

#include <string.h>

#include "vm/intrinsics.h"

namespace syntropy
{
    namespace syntax
    {
        // !!!!TEST!!!!
        template <typename TInstruction, typename... TArgs>
        void* VMPushInstruction(void*& buffer, TArgs&&... arguments)
        {
            auto r = buffer;

            new (buffer) TInstruction(std::forward<TArgs>(arguments)...);

            buffer = Memory::Offset(buffer, sizeof(TInstruction));

            return r;
        }

        //////////////// VIRTUAL MACHINE ////////////////

        VirtualMachine::VirtualMachine(size_t code_size, size_t stack_size)
            : code_buffer_(new char[code_size], code_size)
            , stack_buffer_(new char[stack_size], stack_size)
            , running_(false)
            , execution_context_(*this)
            , instruction_pointer_(nullptr)
            , base_pointer_(nullptr)
            , stack_pointer_(nullptr)
        {
            // !!!!TEST!!!!
            running_ = true;
            
            instruction_pointer_ = reinterpret_cast<VMInstruction*>(code_buffer_[0]);
            base_pointer_ = reinterpret_cast<uint64_t*>(stack_buffer_[0]);
            stack_pointer_ = base_pointer_;

            void* code = instruction_pointer_;

            // F1
            auto f1 = VMPushInstruction<EnterInstruction>(code, 8);

            VMPushInstruction<MoveWordImmediateInstruction>(code, 0, 888);      // Loc0 = 888
            VMPushInstruction<AddIntegerInstruction>(code, 0, -32, -40);        // Loc0 = Arg1 + Arg2
            VMPushInstruction<MoveWordIndirectInstruction>(code, -48, 0);       // *Arg0 = Loc0

            VMPushInstruction<ReturnInstruction>(code, 24);


            // MAIN
            auto main = VMPushInstruction<EnterInstruction>(code, 24);

            VMPushInstruction<MoveWordImmediateInstruction>(code, 0, 0);        // Loc0 = 0
            VMPushInstruction<MoveWordImmediateInstruction>(code, 8, 10);       // Loc1 = 10
            VMPushInstruction<MoveWordImmediateInstruction>(code, 16, 25);      // Loc2 = 25

            VMPushInstruction<PushDirectInstruction>(code, 16, 8);              // Push(Loc2)
            VMPushInstruction<PushDirectInstruction>(code, 8, 8);               // Push(Loc1)
            VMPushInstruction<PushDirectInstruction>(code, 0, 8);               // Push(&Loc0) PUSH_ADDRESS(Loc0)

            VMPushInstruction<CallInstruction>(code, f1);                       // F1(&Loc0, Loc1, Loc2);
            
            VMPushInstruction<HaltInstruction>(code);

            instruction_pointer_ = reinterpret_cast<VMInstruction*>(main);

        }

        VirtualMachine::~VirtualMachine()
        {
            delete[] code_buffer_[0];
            delete[] stack_buffer_[0];
        }

        void VirtualMachine::ExecuteNext()
        {
            // Since executing the current instruction may change the value of the instruction pointer (jump or call)
            // the instruction pointer is incremented *before* executing the instruction, so it can override the result without side effects.
            auto current_instruction = instruction_pointer_;

            instruction_pointer_ = Memory::Offset(current_instruction, current_instruction->GetSize());

            // Execute the current instruction
            current_instruction->Execute(execution_context_);
        }

        bool VirtualMachine::IsRunning() const
        {
            return running_;
        }

        //////////////// VM EXECUTION CONTEXT ////////////////

        VMExecutionContext::VMExecutionContext(VirtualMachine& virtual_machine)
            : virtual_machine_(virtual_machine)
        {

        }

        void VMExecutionContext::Halt()
        {
            virtual_machine_.running_ = false;
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
            virtual_machine_.instruction_pointer_ = reinterpret_cast<VMInstruction*>(function_pointer_);
        }

        void VMExecutionContext::Enter(size_t local_storage_size)
        {
            // Save the caller base pointer.
            *virtual_machine_.stack_pointer_ = reinterpret_cast<uintptr_t>(virtual_machine_.base_pointer_);

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

            virtual_machine_.base_pointer_ = reinterpret_cast<uintptr_t*>(*virtual_machine_.stack_pointer_);

            // Restore the previous instruction pointer and returns the control to the caller.
            --virtual_machine_.stack_pointer_;

            virtual_machine_.instruction_pointer_ = reinterpret_cast<VMInstruction*>(*virtual_machine_.stack_pointer_);

            // Tear down input arguments storage.
            virtual_machine_.stack_pointer_ = Memory::Offset(virtual_machine_.stack_pointer_, -static_cast<int64_t>(input_arguments_size));
        }

    }
}
