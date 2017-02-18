
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
        class VirtualMachineIntrinsics
        {
        public:

            // Basics

            static void Nop(VMExecutionContext& context);

            static void Halt(VMExecutionContext& context);

            // Flow control

            static void Jump(VMExecutionContext& context);

            // Function call

            static void Enter(VMExecutionContext& context);

            static void Call(VMExecutionContext& context);

            static void Return(VMExecutionContext& context);

            // Stack management

            static void PushWord(VMExecutionContext& context);

            static void PushAddress(VMExecutionContext& context);

            static void PopWord(VMExecutionContext& context);

            // Assignment

            static void MoveImmediate(VMExecutionContext& context);

            // D = S
            static void Move(VMExecutionContext& context);

            // *D = S
            static void MoveDstIndirect(VMExecutionContext& context);

            // D = *S
            static void MoveSrcIndirect(VMExecutionContext& context);

            // *D = *S
            static void MoveSrcDstIndirect(VMExecutionContext& context);

            // D = &S
            static void MoveAddress(VMExecutionContext& context);

        };

        /// \brief Static class containing virtual machine's math instructions.
        class VirtualMachineMath
        {
        public:

            static void AddInteger(VMExecutionContext& context);

        };


    }

}