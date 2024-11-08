/*****************************************************************************/
/* General target configuration                                              */
/*****************************************************************************/

/* 32 bit little endian ISA */ 
// #define UNITS_PER_WORD          4
// #define MAX_BITS_PER_WORD       32
// #define BITS_BIG_ENDIAN         0
// #define BYTES_BIG_ENDIAN        0 
// #define WORDS_BIG_ENDIAN        0
// #define STRICT_ALIGNMENT        0
// #define POINTER_BOUNDARY        32
// #define PARM_BOUNDARY           32
// #define FUNCTION_BOUNDARY       32
// #define STRUCTURE_SIZE_BOUNDARY 8
// #define BIGGEST_ALIGNMENT       128

#define UNITS_PER_WORD          1   // 8-bit data word
#define MAX_BITS_PER_WORD       8   // Data width in bits
#define BITS_BIG_ENDIAN         0   // Little-endian bit order within bytes
#define BYTES_BIG_ENDIAN        0   // Little-endian byte order
#define WORDS_BIG_ENDIAN        0   // Little-endian word order

#define STRICT_ALIGNMENT        0   // Set to 1 if unaligned access isn't allowed
#define POINTER_BOUNDARY        8         // Code memory is 16-bit wide
#define PARM_BOUNDARY           8         // 8-bit boundary for parameters
#define FUNCTION_BOUNDARY       8  // Function alignment to code memory width
#define STRUCTURE_SIZE_BOUNDARY 8         // Align structures to 8-bit boundary
#define BIGGEST_ALIGNMENT       8  // Largest alignment, based on code width

/* treat pointers and function addresses as integers */
#define FUNCTION_MODE HImode
#define Pmode HImode

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
    SP_REG,          /* class containing the stack pointer only */   \
    GP_REGS,         /* all registers */     \
    ALL_REGS,        /* all registers, expected as last class */     \
    LIM_REG_CLASSES  /* expected as finalizer by GCC */              \
};
     
/* names of register classes */
#define REG_CLASS_NAMES \
{                       \
    "NO_REGS",          \
    "SP_REG",           \
    "GP_REGS",          \
    "ALL_REGS"          \
}

/* register names, lots to type */
#define REGISTER_NAMES                                                                                   \
{                                                                                                        \
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" \
}

/* register class content */
#define REG_CLASS_CONTENTS                                  \
{                                                           \
    { 0x00000000 }, /* no registers */                      \
    { 0x00008000 }, /* stack pointer (r15) */               \
    { 0xffffffff }, /* GP registers */                      \
    { 0xffffffff }  /* all registers */                     \
}

#define FIXED_REGISTERS                               \
{                                                     \
    /* го-г15 */                                      \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0    \
}

/* list of registers potentially clobbered by callee's */
#define CALL_USED_REGISTERS                               \
{                                                         \
    /* го-г9*/                                            \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                         \
    /* г10-г13 */                                         \
    0, 0, 0, 0,                                           \
    /* г14-г15 */                                         \
    1, 1                                                  \
}

/* suggest a register allocation order */
#define REG_ALLOC_ORDER                                             \
{                                                                   \
    5, 6, 7, 8, 9,                   /* caller saved registers */   \
    10, 11, 12, 13,                  /* callee saved registers */   \
    0,                               /* return value register */    \
    4, 3, 2, 1,                      /* argument registers */       \
    14, 15,                          /* fp, sp */                   \
}

#define N_REG_CLASSES (int)reg_class::LIM_REG_CLASSES
#define GENERAL_REGS       reg_class::GP_REGS

#define REGNO_REG_CLASS(REGNO)      minirisc_regno_to_class(REGNO)
#define REGNO_OK_FOR_BASE_P(REGNO)  minirisc_valid_regno_for_base_p(REGNO)
#define REGNO_OK_FOR_INDEX_P(REGNO) minirisc_valid_regno_for_index_p(REGNO)

#define BASE_REG_CLASS  reg_class::GP_REGS
#define INDEX_REG_CLASS reg_class::GP_REGS

#define FIRST_ARG_REGNUM          1
#define LAST_ARG_REGNUM           4
#define FIRST_CALLEE_SAVED_REGNUM 10
#define LAST_CALLEE_SAVED_REGNUM  13
#define RET_VALUE_REGNUM          0
#define FRAME_POINTER_REGNUM      14
#define STACK_POINTER_REGNUM      15
#define FIRST_PSEUDO_REGISTER     16
#define MAX_REGS_PER_ADDRESS      1
#define ARG_POINTER_REGNUM        FRAME_POINTER_REGNUM
#define NUM_ARG_REGISTERS         LAST_ARG_REGNUM - FIRST_ARG_REGNUM

#define ELIMINABLE_REGS {{ FRAME_POINTER_REGNUM }}
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) (OFFSET) = minirisc_initial_elimination_offset((FROM), (TO))

/*****************************************/
/* Memory, stack, function args          */
/*****************************************/
#define MOVE_MAX                 1
#define SLOW_BYTE_ACCESS         0
#define PUSH_ARGS                0
#define ACCUMULATE_OUTGOING_ARGS 1
#define STACK_BOUNDARY           8
#define STACK_GROWS_DOWNWARD     1
#define FRAME_GROWS_DOWNWARD     1
#define STACK_POINTER_OFFSET     -1
#define EXIT_IGNORE_STACK        1
#define MAX_ARGS_IN_REGISTERS    4

/* cumulative argument info */
typedef struct
{
    int num_reg_args;
    int num_args;
} minirisc_cumulative_arg_info;

#define CUMULATIVE_ARGS minirisc_cumulative_arg_info

/* init cumulative args */
#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, INDIRECT, N_NAMED_ARGS) \
        minirisc_init_cumulative_args(&CUM, FNTYPE, LIBNAME, INDIRECT, N_NAMED_ARGS);

/* misc. function stuff */
#define FUNCTION_ARG_REGNO_P(N) ((N >= FIRST_ARG_REGNUM) && (N <= LAST_ARG_REGNUM))

#define FIRST_PARM_OFFSET(FNDECL) 0

/******************************************************************************/
/* Misc.                                                                      */
/******************************************************************************/

#define DEFAULT_SIGNED_CHAR 1

/* for nested functions only */
#define TRAMPOLINE_SIZE 8
#define TRAMPOLINE_ALIGNMENT 16

/* treat 'case' labels as integers */
#define CASE_VECTOR_MODE HImode

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

#undef TARGET_ASM_ALIGNED_SI_OP
#define TARGET_ASM_ALIGNED_SI_OP "\t.word\t"