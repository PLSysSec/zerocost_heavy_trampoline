#pragma once

#if defined(__x86_64__)

    #define TRANSITION_CONTEXT_OFFSET_RAX 0x0
    #define TRANSITION_CONTEXT_OFFSET_RBX 0x8
    #define TRANSITION_CONTEXT_OFFSET_RCX 0x10
    #define TRANSITION_CONTEXT_OFFSET_RDX 0x18
    #define TRANSITION_CONTEXT_OFFSET_RSI 0x20
    #define TRANSITION_CONTEXT_OFFSET_RDI 0x28
    #define TRANSITION_CONTEXT_OFFSET_R8 0x30
    #define TRANSITION_CONTEXT_OFFSET_R9 0x38
    #define TRANSITION_CONTEXT_OFFSET_R10 0x40
    #define TRANSITION_CONTEXT_OFFSET_R11 0x48
    #define TRANSITION_CONTEXT_OFFSET_R12 0x50
    #define TRANSITION_CONTEXT_OFFSET_R13 0x58
    #define TRANSITION_CONTEXT_OFFSET_R14 0x60
    #define TRANSITION_CONTEXT_OFFSET_R15 0x68
    #define TRANSITION_CONTEXT_OFFSET_XMM0 0x70
    #define TRANSITION_CONTEXT_OFFSET_XMM1 0x78
    #define TRANSITION_CONTEXT_OFFSET_XMM2 0x80
    #define TRANSITION_CONTEXT_OFFSET_XMM3 0x88
    #define TRANSITION_CONTEXT_OFFSET_XMM4 0x90
    #define TRANSITION_CONTEXT_OFFSET_XMM5 0x98
    #define TRANSITION_CONTEXT_OFFSET_XMM6 0xa0
    #define TRANSITION_CONTEXT_OFFSET_XMM7 0xa8
    #define TRANSITION_CONTEXT_OFFSET_XMM8 0xb0
    #define TRANSITION_CONTEXT_OFFSET_XMM9 0xb8
    #define TRANSITION_CONTEXT_OFFSET_XMM10 0xc0
    #define TRANSITION_CONTEXT_OFFSET_XMM11 0xc8
    #define TRANSITION_CONTEXT_OFFSET_XMM12 0xd0
    #define TRANSITION_CONTEXT_OFFSET_XMM13 0xd8
    #define TRANSITION_CONTEXT_OFFSET_XMM14 0xe0
    #define TRANSITION_CONTEXT_OFFSET_XMM15 0xe8
    #define TRANSITION_CONTEXT_OFFSET_SOURCE_FRAME_PTR 0xf0
    #define TRANSITION_CONTEXT_OFFSET_SOURCE_STACK_PTR 0xf8
    #define TRANSITION_CONTEXT_OFFSET_TARGET_FRAME_PTR 0x100
    #define TRANSITION_CONTEXT_OFFSET_TARGET_STACK_PTR 0x108
    #define TRANSITION_CONTEXT_OFFSET_SOURCE_PROG_CTR 0x110
    #define TRANSITION_CONTEXT_OFFSET_TARGET_PROG_CTR 0x118
    #define TRANSITION_CONTEXT_OFFSET_SOURCE_MXCSR 0x120
    #define TRANSITION_CONTEXT_OFFSET_TARGET_MXCSR 0x128
    #define TRANSITION_CONTEXT_OFFSET_SOURCE_FCW 0x130
    #define TRANSITION_CONTEXT_OFFSET_TARGET_FCW 0x138
    #define TRANSITION_CONTEXT_OFFSET_STACK_PARAMS_SIZE 0x140
    #define TRANSITION_CONTEXT_OFFSET_STACK_PARAMS_BUFFER 0x148
    #define TRANSITION_CONTEXT_OFFSET_WINDOWS_MODE_ABI 0x150
    #define TRANSITION_CONTEXT_TOTAL_SIZE 0x158

    #if !defined(__ASSEMBLER__)

        #include <stdint.h>

        #ifdef __cplusplus
        extern "C" {
        #endif

        typedef struct TransitionContext {
            /*          0x0, 0x8, 0x10, 0x18, 0x20, 0x28 */
            uint64_t    rax, rbx,  rcx,  rdx,  rsi,  rdi;
            /*          0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68 */
            uint64_t    r8,     r9,  r10,  r11,  r12,  r13,  r14,  r15;
            /*          0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xa0, 0xa8 */
            uint64_t    xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
            /*          0xb0, 0xb8,  0xc0,  0xc8,  0xd0,  0xd8,  0xe0,  0xe8 */
            uint64_t    xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15;
            /*          0xf0,                         0xf8,            0x100,          0x108 */
            uint64_t    source_frame_ptr, source_stack_ptr, target_frame_ptr, target_stack_ptr;
            /*          0x110,                     0x118 */
            uint64_t    source_prog_ctr, target_prog_ctr;
            /*          0x120,               0x128 */
            uint64_t    source_mxcsr, target_mxcsr; // these are 32-bit, but 64-bit simplifies offsets
            /*          0x130,           0x138 */
            uint64_t    source_fcw, target_fcw; // these are 16-bit, but 64-bit simplifies offsets
            /*          0x140            */
            uint64_t    stack_params_size;
            /*          0x148              */
            void*       stack_params_buffer;
            /*          0x150 */
            uint64_t    windows_mode_abi;
            /* end:     0x158 */
        } TransitionContext;

        TransitionContext* get_saved_transition_context();
        void set_saved_transition_context(TransitionContext* val);

        #ifdef __cplusplus

        }

        #define NACL_CHECK_FIELD(offset_val, field)                                           \
            static_assert(offset_val == __builtin_offsetof(TransitionContext, field));

        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_RAX, rax);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_RBX, rbx);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_RCX, rcx);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_RDX, rdx);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_RSI, rsi);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_RDI, rdi);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_R8, r8);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_R9, r9);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_R10, r10);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_R11, r11);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_R12, r12);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_R13, r13);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_R14, r14);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_R15, r15);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM0, xmm0);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM1, xmm1);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM2, xmm2);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM3, xmm3);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM4, xmm4);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM5, xmm5);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM6, xmm6);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM7, xmm7);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM8, xmm8);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM9, xmm9);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM10, xmm10);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM11, xmm11);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM12, xmm12);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM13, xmm13);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM14, xmm14);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_XMM15, xmm15);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_SOURCE_FRAME_PTR, source_frame_ptr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_SOURCE_STACK_PTR, source_stack_ptr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_TARGET_FRAME_PTR, target_frame_ptr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_TARGET_STACK_PTR, target_stack_ptr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_SOURCE_PROG_CTR, source_prog_ctr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_TARGET_PROG_CTR, target_prog_ctr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_SOURCE_MXCSR, source_mxcsr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_TARGET_MXCSR, target_mxcsr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_SOURCE_FCW, source_fcw);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_TARGET_FCW, target_fcw);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_STACK_PARAMS_SIZE, stack_params_size);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_STACK_PARAMS_BUFFER, stack_params_buffer);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_WINDOWS_MODE_ABI, windows_mode_abi);
        static_assert(TRANSITION_CONTEXT_TOTAL_SIZE == sizeof(TransitionContext));

        #undef NACL_CHECK_FIELD

        #endif
    #endif

#elif defined(__i386__)

    #define TRANSITION_CONTEXT_OFFSET_EAX 0x0
    #define TRANSITION_CONTEXT_OFFSET_EBX 0x4
    #define TRANSITION_CONTEXT_OFFSET_ECX 0x8
    #define TRANSITION_CONTEXT_OFFSET_EDX 0xc
    #define TRANSITION_CONTEXT_OFFSET_ESI 0x10
    #define TRANSITION_CONTEXT_OFFSET_EDI 0x14
    #define TRANSITION_CONTEXT_OFFSET_ST0 0x18
    #define TRANSITION_CONTEXT_OFFSET_SOURCE_FRAME_PTR 0x20
    #define TRANSITION_CONTEXT_OFFSET_SOURCE_STACK_PTR 0x24
    #define TRANSITION_CONTEXT_OFFSET_TARGET_FRAME_PTR 0x28
    #define TRANSITION_CONTEXT_OFFSET_TARGET_STACK_PTR 0x2c
    #define TRANSITION_CONTEXT_OFFSET_SOURCE_PROG_CTR 0x30
    #define TRANSITION_CONTEXT_OFFSET_TARGET_PROG_CTR 0x34
    #define TRANSITION_CONTEXT_OFFSET_SOURCE_MXCSR 0x38
    #define TRANSITION_CONTEXT_OFFSET_TARGET_MXCSR 0x3c
    #define TRANSITION_CONTEXT_OFFSET_SOURCE_FCW 0x40
    #define TRANSITION_CONTEXT_OFFSET_TARGET_FCW 0x44
    #define TRANSITION_CONTEXT_OFFSET_STACK_PARAMS_SIZE 0x48
    #define TRANSITION_CONTEXT_OFFSET_STACK_PARAMS_BUFFER 0x4c
    #define TRANSITION_CONTEXT_OFFSET_WINDOWS_MODE_ABI 0x50
    #define TRANSITION_CONTEXT_TOTAL_SIZE 0x54

    #if !defined(__ASSEMBLER__)

        #include <stdint.h>

        #ifdef __cplusplus
        extern "C" {
        #endif

        typedef struct TransitionContext {
            /*          0x0, 0x4, 0x8, 0xc, 0x10, 0x14 */
            uint32_t    eax, ebx,  ecx,  edx,  esi,  edi;
            /*          0x18 */
            uint64_t    st0;
            /*          0x20,                         0x24,             0x28,             0x2c */
            uint32_t    source_frame_ptr, source_stack_ptr, target_frame_ptr, target_stack_ptr;
            /*          0x30,                        0x34 */
            uint32_t    source_prog_ctr, target_prog_ctr;
            /*          0x38,                 0x3c */
            uint32_t    source_mxcsr, target_mxcsr;
            /*          0x40,             0x44 */
            uint32_t    source_fcw, target_fcw; // these are 16-bit, but 32-bit simplifies offsets
            /*          0x48             */
            uint32_t    stack_params_size;
            /*          0x4c               */
            void*       stack_params_buffer;
            /*          0x50 */
            uint32_t    windows_mode_abi;
            /* end:     0x54 */
        } TransitionContext;

        TransitionContext* get_saved_transition_context();
        void set_saved_transition_context(TransitionContext* val);

        #ifdef __cplusplus

        }

        #define NACL_CHECK_FIELD(offset_val, field)                                           \
            static_assert(offset_val == __builtin_offsetof(TransitionContext, field));

        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_EAX, eax);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_EBX, ebx);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_ECX, ecx);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_EDX, edx);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_ESI, esi);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_EDI, edi);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_ST0, st0);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_SOURCE_FRAME_PTR, source_frame_ptr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_SOURCE_STACK_PTR, source_stack_ptr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_TARGET_FRAME_PTR, target_frame_ptr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_TARGET_STACK_PTR, target_stack_ptr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_SOURCE_PROG_CTR, source_prog_ctr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_TARGET_PROG_CTR, target_prog_ctr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_SOURCE_MXCSR, source_mxcsr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_TARGET_MXCSR, target_mxcsr);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_SOURCE_FCW, source_fcw);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_TARGET_FCW, target_fcw);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_STACK_PARAMS_SIZE, stack_params_size);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_STACK_PARAMS_BUFFER, stack_params_buffer);
        NACL_CHECK_FIELD(TRANSITION_CONTEXT_OFFSET_WINDOWS_MODE_ABI, windows_mode_abi);
        static_assert(TRANSITION_CONTEXT_TOTAL_SIZE == sizeof(TransitionContext));

        #undef NACL_CHECK_FIELD

        #endif
    #endif

#else

#error "Unknown architecture"

#endif