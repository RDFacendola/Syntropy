
/// \file virtual_machine.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstdint>
#include "syntropy/containers/map.h"

#include "syntropy/memory/memory_buffer.h"

#include "syntropy/types/label.h"

namespace syntropy
{
    namespace syntax
    {

        class VirtualMachine;
        class VMExecutionContext;

        /// \brief Type alias for a variable representing a "register" of a syntropy virtual machine.
        /// Since the virtual machine doesn't have real registers, this is just an offset relative to the current base pointer.
        /// Equivalent of intptr_t.
        using register_t = int32_t;

        /// \brief Type alias for a word of a virtual machine.
        /// Must fit a pointer for the current architecture. (ie: int32_t is not valid under x64 architecture)
        /// Equivalent of int.
        using word_t = int64_t;

        /// \brief Type alias for a size type for a virtual machine.
        /// Equivalent of size_t.
        using storage_t = uint32_t;

        /// \brief Type alias for instructions that can be executed by a virtual machine.
        using instruction_t = void(*)(VMExecutionContext&);

        /// \brief Execution context for a virtual machine. Used to change the status of the virtual machine from within the code being executed.
        /// \author Raffaele D. Facendola - February 2017
        class VMExecutionContext
        {
        public:

            VMExecutionContext(VirtualMachine& virtual_machine);

            /// \brief Get the virtual machine this execution context refers to.
            /// \return Returns the virtual machine this execution context refers to.
            VirtualMachine& GetVirtualMachine();

            /// \brief Get the next immediate value for the current instruction and advances the instruction pointer.
            /// \return Returns the next argument for the current instruction.
            template <typename TArgument>
            const TArgument& GetNextImmediate();

            /// \brief Interpret the next argument for the current instruction as a function argument and return a pointer to its value.
            /// \return Returns the address of the register stored as a next argument for the current instruction.
            template <typename TArgument>
            TArgument* GetNextArgument();

        private:

            VirtualMachine& virtual_machine_;       ///< \brief Virtual machine this execution context refers to.

        };

        /// \brief A basic virtual machine to run script code.
        /// \author Raffaele D. Facendola - February 2017
        class VirtualMachine
        {

            friend class VMExecutionContext;
            friend class VirtualMachineIntrinsics;

        public:

            /// \brief Create a new virtual machine.
            /// \param stack_size Size of the memory buffer containing the stack, in bytes.
            VirtualMachine(Bytes stack_size);

            /// \brief No copy constructor.
            VirtualMachine(const VirtualMachine&) = delete;

            /// \brief Virtual destructor.
            virtual ~VirtualMachine();

            /// \brief No assignment operator.
            VirtualMachine& operator=(const VirtualMachine&) = delete;

            /// \brief Execute the next instruction.
            void ExecuteNext();

            /// \brief Check whether the virtual machine is running some code.
            /// \return Returns true if the machine has instructions to execute, returns false otherwise.
            bool IsRunning() const;

        protected:

            /// \brief Get the address of a function by name.
            /// \param function_name Name of the function.
            /// \return Returns a pointer to the requested function. Returns nullptr if no such function could be found.
            void* GetFunctionAddress(const Label& function_name);

            /// \brief Register an address for a symbolic function name.
            /// \param function_name Symbolic function name.
            /// \param function_address Actual address of the function.
            void RegisterFunctionAddress(const Label& function_name, void* function_address);

        private:

            // Memory

            MemoryBuffer stack_segment_;                        ///< \brief Buffer containing the stack segment.

            // Status

            VMExecutionContext execution_context_;              ///< \brief Execution context passed to the instructions.

            Map<Label, void*> function_address_table_;        ///< \brief Maps each function name with its actual pointer in memory.

            // Registers

            instruction_t* instruction_pointer_;                ///< \brief Pointer to the current instruction to execute.

            word_t* base_pointer_;                              ///< \brief Pointer to the base address of the current function frame.

            word_t* stack_pointer_;                             ///< \brief Pointer to the first free element in the stack.

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

        inline VirtualMachine& VMExecutionContext::GetVirtualMachine()
        {
            return virtual_machine_;
        }

        template <typename TArgument>
        inline const TArgument& VMExecutionContext::GetNextImmediate()
        {
            TArgument* argument = reinterpret_cast<TArgument*>(virtual_machine_.instruction_pointer_);

            virtual_machine_.instruction_pointer_ = reinterpret_cast<instruction_t*>(argument + 1);             // Advances the instruction pointer to the next instruction/argument

            return *argument;
        }

        template <typename TArgument>
        inline TArgument* VMExecutionContext::GetNextArgument()
        {
            auto register_offset = GetNextImmediate<register_t>();                                              // Offset of the register, relative to the current base pointer.

            return (MemoryAddress(virtual_machine_.base_pointer_) + Bytes(register_offset)).As<TArgument>();
        }

    }
}