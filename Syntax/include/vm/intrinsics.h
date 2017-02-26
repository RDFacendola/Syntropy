
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

        /// \brief Static class containing virtual machine's intrinsic instructions.
        /// Intrinsic functions are meant to provide primitive functionalities (such as calling a function, jumping, moving memory around and so on).
        /// \author Raffaele D. Facendola - February 2017
        class VirtualMachineIntrinsics
        {

        public:

            // Basics

            /// \brief Do nothing.
            /// Nop()
            static void Nop(VMExecutionContext& context);

            /// \brief Terminate the execution of the virtual machine.
            /// Halt()
            static void Halt(VMExecutionContext& context);

            // Flow control

            /// \brief Jump to another instruction.
            /// Jump(word_t offset)
            static void Jump(VMExecutionContext& context);

            // Function call

            /// \brief Setup a frame for a new function.
            /// Enter(storage_t local_storage_in_bytes)
            static void Enter(VMExecutionContext& context);

            /// \brief Jump to another function by name.
            /// Call(word_t function_name)
            static void Call(VMExecutionContext& context);

            /// \brief Tear down the current frame, local storage and input arguments storage and return to the caller.
            /// Return(storage_t input_storage_in_bytes)
            static void Return(VMExecutionContext& context);

            // Stack management

            /// \brief Push a word-sized value stored in a register on top of the stack.
            /// PushWord(register_t register)
            static void PushWord(VMExecutionContext& context);

            /// \brief Push the address of a register on top of the stack.
            /// PushAddress(register_t register)
            static void PushAddress(VMExecutionContext& context);

            /// \brief Pop a word-sized value from the stack and store it inside a register.
            /// PopWord(register_t register)
            static void PopWord(VMExecutionContext& context);

            // Assignment

            /// \brief Move a word-sized immediate value to a register.
            /// MoveImmediate(register_t register, word_t value)                // destination = value
            static void MoveImmediate(VMExecutionContext& context);

            /// \brief Move the content of a register to another register.
            /// Move(register_t destination, register_t source)                 // destination = source
            static void Move(VMExecutionContext& context);

            /// \brief Move a word-sized register value to a location pointer by another register.
            /// MoveDstIndirect(register_t destination, register_t source)      // *destination = source
            static void MoveDstIndirect(VMExecutionContext& context);

            /// \brief Move a word-sized value pointed by a register to another register.
            /// MoveSrcIndirect(register_t destination, register_t source)      // destination = *source
            static void MoveSrcIndirect(VMExecutionContext& context);

            /// \brief Move a word-sized value pointed by a register to a location pointer by another register.
            /// MoveSrcDstIndirect(register_t destination, register_t source)   // *destination = *source
            static void MoveSrcDstIndirect(VMExecutionContext& context);

            /// \brief Move the address of a register to another register.
            /// MoveAddress(register_t destination, register_t source)          // destination = &source
            static void MoveAddress(VMExecutionContext& context);

        };

        /// \brief Static class containing virtual machine's math instructions.
        class VirtualMachineMath
        {
        public:

            /// \brief Add two word-sized integers and store the result in a third integer.
            /// AddInteger(register_t result, register_t first, register_t second)
            static void AddInteger(VMExecutionContext& context);

        };


    }

}