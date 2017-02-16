#include "vm/intrinsics.h"

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

        EnterInstruction::EnterInstruction(int32_t local_storage_size)
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

        ReturnInstruction::ReturnInstruction(int32_t input_storage_size)
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
            context.Push(context.Get<void>(offset_), size_);
        }

        //////////////// POP DIRECT INSTRUCTION ////////////////

        PopDirectInstruction::PopDirectInstruction(int32_t offset, uint32_t size)
            : offset_(offset)
            , size_(size)
        {

        }

        void PopDirectInstruction::Execute(VMExecutionContext& context) const
        {
            context.Pop(context.Get<void>(offset_), size_);
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

        // MATH

        //////////////// ADD INT64 INSTRUCTION ////////////////

        AddInt64Instruction::AddInt64Instruction(int32_t result, int32_t op1, int32_t op2)
            : result_(result)
            , op1_(op1)
            , op2_(op2)
        {

        }

        void AddInt64Instruction::Execute(VMExecutionContext& context) const
        {
            auto result = context.Get<int64_t>(result_);
            auto op1 = context.Get<int64_t>(op1_);
            auto op2 = context.Get<int64_t>(op2_);

            *result = *op1 + *op2;
        }

    }
}

