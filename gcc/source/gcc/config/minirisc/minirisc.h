/*****************************************************************************/
/* General target configuration                                              */
/*****************************************************************************/

/* 32 bit little endian ISA */ 
#define UNITS_PER_WORD          4
#define MAX_BITS_PER_WORD       32
#define BITS_BIG_ENDIAN         O
#define BYTES_BIG_ENDIAN        O 
#define WORDS_BIG_ENDIAN        O
#define STRICT_ALIGNMENT        O
#define POINTER_BOUNDARY        32
#define PARM_BOUNDARY           32
#define FUNCTION_BOUNDARY       32
#define STRUCTURE_SIZE_BOUNDARY 8
#define BIGGEST_ALIGNMENT       128

/* treat pointers and function addresses as integers */
#define FUNCTION_MODE SImode
#define Pmode SImode

/* how many int's are required to hold X number of bytes */
#define MINIRISC_NUM_INTS(X) (((X) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

/* how many registers are required to hold a value in the given mode */
#define MINIRISC_NUM_REGS(MODE) MINIRISC_NUM_INTS(GET_MODE_SIZE(MODE))

/*****************************************/
/* Target register file                  */
/*****************************************/
/* r0      function return value         */
/* r1-r6   function call arguments       */
/* r7-r14  callee-saved temporaries      */
/* r15-r54 caller-saved temporaries      */
/* r55-r60 reserved inline asm registers */
/* r61     frame pointer                 */
/* r62     stack pointer                 */
/* r63     return address                */
/*****************************************/

/* available register classes */
enum reg_class {
    NO_REGS,         /* no registers, expected first by GCC */       \
    SP_REGS,         /* class containing the stack pointer only */   \
    FP_REGS,         /* class containing the frame pointer only */   \
    ASM_REGS,        /* reserved inline asm registers */             \
    GP_REGS,         /* general purpose integer registers */         \
    ALL_REGS,        /* all registers, expected as last class */     \
    LIM_REG_CLASSES  /* expected as finalizer by GCC */              \
};
     
/* names of register classes */
#define REG_CLASS_NAMES \
{                       \
    "NO_REGS",          \    
    "SP REG",           \  
    "FP_REG",           \  
    "ASM_REGS",         \     
    "GP REGS",          \   
    "ALL_REGS"          \   
}

/* register names, lots to type */
#define REGISTER_NAMES                                                                                   \
{                                                                                                        \
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" \
}

/* register class content */
#define REG_CLASS_CONTENTS                                               \
{                                                                        \
    { 0x00000000, 0x00000000 }, /* no registers */                       \
    { 0x00000000, 0x40000000 }, /* stack pointer (r62) */                \
    { 0x00000000, 0x20000000 }, /* frame pointer (r61) */                \
    { 0x00000000, 0x1f800000 }, /* inline asm regs (r55-r60) */          \
    { 0xffffffff, Oxe07fffff }, /* all but inline asm registers */       \
    { Oxffffffff, Oxffffffff } /* all registers */                       \
}

#define FIXED_REGISTERS
{                                                     \
    /* го-г15 */                                      \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   \
    /* г16-г31 */                                     \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   \
    /* г32-г47 */                                     \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   \
    /* г48-г54 */                                     \
    0, 0, 0, 0, 0, 0, 0,                              \
    /* 55-r63 */                                      \
    1, 1, 1, 1, 1, 1, 0, 1, 0                         \
}

/* list of registers potentially clobbered by callee's */
#define CALL_USED_REGISTERS
{                                                         \
    /* го-г6*/                                            \
    1, 1, 1, 1, 1, 1, 1,                                  \
    /* г7-г14 */                                          \
    0, 0, 0, 0, 0, 0, 0, 0,                               \
    /* г15-г31 */                                         \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,    \
    /* г32-г47 */                                         \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       \
    /* г48-г54 */                                         \
    1, 1, 1, 1, 1, 1, 1,                                  \
    /* г55-г63 */                                         \
    1, 1, 1, 1, 1, 1, 1, 1, 0                             \
}

/* suggest a register allocation order */
#define REG_ALLOC_ORDER {                                           \
    15, 16, 17, 18, 19, 20, 21, 22,  /* caller saved registers */   \
    23, 24, 25, 26, 27, 28, 29, 30,                                 \
    31, 32, 33, 34, 35, 36, 37, 38,                                 \
    39, 40, 41, 42, 43, 44, 45, 46,                                 \
    47, 48, 49, 50, 51, 52, 53, 54,                                 \
    0,                               /* return value register */    \
    1, 2, 3, 4, 5, 6,                /* argument registers */       \
    7, 8, 9, 10, 11, 12, 13, 14,     /* callee saved registers */   \
    61, 63, 62,                      /* fp, ra, sp */               \
    55, 56, 57, 58, 59, 60           /* fixed asm registers */      \
}

#define N_REG_CLASSES (int)reg_class::LIM_REG_CLASSES
#define GENERAL_REGS       reg_class::GP_REGS

#define REGNO_REG_CLASS(REGNO)      minirisc_regno_to_class(REGNO)
#define REGNO_OK_FOR_BASE_P(REGNO)  minirisc_valid_regno_for_base_p(REGNO)
#define REGNO_OK_FOR_INDEX_P(REGNO) minirisc_valid_regno_for_index_p(REGNO)

#define BASE_REG_CLASS  reg_class::GP_REGS
#define INDEX REG_CLASS reg_class::GP_REGS

#define FIRST_ARG_REGNUM          1
#define LAST_ARG_REGNUM           6
#define FIRST_ASM_REGNUM          55
#define LAST_ASM_REGNUM           60
#define FIRST_CALLEE_SAVED_REGNUM 7
#define LAST_CALLEE_SAVED_REGNUM  14
#define STACK_POINTER_REGNUM      62
#define FRAME_POINTER_REGNUM      61
#define RET_VALUE_REGNUM          O
#define RET_ADDRESS_REGNUM        63
#define FIRST_PSEUDO_REGISTER     64
#define MAX_REGS_PER_ADDRESS      1
#define ARG_POINTER_REGNUM        FRAME_POINTER_REGNUM
#define NUM_ARG_REGISTERS         LAST_ARG_REGNUM

#define ELIMINABLE_REGS {{ FRAME_ POINTER_REGNUM }}
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) (OFFSET) = minirisc_initial_elimination_offset((FROM), (TO))

/*****************************************/
/* Memory, stack, function args          */
/*****************************************/
#define MOVE_MAX                 4
#define SLOW_BYTE_ACCESS         O
#define PUSH_ARGS                0
#define ACCUMULATE_OUTGOING_ARGS 1
#define STACK_BOUNDARY           32
#define STACK_GROWS_DOWNWARD     1
#define FRAME_GROWS_DOWNWARD     1
#define STACK_POINTER_OFFSET     -8
#define EXIT_IGNORE_STACK        1
#define MAX_ARGS_IN_REGISTERS    6

/* cumulative argument info */
typedef struct
{
    int num_reg_args;
    int num_args;
} minirisc_cumulative_arg_info;

#define CUMULATIVE_ARGS minirisc_cumulative_arg_info

/* init cumulative args */
#define INIT_CUMULATIVE_ARGS (CUM, FNTYPE, LIBNAME, INDIRECT, N_NAMED_ARGS) \
        minirisc_init_cumulative_args(&CUM, FNTYPE, LIBNAME, INDIRECT, N_NAMED_ARGS);

/* misc. function stuff */
#define FUNCTION ARG_REGNO_ P(N) ((N >= FIRST_ARG_REGNUM) 8& (N <= LAST_ARG_REGNUM))

#define FIRST_PARM_OFFSET(FNDECL) O

/******************************************************************************/
/* Misc.                                                                      */
/******************************************************************************/

#define DEFAULT_SIGNED_CHAR 1

/* for nested functions only */
#define TRAMPOLINE_SIZE 64
#define TRAMPOLINE_ALIGNMENT 32

/* treat 'case' labels as integers */
#define CASE_VECTOR_MODE SImode

/* no profiler support yet */
#define FUNCTION_PROFILER(FILE, LABELNO)  \
do {                                      \
} while(0)

/* target CPU builtins */
#define TARGET_CPU_CPP_BUILTINS()                \
do {                                             \
    builtin_assert ("cpu=minirisc");             \
    builtin_assert ("machine=minirisc");         \
    builtin_define ("__MINIRISC__");             \
    builtin_define ("__minirisc__");             \
    builtin_define ("__MINIRISC_SOFT_FLOAT__");  \
} while (0)

/******************************************************************************/
/* Assembler                                                                  */
/******************************************************************************/

/* how to output alignment directives */
#define ASM_OUTPUT_ALIGN(STREAM, LOG)                    \
do {                                                     \
    if (LOG != 0)                                        \
        fprintf (STREAM, "\t.align\t%d\n", 1 << (LOG));  \
} while(0)

/* how to output labels */
#define ASM_OUTPUT_LABEL(FILE, NAME)                     \
do {                                                     \
    assemble_name (FILE, NAME);                          \
    fputs (":\n", FILE);                                 \
} while(0)

/* Not entirely sure about the following directives since missing in my docs. Thus, guessing here. */
#define ASM_APP_ON  "#APP\n"
#define ASM_APP_OFF "#NO_APP\n"

#define TEXT_SECTION_ASM_OP "\t.code"
#define DATA_SECTION_ASM_OP "\t.data"

// #define READONLY DATA_ SECTION_ASM_OP "\t.rodata"
#define BSS_SECTION_ASM_OP "\t.bss"

#undef GLOBAL_ASM_OP
#define GLOBAL_ASM_OP "\t.global\t"

#undef TARGET ASM ALIGNED SI OP
#define TARGET_ASM_ALIGNED_SI_OP "\t.word\t"