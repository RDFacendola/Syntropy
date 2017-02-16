
/// \file virtual_machine.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>

#include "memory/memory.h"

namespace syntropy
{
    namespace syntax
    {

        class VirtualMachine;

        /// \brief Type alias for a variable representing a "register" of a syntropy virtual machine.
        /// Since the virtual machine doesn't have real register, this is just an offset relative to the current base pointer.
        using register_t = int32_t;

        /// \brief Type alias for a word of a virtual machine.
        using word_t = int64_t;

        struct VMInstruction;

        /// \brief Execution context for a virtual machine. Used to change the status of the virtual machine from within the code being executed.
        /// \author Raffaele D. Facendola - February 2017
        class VMExecutionContext
        {
        public:

            VMExecutionContext(VirtualMachine& virtual_machine);

            // Instrinsics

            /// \brief Causes the current program being executed to terminate.
            /// Calling this method, causes the status of the virtual machine to become invalid.
            void Halt();

            /// \brief Jump to another instruction.
            /// \param offset Offset of the jump with respect to the current instruction pointer, in bytes.
            void Jump(int64_t offset);

            /// \brief Call a function.
            /// This method causes the instruction pointer of the virtual machine to jump to a new address.
            /// \param function_pointer_ Pointer to the function to call.
            void Call(void* function_pointer_);

            /// \brief Setup a frame for a function call.
            /// \param local_storage_size Storage for local variables of the function.
            void Enter(size_t local_storage_size);

            /// \brief Tear down a function frame and return to the caller.
            /// This function deallocate the memory reserved for the input arguments.
            /// \param input_arguments_size Storage for the input arguments.
            void Return(size_t input_arguments_size);

            /// \brief Push a memory block on top of the stack.
            /// \param source Base address of the block to push.
            /// \param size Size of the block to push.
            void Push(void* source, size_t size);

            /// \brief Pop a value from the top of the virtual machine stack.
            /// \param destination Address where the value on top of the stack will be popped to.
            /// \param size Size of the block to pop.
            void Pop(void* destination, size_t size);

            /// \brief Get a pointer to a register.
            /// Negative offsets refers to input variables, while positive offset refers to local variables.
            /// \param Offset of the variable relative to the current base pointer, in bytes.
            template <typename TRegister>
            TRegister* GetRegister(register_t reg);

        private:

            VirtualMachine& virtual_machine_;       ///< \brief Virtual machine this execution context refers to.

        };

        /// \brief A basic virtual machine to run script code.
        /// \author Raffaele D. Facendola - February 2017
        class VirtualMachine
        {

            friend class VMExecutionContext;

        public:

            /// \brief Create a new virtual machine.
            /// \param code_size Size of the memory buffer containing the code, in bytes.
            /// \param stack_size Size of the memory buffer containing the stack, in bytes.
            VirtualMachine(size_t code_size, size_t stack_size);

            /// \brief Virtual destructor.
            virtual ~VirtualMachine();

            /// \brief Execute the next instruction.
            void ExecuteNext();

            /// \brief Check whether the virtual machine is running some code.
            /// \return Returns true if the machine has instructions to execute, returns false otherwise.
            bool IsRunning() const;

        private:

            // Memory

            MemoryBuffer code_buffer_;                          ///< \brief Buffer containing the code.

            MemoryBuffer stack_buffer_;                         ///< \brief Buffer containing the stack.

            // Status

            bool running_;                                      ///< \brief Whether the virtual machine is running some code or not.

            VMExecutionContext execution_context_;              ///< \brief Execution context passed to the instructions.

            // Registers

            VMInstruction* instruction_pointer_;                ///< \brief Pointer to the current instruction to execute.

            uintptr_t* base_pointer_;                           ///< \brief Pointer to the base address of the current function frame.

            uintptr_t* stack_pointer_;                          ///< \brief Pointer to the first free element in the stack.

        };

        /// \brief Base class for instruction that can be executed by a virtual machine
        /// \author Raffaele D. Facendola - February 2017
        struct VMInstruction
        {

            /// \brief Execute the instruction in the given context.
            /// \param context Execution context used to access the status of the virtual machine.
            virtual void Execute(VMExecutionContext& context) const = 0;

            /// \brief Get the size of the instruction, in bytes. 
            /// \return Returns the size of the instruction, in bytes.
            virtual size_t GetSize() const = 0;

        };

    }
}

namespace syntropy
{
    namespace syntax
    {
        /************************************************************************/
        /* VM EXECUTION CONTEXT                                                 */
        /************************************************************************/

        template <typename TRegister>
        TRegister* VMExecutionContext::GetRegister(register_t reg)
        {
            return reinterpret_cast<TRegister*>(Memory::Offset(virtual_machine_.base_pointer_, reg));
        }

    }
}