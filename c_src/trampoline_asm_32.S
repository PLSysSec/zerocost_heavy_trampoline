#include "ctx_save_trampoline_internal.hpp"

#ifndef HEAVY_TRAMPOLINE_MODIFY_FUNCTION_CALL
#define HEAVY_TRAMPOLINE_MODIFY_FUNCTION_CALL(x) x
#endif

/*
 * param1(rdi): TransitionContext*
 */
.globl switch_execution_context
.type switch_execution_context,@function
.align 16
switch_execution_context:
_switch_execution_context:

        /* Stage 0: Pick regs */

        // Stores the current ctx structure
        //      - Callersave GPR not frame or stack
        //      - Cannot be a function parameter register
        //      - Cannot be a reg used by movsb
        //      - If changing, also adjust the use of "REG_PARAM specific" below
        #define REG_PARAM %edx
        // Temporary that holds computed end of stack frame after stack param copies
        //      - Callersave GPR not frame or stack
        //      - Cannot be REG_PARAM
        //      - Cannot be a reg used by movsb
        #define REG_TMP_STACK_FRAME %eax
        // Temporary for memory load operations
        //      - Callersave GPR not frame or stack
        //      - Cannot be REG_PARAM or REG_TMP_STACK_FRAME
        //      - If changing, also adjust the use of "REG_TMP_LOAD specific" below
        #define REG_TMP_LOAD %ecx
        // Temporary for PIC support
        //      - GPR not frame or stack
        //      - Cannot be REG_PARAM
        #define REG_TMP_PIC %esi
        #define GET_REG_TMP_PIC __x86.get_pc_thunk.si

        movl    0x4(%esp), REG_PARAM

        /* Stage 1: Save relevant context_from state */

        // Save callee save registers and stack
        movl    %ebx, TRANSITION_CONTEXT_OFFSET_EBX(REG_PARAM)
        movl    %edi, TRANSITION_CONTEXT_OFFSET_EDI(REG_PARAM)
        movl    %esi, TRANSITION_CONTEXT_OFFSET_ESI(REG_PARAM)
        // Floating point control word and mxcsr are partially callee saved --- just save the whole thing
        fnstcw  TRANSITION_CONTEXT_OFFSET_SOURCE_FCW(REG_PARAM)
        stmxcsr TRANSITION_CONTEXT_OFFSET_SOURCE_MXCSR(REG_PARAM)
        // Save stack and frame ptr
        movl    %esp, TRANSITION_CONTEXT_OFFSET_SOURCE_STACK_PTR(REG_PARAM)
        movl    %ebp, TRANSITION_CONTEXT_OFFSET_SOURCE_FRAME_PTR(REG_PARAM)

        /* Stage 2: Load relevant context_to state */

        // stack_ptr = target_stack
        // if (!stack_ptr) {
        //     // dont switch stacks
        //     stack_ptr = $rsp;
        // }
        movl    TRANSITION_CONTEXT_OFFSET_TARGET_STACK_PTR(REG_PARAM), REG_TMP_STACK_FRAME
        test    REG_TMP_STACK_FRAME, REG_TMP_STACK_FRAME
        cmovz   %esp, REG_TMP_STACK_FRAME

        // Copy the stack parameters to the target stack frame
        // if parameters_size > 0 {
        //    stack_ptr -= parameters_size
        //    // align 16
        //    stack_ptr -= (stack_ptr % 16);
        //    memcpy(stack_ptr, parameters_buff, parameters_size)
        // }
        andl    $-16, REG_TMP_STACK_FRAME
        movl    TRANSITION_CONTEXT_OFFSET_STACK_PARAMS_SIZE(REG_PARAM), REG_TMP_LOAD
        testl   REG_TMP_LOAD, REG_TMP_LOAD
        je      switch_execution_context_params_done

        // Copy the parameters
        movl    REG_TMP_LOAD, %ecx
        movl    TRANSITION_CONTEXT_OFFSET_STACK_PARAMS_BUFFER(REG_PARAM), %esi
        subl    REG_TMP_LOAD, REG_TMP_STACK_FRAME
        andl    $-16, REG_TMP_STACK_FRAME
        movl    REG_TMP_STACK_FRAME, %edi
        rep movsb

switch_execution_context_params_done:

        // Load the stack pointer of the context_to
        movl    REG_TMP_STACK_FRAME, %esp
        // Set the frame pointer also to make sure previous frame pointer val does not leak
        movl    REG_TMP_STACK_FRAME, %ebp

        // Clear non parameter gpr registers to prevents leaks
        // PIC support
        call GET_REG_TMP_PIC
        // Clear the x87, MMX, and SSE state.
        fxrstor fxrstor_default_state-.(REG_TMP_PIC)
        // Clear the AVX state that the "fxrstor" instruction doesn't cover.
        vzeroupper
        // xor also leaves eflags in a fixed known state, so no leaks through this
        xor     %eax, %eax
        xor     %ebx, %ebx
        xor     %ecx, %ecx
        // REG_PARAM specific: Disable edx clear as it is being used for REG_PARAM
        // xor     %edx, %edx
        xor     %esi, %esi
        xor     %edi, %edi
        // Clear direction flag before call or return
        cld

        // Load float state
        fldcw   TRANSITION_CONTEXT_OFFSET_TARGET_FCW(REG_PARAM)
        ldmxcsr TRANSITION_CONTEXT_OFFSET_TARGET_MXCSR(REG_PARAM)

        // Load the target function ptr
        movl    TRANSITION_CONTEXT_OFFSET_TARGET_PROG_CTR(REG_PARAM), REG_PARAM
        HEAVY_TRAMPOLINE_MODIFY_FUNCTION_CALL(calll   *REG_PARAM)

        #undef REG_PARAM
        #undef REG_TMP_STACK_FRAME
        #undef REG_TMP_LOAD
        #undef REG_TMP_PIC
        #undef GET_REG_TMP_PIC

        // Fall-through

// switch_execution_context_out:

        /* Stage 0: Pick regs. Need temporary working regs which are caller
        saved by the invoker of switch_execution_context
        */

        // Temporary that holds part 1 of the GPR return
        //      - Calleesave GPR not frame or stack
        //      - Cannot be a return GPR
        #define REG_TMP_RET1 %ebx
        // Temporary that holds part 2 of the GPR return
        //      - Calleesave GPR not frame or stack
        //      - Cannot be REG_TMP_RET1
        //      - Cannot be a return GPR
        #define REG_TMP_RET2 %esi
        // Stores the current ctx structure
        //      - Callersave GPR not frame or stack
        //      - Cannot be REG_TMP_RET1 or REG_TMP_RET2
        #define REG_PARAM %ecx
        // Temporary for PIC support
        //      - GPR not frame or stack
        //      - Cannot be REG_PARAM
        #define REG_TMP_PIC %esi
        #define GET_REG_TMP_PIC __x86.get_pc_thunk.si

        // Save return values that could get overwritten by this function call
        movl    %eax, REG_TMP_RET1
        movl    %edx, REG_TMP_RET2

        // retrieve the param again
        calll   get_saved_transition_context@PLT
        movl    %eax, REG_PARAM

        /* Stage 1: Save relevant context_to state */

        // Save returned values
        movl    REG_TMP_RET1, TRANSITION_CONTEXT_OFFSET_EAX(REG_PARAM)
        movl    REG_TMP_RET2, TRANSITION_CONTEXT_OFFSET_EDX(REG_PARAM)
        fstl    TRANSITION_CONTEXT_OFFSET_ST0(REG_PARAM)

        /* Stage 2: Load relevant context_from state */

        // Clear regs that are caller save and not return registers
        // Needed when returning into the sandbox from app --- caller is sandbox, callee is app
        // xor also leaves eflags in a fixed known state, so no leaks through this
        // REG_PARAM specific: Delay ecx clear as it is being used for REG_PARAM
        // xorl    %ecx, %ecx
        // Clear direction flag before call or return
        cld
        // PIC support
        call GET_REG_TMP_PIC
        // Clear the x87, MMX, and SSE state.
        fxrstor fxrstor_default_state-.(REG_TMP_PIC)
        // Clear the AVX state that the "fxrstor" instruction doesn't cover.
        vzeroupper

        // Restore callee save registers
        movl    TRANSITION_CONTEXT_OFFSET_EBX(REG_PARAM), %ebx
        movl    TRANSITION_CONTEXT_OFFSET_EDI(REG_PARAM), %edi
        movl    TRANSITION_CONTEXT_OFFSET_ESI(REG_PARAM), %esi
        // Restore fp control word which is callee save
        fldcw   TRANSITION_CONTEXT_OFFSET_SOURCE_FCW(REG_PARAM)
        ldmxcsr TRANSITION_CONTEXT_OFFSET_SOURCE_MXCSR(REG_PARAM)
        // Restore stack
        movl    TRANSITION_CONTEXT_OFFSET_SOURCE_STACK_PTR(REG_PARAM), %esp
        movl    TRANSITION_CONTEXT_OFFSET_SOURCE_FRAME_PTR(REG_PARAM), %ebp

        // Restore return values in registers
        movl    TRANSITION_CONTEXT_OFFSET_EAX(REG_PARAM), %eax
        movl    TRANSITION_CONTEXT_OFFSET_EDX(REG_PARAM), %edx
        fldl    TRANSITION_CONTEXT_OFFSET_ST0(REG_PARAM)

        // REG_PARAM specific: Delayed ecx clear now
        xorl    %ecx, %ecx

        ret

        #undef REG_TMP_RET1
        #undef REG_TMP_RET2
        #undef REG_PARAM
        #undef REG_TMP_PIC
        #undef GET_REG_TMP_PIC

__x86.get_pc_thunk.si:
        .cfi_startproc
        movl (%esp), %esi
        ret
        .cfi_endproc

.section .rodata, "a"
        /*
         * This is the memory block for "fxrstor" to read.  The only
         * contents that matter are the fcw and mxcsr words, which we
         * store separately.  The mxcsr_mask word is ignored by the
         * hardware, so there is no need to get the hardware-supplied
         * value for that.  The hardware requires that this address be
         * aligned to 16 bytes.  Align it further to 64 bytes because
         * that is the usual size of a cache line; this might help
         * performance and is very unlikely to hurt it.
         */
        .balign 64
fxrstor_default_state:
        .space 512
        /*
         * This is the memory block for "frstor" to read.  It matters that
         * some of these words have the expected nonzero values, so it's
         * not possible to reuse the fxrstor_default_state space for this
         * too.  The bit pattern below is the usual default state.
         */
frstor_default_state:
        .long 0xffff037f  /* x87 control word */
        .long 0xffff0000  /* x87 status word */
        .long 0xffffffff  /* x87 tag word */
        .long 0           /* x87 IP offset */
        .long 0           /* x87 IP selector */
        .long 0           /* x87 operand pointer offset */
        .long 0xffff0000  /* x87 operand pointer selector */
        /*
         * This is better written as: .space 108 - (. - frstor_default_state).
         * But the LLVM assembler (used on MacOS and in Clang builds on
         * Linux) is buggy and does not support that use of label arithmetic.
         * Reported as: http://llvm.org/bugs/show_bug.cgi?id=15315
         */
        .space 108 - (7 * 4)

#undef HEAVY_TRAMPOLINE_MODIFY_FUNCTION_CALL
