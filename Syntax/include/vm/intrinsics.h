
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

            EnterInstruction(storage_t local_storage_size);

            virtual void Execute(VMExecutionContext& context) const override;

            storage_t local_storage_size_;          ///< \brief Memory to reserve for local variables, in bytes.
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

            ReturnInstruction(storage_t input_storage_size);

            virtual void Execute(VMExecutionContext& context) const override;

            storage_t input_storage_size_;          ///< \brief Memory reserved for input variables, in bytes.
            int32_t padding_;

        };

        // STACK MANAGEMENT

        /// \brief Push of word-sized value on top of the stack.
        /// \author Raffaele D. Facendola - February 2017
        struct PushWordInstruction : VMBaseInstruction<PushWordInstruction>
        {

            PushWordInstruction(register_t source);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t source_;         ///< \brief Contains the value to push on top of the stack.
            int32_t padding_;

        };

        /// \brief Push an address on top of the stack.
        /// \author Raffaele D. Facendola - February 2017
        struct PushAddressInstruction : VMBaseInstruction<PushAddressInstruction>
        {

            PushAddressInstruction(register_t source);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t source_;         ///< \brief Location whose effective address should be pushed on top of the stack.
            int32_t padding_;

        };

        /// \brief Pop a word-sized value from the top of ht estack.
        /// \author Raffaele D. Facendola - February 2017
        struct PopWordInstruction : VMBaseInstruction<PopWordInstruction>
        {

            PopWordInstruction(register_t destination);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;    ///< \brief Destination where the value on top of the stack will be assigned to.
            int32_t padding_;

        };

        // ASSIGNMENT

        /// \brief Move an immediate word-sized value to a destination register.
        /// DST = [value]
        /// \author Raffaele D. Facendola - February 2017
        struct MoveWordImmediateInstruction : VMBaseInstruction<MoveWordImmediateInstruction>
        {

            MoveWordImmediateInstruction(register_t destination, word_t value);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Destination of the assignment.
            int32_t padding_;

            word_t value_;                  ///< \brief Immediate value to assign.

        };
        
        /// \brief Move a word-sized value from a source register to a destination register.
        /// DST = SRC
        /// \author Raffaele D. Facendola - February 2017
        struct MoveWordInstruction : VMBaseInstruction<MoveWordInstruction>
        {

            MoveWordInstruction(register_t destination, register_t source);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Destination of the assignment.
            register_t source_;             ///< \brief Source of the assignment.

        };

        /// \brief Move a word-sized value from a source register to a location pointed by a destination register.
        /// *DST = SRC
        /// \author Raffaele D. Facendola - February 2017
        struct MoveWordDstIndirectInstruction : VMBaseInstruction<MoveWordDstIndirectInstruction>
        {

            MoveWordDstIndirectInstruction(register_t destination, register_t source);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Points to the destination of the assignment.
            register_t source_;             ///< \brief Source of the assignment.

        };

        /// \brief Move a word-sized value pointed by a source register to a destination register.
        /// DST = *SRC
        /// \author Raffaele D. Facendola - February 2017
        struct MoveWordSrcIndirectInstruction : VMBaseInstruction<MoveWordSrcIndirectInstruction>
        {

            MoveWordSrcIndirectInstruction(register_t destination, register_t source);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Destination of the assignment.
            register_t source_;             ///< \brief Points to the source of the assignment.

        };

        /// \brief Move a word-sized value pointed by a source register to a location pointed by a destination register.
        /// *DST = *SRC
        /// \author Raffaele D. Facendola - February 2017
        struct MoveWordSrcDstIndirectInstruction : VMBaseInstruction<MoveWordSrcDstIndirectInstruction>
        {

            MoveWordSrcDstIndirectInstruction(register_t destination, register_t source);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Points to the destination of the assignment.
            register_t source_;             ///< \brief Points to the source of the assignment.

        };

        /// \brief Move the effective address of a source register to a destination register.
        /// DST = &SRC
        /// \author Raffaele D. Facendola - February 2017
        struct MoveAddressInstruction : VMBaseInstruction<MoveAddressInstruction>
        {

            MoveAddressInstruction(register_t destination, register_t source);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Destination of the assignment.
            register_t source_;             ///< \brief Register whose effective address should be moved.

        };

        // MoveImmediate        --> Copy immediate value

        /// \brief Move a value from a register to another register.
        /// \example i(x, y)       x = y
        /// \author Raffaele D. Facendola - February 2017
        struct MoveInstruction : VMBaseInstruction<MoveWordInstruction>
        {

            MoveInstruction(register_t destination, register_t source, storage_t size);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Destination of the assignment.
            register_t source_;             ///< \brief Source of the assignment.

            storage_t size_;                ///< \brief Size of the value to move, in bytes.
            int32_t padding_;

        };

        /// \brief Move a value from a register to the location pointed by another register.
        /// \example i(x, y)       *x = y
        /// \author Raffaele D. Facendola - February 2017
        struct MoveIndirectInstruction : VMBaseInstruction<MoveIndirectInstruction>
        {

            MoveIndirectInstruction(register_t destination, register_t source, storage_t size);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t destination_;        ///< \brief Points to the destination of the assignment.
            register_t source_;             ///< \brief Source of the assignment.

            storage_t size_;                ///< \brief Size of the value to move, in bytes.
            int32_t padding_;

        };

        // Math

        /// \brief Perform the sum of two integer numbers.
        /// \example i(r, f, s)     r = f + s
        /// \author Raffaele D. Facendola - February 2017
        struct AddIntegerInstruction : VMBaseInstruction<AddIntegerInstruction>
        {

            AddIntegerInstruction(register_t result, register_t first, register_t second);

            virtual void Execute(VMExecutionContext& context) const override;

            register_t result_;         ///< \brief Contains the sum of the two other arguments.
            int32_t padding_;

            register_t first_;          ///< \brief First element to add.
            register_t second_;         ///< \brief Second element to add.

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
