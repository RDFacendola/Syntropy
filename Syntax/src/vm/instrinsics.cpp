#include "vm/intrinsics.h"

#include <string.h>

namespace syntropy
{
    namespace syntax
    {

        // INSTRINSICS

        /************************************************************************/
        /* NOP INSTRUCTION                                                      */
        /************************************************************************/

        void NopInstruction::Execute(VMExecutionContext& /*context*/) const
        {
            // Do nothing
        }

        /************************************************************************/
        /* HALT INSTRUCTION                                                     */
        /************************************************************************/

        void HaltInstruction::Execute(VMExecutionContext& context) const
        {
            context.Halt();
        }

        /************************************************************************/
        /* JUMP INSTRUCTION                                                     */
        /************************************************************************/

        JumpInstruction::JumpInstruction(int64_t offset)
            : offset_(offset)
        {

        }

        void JumpInstruction::Execute(VMExecutionContext& context) const
        {
            context.Jump(offset_);
        }

        /************************************************************************/
        /* ENTER INSTRUCTION                                                    */
        /************************************************************************/

        EnterInstruction::EnterInstruction(storage_t local_storage_size)
            : local_storage_size_(local_storage_size)
        {

        }

        void EnterInstruction::Execute(VMExecutionContext& context) const
        {
            context.Enter(local_storage_size_);
        }

        /************************************************************************/
        /* CALL INSTRUCTION                                                     */
        /************************************************************************/

        CallInstruction::CallInstruction(void* function)
            : function_(function)
        {

        }

        void CallInstruction::Execute(VMExecutionContext& context) const
        {
            context.Call(function_);
        }

        /************************************************************************/
        /* RETURN INSTRUCTION                                                   */
        /************************************************************************/

        ReturnInstruction::ReturnInstruction(storage_t input_storage_size)
            : input_storage_size_(input_storage_size)
        {

        }

        void ReturnInstruction::Execute(VMExecutionContext& context) const
        {
            context.Return(input_storage_size_);
        }

        //////////////// PUSH DIRECT INSTRUCTION ////////////////

        PushDirectInstruction::PushDirectInstruction(int32_t offset, uint32_t size)
            : offset_(offset)
            , size_(size)
        {

        }

        void PushDirectInstruction::Execute(VMExecutionContext& context) const
        {
            context.Push(context.GetRegister<void>(offset_), size_);
        }

        //////////////// POP DIRECT INSTRUCTION ////////////////

        PopDirectInstruction::PopDirectInstruction(int32_t offset, uint32_t size)
            : offset_(offset)
            , size_(size)
        {

        }

        void PopDirectInstruction::Execute(VMExecutionContext& context) const
        {
            context.Pop(context.GetRegister<void>(offset_), size_);
        }

        // MOVE

        /************************************************************************/
        /* MOVE WORD IMMEDIATE INSTRUCTION                                      */
        /************************************************************************/

        MoveWordImmediateInstruction::MoveWordImmediateInstruction(register_t destination, word_t value)
            : destination_(destination)
            , value_(value)
        {

        }

        void MoveWordImmediateInstruction::Execute(VMExecutionContext& context) const
        {
            auto destination = context.GetRegister<word_t>(destination_);

            *destination = value_;
        }

        /************************************************************************/
        /* MOVE WORD INSTRUCTION                                                */
        /************************************************************************/

        MoveWordInstruction::MoveWordInstruction(register_t destination, register_t source)
            : destination_(destination)
            , source_(source)
        {

        }

        void MoveWordInstruction::Execute(VMExecutionContext& context) const
        {
            auto source = context.GetRegister<word_t>(source_);
            auto destination = context.GetRegister<word_t>(destination_);

            *destination = *source;
        }

        /************************************************************************/
        /* MOVE WORD INDIRECT INSTRUCTION                                       */
        /************************************************************************/

        MoveWordIndirectInstruction::MoveWordIndirectInstruction(register_t destination, register_t source)
            : destination_(destination)
            , source_(source)
        {

        }

        void MoveWordIndirectInstruction::Execute(VMExecutionContext& context) const
        {
            auto source = context.GetRegister<word_t>(source_);
            auto destination = context.GetRegister<word_t*>(destination_);

            **destination = *source;
        }

        /************************************************************************/
        /* MOVE ADDRESS INSTRUCTION                                             */
        /************************************************************************/

        MoveAddressInstruction::MoveAddressInstruction(register_t destination, register_t source)
            : destination_(destination)
            , source_(source)
        {

        }

        void MoveAddressInstruction::Execute(VMExecutionContext& context) const
        {
            auto source = context.GetRegister<word_t>(source_);
            auto destination = context.GetRegister<word_t>(destination_);

            *destination = reinterpret_cast<word_t>(source);
        }

        /************************************************************************/
        /* MOVE INSTRUCTION                                                     */
        /************************************************************************/

        MoveInstruction::MoveInstruction(register_t destination, register_t source, storage_t size)
            : destination_(destination)
            , source_(source)
            , size_(size)
        {

        }

        void MoveInstruction::Execute(VMExecutionContext& context) const
        {
            auto source = context.GetRegister<void>(source_);
            auto destination = context.GetRegister<void>(destination_);

            memmove_s(destination, size_, source, size_);
        }

        /************************************************************************/
        /* MOVE INDIRECT INSTRUCTION                                            */
        /************************************************************************/

        MoveIndirectInstruction::MoveIndirectInstruction(register_t destination, register_t source, storage_t size)
            : destination_(destination)
            , source_(source)
            , size_(size)
        {

        }

        void MoveIndirectInstruction::Execute(VMExecutionContext& context) const
        {
            auto source = context.GetRegister<void>(source_);
            auto destination = context.GetRegister<void*>(destination_);

            memmove_s(*destination, size_, source, size_);
        }

        // MATH

        /************************************************************************/
        /* ADD INTEGER INSTRUCTION                                              */
        /************************************************************************/

        AddIntegerInstruction::AddIntegerInstruction(register_t result, register_t first, register_t second)
            : result_(result)
            , first_(first)
            , second_(second)
        {

        }

        void AddIntegerInstruction::Execute(VMExecutionContext& context) const
        {
            auto result = context.GetRegister<word_t>(result_);
            auto first = context.GetRegister<word_t>(first_);
            auto second = context.GetRegister<word_t>(second_);

            *result = *first + *second;
        }

    }
}

