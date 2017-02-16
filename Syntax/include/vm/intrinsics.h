
/// \file virtual_machine.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "virtual_machine.h"

namespace syntropy
{
    namespace syntax
    {

        /// \brief Utility class used as base for instruction that can be executed by a virtual machine.
        /// \tparam TInstruction Concrete instruction type.
        /// \author Raffaele D. Facendola - February 2017
        template <typename TInstruction>
        struct VMBaseInstruction : VMInstruction
        {

            virtual size_t GetSize() const override;

        };

        // INTRINSICS

        /// \brief Causes the virtual machine to do nothing.
        /// \author Raffaele D. Facendola - February 2017
        struct NopInstruction : VMBaseInstruction<NopInstruction>
        {

            virtual void Execute(VMExecutionContext& context) const override;

        };

        /// \brief Causes the virtual machine to stop.
        /// \author Raffaele D. Facendola - February 2017
        struct HaltInstruction : VMBaseInstruction<HaltInstruction>
        {

            virtual void Execute(VMExecutionContext& context) const override;

        };

        /// \brief Causes the virtual machine to jump to another instruction.
        /// \author Raffaele D. Facendola - February 2017
        struct JumpInstruction : VMBaseInstruction<JumpInstruction>
        {

            JumpInstruction(int64_t offset);

            virtual void Execute(VMExecutionContext& context) const override;

            int64_t offset_;            ///< \brief Offset to add to the current instruction pointer to perform the jump, in bytes.

        };

        /// \brief Setup a frame for a function call.
        /// \author Raffaele D. Facendola - February 2017
        struct EnterInstruction : VMBaseInstruction<EnterInstruction>
        {

            EnterInstruction(int32_t local_storage_size);

            virtual void Execute(VMExecutionContext& context) const override;

            int32_t local_storage_size_;            ///< \brief Memory to reserve for local variables, in bytes.
            int32_t padding_;

        };

        /// \brief Causes the virtual machine to call a function.
        /// \author Raffaele D. Facendola - February 2017
        struct CallInstruction : VMBaseInstruction<CallInstruction>
        {

            CallInstruction(void* function);

            virtual void Execute(VMExecutionContext& context) const override;

            void* function_;        ///< \brief Pointer to the function to call.

        };

        /// \brief Tear down a function frame along with its local storage and input storage and return to the caller.
        /// \author Raffaele D. Facendola - February 2017
        struct ReturnInstruction : VMBaseInstruction<ReturnInstruction>
        {

            ReturnInstruction(int32_t input_storage_size);

            virtual void Execute(VMExecutionContext& context) const override;

            int32_t input_storage_size_;            ///< \brief Memory reserved for input variables, in bytes.
            int32_t padding_;

        };

        // STACK MANAGEMENT

        /// \brief Push a local variable value on top of the stack.
        /// \author Raffaele D. Facendola - February 2017
        struct PushDirectInstruction : VMBaseInstruction<PushDirectInstruction>
        {

            PushDirectInstruction(int32_t offset, uint32_t size);

            virtual void Execute(VMExecutionContext& context) const override;

            int32_t offset_;            ///< \brief Offset of the memory to push on top of the stack, relative to the base pointer.
            uint32_t size_;             ///< \brief Size of the memory block to push on top of the stack.

        };

        /// \brief Pop a value from the top of the stack and store the result in a local variable.
        /// \author Raffaele D. Facendola - February 2017
        struct PopDirectInstruction : VMBaseInstruction<PopDirectInstruction>
        {

            PopDirectInstruction(int32_t offset, uint32_t size);

            virtual void Execute(VMExecutionContext& context) const override;

            int32_t offset_;            ///< \brief Offset of the memory where the popped value will be stored at
            uint32_t size_;             ///< \brief Size of the memory block to pop.

        };

        // MOVE

        // MoveImmediate        --> Copy immediate value
        // Move                 --> Move the content of a location to another location
        // MoveIndirect         --> Move the content of a location to the location pointed by another location

        /// \brief Move an immediate word-sized value to a given location.
        /// \author Raffaele D. Facendola - February 2017
        struct MoveWordImmediateInstruction : VMBaseInstruction<MoveWordImmediateInstruction>
        {

            MoveWordImmediateInstruction(register_t destination, word_t value);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Destination of the assignment.
            int32_t padding_;

            word_t value_;                  ///< \brief Immediate value to assign.

        };
        
        /// \brief Move a word-sized value from a register to another register.
        /// \author Raffaele D. Facendola - February 2017
        struct MoveWordInstruction : VMBaseInstruction<MoveWordInstruction>
        {

            MoveWordInstruction(register_t destination, register_t source);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Destination of the assignment.
            register_t source_;             ///< \brief Source of the assignment.

        };

        /// \brief Move a word-sized value from a register to the location pointed by another register.
        /// \author Raffaele D. Facendola - February 2017
        struct MoveWordIndirectInstruction : VMBaseInstruction<MoveWordIndirectInstruction>
        {

            MoveWordIndirectInstruction(register_t destination, register_t source);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Points to the destination of the assignment.
            register_t source_;             ///< \brief Source of the assignment.

        };

        /// \brief Move the effective address of a register to another register.
        /// \author Raffaele D. Facendola - February 2017
        struct MoveAddressInstruction : VMBaseInstruction<MoveWordIndirectInstruction>
        {

            MoveAddressInstruction(register_t destination, register_t source);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Destination of the assignment.
            register_t source_;             ///< \brief Register whose effective address should be moved.

        };


        // Math

        /// \brief Perform the integer addition of two values.
        /// \author Raffaele D. Facendola - February 2017
        struct AddInt64Instruction : VMBaseInstruction<AddInt64Instruction>
        {

            AddInt64Instruction(int32_t result, int32_t op1, int32_t op2);

            virtual void Execute(VMExecutionContext& context) const override;

            int32_t result_;            ///< \brief Offset of the variable containing the result of the sum
            int32_t op1_;               ///< \brief First element to add.

            int32_t op2_;               ///< \brief Second element to add.
            int32_t padding_;           ///< \brief Not used.

        };

    }

}

namespace syntropy
{
    namespace syntax
    {

        template <typename TInstruction>
        size_t VMBaseInstruction<TInstruction>::GetSize() const 
        {
            return sizeof(TInstruction);
        }

    }
}
