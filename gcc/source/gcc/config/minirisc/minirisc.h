/* Target Definitions for minirisc.
   Copyright (C) 2008-2022 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef GCC_MINIRISC_H
#define GCC_MINIRISC_H

/******************************************************************************
 * Storage Layout
 ******************************************************************************/

#define TARGET_LITTLE_ENDIAN 1
#define BITS_BIG_ENDIAN 0
#define BYTES_BIG_ENDIAN (!TARGET_LITTLE_ENDIAN)
#define WORDS_BIG_ENDIAN (!TARGET_LITTLE_ENDIAN)

/* Number of storage units in a word; normally the size of a
   general-purpose register, a power of two from 1 or 8.  */
#define UNITS_PER_WORD 1

/* Width of a pointer, in bits. You must specify a value no wider than the width of Pmode.
   If it is not equal to the width of Pmode, you must define POINTERS_EXTEND_UNSIGNED.
   If you do not specify a value the default is BITS_PER_WORD. */
#define POINTER_SIZE 8

/* Normal alignment required for function parameters on the stack, in
   bits.  All stack parameters receive at least this much alignment
   regardless of data type.  */
#define PARM_BOUNDARY 8

/* Define this macro to the minimum alignment enforced by hardware
   for the stack pointer on this machine.  The definition is a C
   expression for the desired alignment (measured in bits).  */
#define STACK_BOUNDARY 8

/* Alignment required for a function entry point, in bits.  */
#define FUNCTION_BOUNDARY 8

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY 8

/* No data type wants to be aligned rounder than this.  */
#define BIGGEST_ALIGNMENT 8

/* The best alignment to use in cases where we have a choice.  */
#define FASTEST_ALIGNMENT 8

/* Every structures size must be a multiple of 8 bits.  */
#define STRUCTURE_SIZE_BOUNDARY 8

/* Look at the fundamental type that is used for a bit-field and use
   that to impose alignment on the enclosing structure.
   struct s {int a:8}; should have same alignment as "int", not "char".  */
#define	PCC_BITFIELD_TYPE_MATTERS 1

/* Largest integer machine mode for structures.  If undefined, the default
   is GET_MODE_SIZE(DImode).  */
#define MAX_FIXED_MODE_SIZE 8

/* Make arrays of chars word-aligned for the same reasons.  */
#define DATA_ALIGNMENT(TYPE, ALIGN)		\
  (TREE_CODE (TYPE) == ARRAY_TYPE		\
   && TYPE_MODE (TREE_TYPE (TYPE)) == QImode	\
   && (ALIGN) < FASTEST_ALIGNMENT ? FASTEST_ALIGNMENT : (ALIGN))

/* Set this nonzero if move instructions will actually fail to work
   when given unaligned data.  */
#define STRICT_ALIGNMENT 0


/******************************************************************************
 * Type Layout
 ******************************************************************************/

#define INT_TYPE_SIZE 8
#define SHORT_TYPE_SIZE 8
#define LONG_TYPE_SIZE 16
#define LONG_LONG_TYPE_SIZE 32

#define FLOAT_TYPE_SIZE 32
#define DOUBLE_TYPE_SIZE 64
#define LONG_DOUBLE_TYPE_SIZE 64

#define DEFAULT_SIGNED_CHAR 0

#undef  SIZE_TYPE
#define SIZE_TYPE "unsigned int"

#undef  PTRDIFF_TYPE
#define PTRDIFF_TYPE "int"

#undef  WCHAR_TYPE
#define WCHAR_TYPE "unsigned int"

#undef  WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE BITS_PER_WORD


/******************************************************************************
 * Registers
 ******************************************************************************/

/* Register Basics */

// Registers accessable from assembly
#define MINIRISC_R0   0 //return
#define MINIRISC_R1   1
#define MINIRISC_R2   2
#define MINIRISC_R3   3
#define MINIRISC_R4   4
#define MINIRISC_R5   5
#define MINIRISC_R6   6
#define MINIRISC_R7   7 
#define MINIRISC_R8   8
#define MINIRISC_R9   9 //prologue/epilogue
#define MINIRISC_R10  10
#define MINIRISC_R11  11
#define MINIRISC_R12  12
#define MINIRISC_R13  13

#define MINIRISC_SP  14 //sp...
#define MINIRISC_FP  15

// Virtual registers
#define MINIRISC_QFP 16
#define MINIRISC_QAP 17

// Not accessable from assebly
#define MINIRISC_PC  18
#define MINIRISC_CC  19

#define FIRST_PSEUDO_REGISTER 20

#define FIXED_REGISTERS                                           \
{                                                                 \
    /* го-г13=>0; r14-r19=>1 */                                   \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1    \
}

#define CALL_USED_REGISTERS                                       \
{                                                                 \
    /* r0=>0; г1-г8=>1; r9-r13=>0; r14-r19=>1 */                  \
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1    \
}

#define REG_ALLOC_ORDER                                                             \
{                                                                                   \
    1, 2, 3, 4, 5, 6, 7, 8,          /* caller saved registers */                    \
    10, 11, 12, 13,                 /* callee saved registers */                    \
    0,                              /* return value register */                     \
    9,                              /* used in prologue/epilogue construction */    \
    14, 15, 16, 17, 18, 19          /* fp, sp, (QFP, QAP, PC, CC) */                \
}
// 4, 3, 2, 1,                      /* argument registers */                        \

/* Values in Registers */

/* We can't copy to or from our CC register. */
#define AVOID_CCMODE_COPIES 1


/******************************************************************************
 * Register Classes
 ******************************************************************************/

enum reg_class
{
  NO_REGS,
  GENERAL_REGS,
  SPECIAL_REGS,
  CC_REGS,
  ALL_REGS,
  LIM_REG_CLASSES
};

#define N_REG_CLASSES LIM_REG_CLASSES

#define REG_CLASS_NAMES                         \
    { "NO_REGS",                                \
      "GENERAL_REGS",                           \
      "SPECIAL_REGS",                           \
      "CC_REGS",                                \
      "ALL_REGS" }

#define REG_CLASS_CONTENTS                         \
{ { 0x00000000 }, /* Empty */			               \
  { 0x0001FFFF }, /* R0 to R13, SP, FP, QFP */     \
  { 0x00040000 }, /* PC */	                        \
  { 0x00080000 }, /* CC */                         \
  { 0x000FFFFF }  /* All registers */              \
}

/* A C expression whose value is a register class containing hard
   register REGNO.  */
#define REGNO_REG_CLASS(R) ((R < MINIRISC_PC) ? GENERAL_REGS :		\
                            (R == MINIRISC_CC ? CC_REGS : SPECIAL_REGS))

/* A macro whose definition is the name of the class to which a valid
   base register must belong.  A base register is one used in an
   address which is the register value plus a displacement.  */
#define BASE_REG_CLASS GENERAL_REGS

#define INDEX_REG_CLASS NO_REGS

#define HARD_REGNO_OK_FOR_BASE_P(NUM) \
  ((unsigned) (NUM) < FIRST_PSEUDO_REGISTER \
   && (REGNO_REG_CLASS(NUM) == GENERAL_REGS \
       || (NUM) == HARD_FRAME_POINTER_REGNUM))

/* A C expression which is nonzero if register number NUM is suitable
   for use as a base register in operand addresses.  */
#ifdef REG_OK_STRICT
#define REGNO_OK_FOR_BASE_P(NUM)		 \
  (HARD_REGNO_OK_FOR_BASE_P(NUM) 		 \
   || HARD_REGNO_OK_FOR_BASE_P(reg_renumber[(NUM)]))
#else
#define REGNO_OK_FOR_BASE_P(NUM)		 \
  ((NUM) >= FIRST_PSEUDO_REGISTER || HARD_REGNO_OK_FOR_BASE_P(NUM))
#endif

/* A C expression which is nonzero if register number NUM is suitable
   for use as an index register in operand addresses.  */
#define REGNO_OK_FOR_INDEX_P(NUM) MINIRISC_FP


/******************************************************************************
 * Stack and Calling
 ******************************************************************************/

/* Frame Layout */

/* Define this macro if pushing a word onto the stack moves the stack
   pointer to a smaller address.  */
#define STACK_GROWS_DOWNWARD 1

/* Define this macro to nonzero value if the addresses of local variable slots
   are at negative offsets from the frame pointer.  */
#define FRAME_GROWS_DOWNWARD 1

/* Offset from the argument pointer register to the first argument's
   address.  On some machines it may depend on the data type of the
   function.  */
#define FIRST_PARM_OFFSET(F) 3

/* A C expression whose value is RTL representing the location of the
   incoming return address at the beginning of any function, before
   the prologue.  */
#define INCOMING_RETURN_ADDR_RTX					\
    gen_frame_mem (Pmode,                                               \
                   plus_constant (Pmode, stack_pointer_rtx, UNITS_PER_WORD))

/* Exception Handling */

/* Describe how we implement __builtin_eh_return.  */
#define EH_RETURN_DATA_REGNO(N)	((N) < 4 ? (N+2) : INVALID_REGNUM)

/* Store the return handler into the call frame.  */
#define EH_RETURN_HANDLER_RTX						\
    gen_frame_mem (Pmode,                                               \
                   plus_constant (Pmode, frame_pointer_rtx, UNITS_PER_WORD))

/* Frame Registers */

/* The register number of the stack pointer register, which must also
   be a fixed register according to `FIXED_REGISTERS'.  */
#define STACK_POINTER_REGNUM MINIRISC_SP

/* The register number of the frame pointer register, which is used to
   access automatic variables in the stack frame.  */
#define FRAME_POINTER_REGNUM MINIRISC_QFP

/* The register number of the arg pointer register, which is used to
   access the function's argument list.  */
#define ARG_POINTER_REGNUM MINIRISC_QAP

#define HARD_FRAME_POINTER_REGNUM MINIRISC_FP

/* Elimination */

#define ELIMINABLE_REGS                                         \
    {{ FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM },       \
     { ARG_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM }}

/* This macro returns the initial difference between the specified pair
   of registers.  */
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)			\
    do {                                                                \
        (OFFSET) = minirisc_initial_elimination_offset ((FROM), (TO));    \
    } while (0)

/* Stack Arguments */

/* Define this if it is the responsibility of the caller to allocate
   the area reserved for arguments passed in registers.  */
#define REG_PARM_STACK_SPACE(FNDECL) (6 * UNITS_PER_WORD)

/* Define this if the above stack space is to be considered part of the
   space allocated by the caller.  */
#define OUTGOING_REG_PARM_STACK_SPACE(FNTYPE) 1
#define STACK_PARMS_IN_REG_PARM_AREA

/* Register Arguments */

/* A C type for declaring a variable that is used as the first
   argument of `FUNCTION_ARG' and other related values.  */
#define CUMULATIVE_ARGS unsigned int

/* If defined, the maximum amount of space required for outgoing arguments
   will be computed and placed into the variable
   `current_function_outgoing_args_size'.  No space will be pushed
   onto the stack for each call; instead, the function prologue should
   increase the stack frame size by this amount.  */
#define ACCUMULATE_OUTGOING_ARGS 1

/* A C statement (sans semicolon) for initializing the variable CUM
   for the state at the beginning of the argument list.
   For minirisc, the first arg is passed in R0.  */
#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,FNDECL,N_NAMED_ARGS)    \
    (CUM = MINIRISC_R0)

/* A C expression that is nonzero if REGNO is the number of a hard
   register in which function arguments are sometimes passed.  */
#define FUNCTION_ARG_REGNO_P(r) 0

/* Function Entry and Exit */

/* Define this macro as a C expression that is nonzero for registers that are
   used by the epilogue or the return pattern.  The stack and frame
   pointer registers are already assumed to be used as needed.  */
#define EPILOGUE_USES(R) (R == MINIRISC_R9)

/* Profiling */

/* Generating Code for Profiling */
#define FUNCTION_PROFILER(FILE,LABELNO) (abort (), 0)


/******************************************************************************
 * Trampolines
 ******************************************************************************/

/* Trampolines for Nested Functions.  */
#define TRAMPOLINE_SIZE 0

/* Alignment required for trampolines, in bits.  */
#define TRAMPOLINE_ALIGNMENT 0


/******************************************************************************
 * Addressing Modes
 ******************************************************************************/

#define HAVE_PRE_INCREMENT 0
#define HAVE_PRE_DECREMENT 0
#define HAVE_POST_INCREMENT 0
#define HAVE_POST_DECREMENT 0
#define HAVE_PRE_MODIFY_DISP 0
#define HAVE_POST_MODIFY_DISP 0
#define HAVE_PRE_MODIFY_REG 0
#define HAVE_POST_MODIFY_REG 0

/* A number, the maximum number of registers that can appear in a
   valid memory address.  */
#define MAX_REGS_PER_ADDRESS 1

/* Define this macro as a C expression which is nonzero if accessing
   less than a word of memory (i.e. a `char' or a `short') is no
   faster than accessing a word of memory.  */
#define SLOW_BYTE_ACCESS 1


/******************************************************************************
 * Sections
 ******************************************************************************/

#define TEXT_SECTION_ASM_OP "\t.text"
#define DATA_SECTION_ASM_OP "\t.data"
#define READONLY_DATA_SECTION_ASM_OP "\t.rodata"
#define BSS_SECTION_ASM_OP "\t.bss"


/******************************************************************************
 * Assembler Format
 ******************************************************************************/

/* File Framework */

#define ASM_COMMENT_START "//"
#define ASM_APP_ON ""
#define ASM_APP_OFF ""
#define FILE_ASM_OP "\t.file\n"

/* Label Output */

#define GLOBAL_ASM_OP ".global\t"

/* Instruction Output */

#define REGISTER_NAMES                                                                                                            \
{                                                                                                                                 \
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "sp", "fp", "?fp", "?ap", "pc", "?cc" \
}

/* Alignment Output */

#define ASM_OUTPUT_ALIGN(STREAM,POWER)          \
    fprintf (STREAM, "\t.align\t%d\n", POWER);

#define ASM_OUTPUT_SKIP(FILE,SIZE) {}

/* Following is an  C statement which is used  to store into
   the string `LABEL',  a label whose name is  made from the
   string `PREFIX' and the number `NUM'.

   This string, when output subsequently by `assemble_name',
   should        produce        the       output        that
   `ASM_OUTPUT_INTERNAL_LABEL'  would produce with  the same
   prefix and num. */
#define ASM_GENERATE_INTERNAL_LABEL(LABEL,PREFIX,NUM)   \
    sprintf (LABEL, ".%s%ld", PREFIX, (long) NUM)

/* This macro says  that how to output an  assembler line to
   define a local common symbol. */
#define ASM_OUTPUT_ALIGNED_LOCAL(FILE, NAME, SIZE, ALIGNED)		\
    ( fputs ("\t.comm\t", (FILE)),					\
      assemble_name ((FILE), (NAME)),					\
      fprintf ((FILE), "," HOST_WIDE_INT_PRINT_UNSIGNED",%u\n",	\
               (SIZE), ((ALIGNED) / BITS_PER_UNIT)))

/* This macro says  that how to output an  assembler line to
   define a global common symbol. */
#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)                    \
    ( fputs ("\t.comm\t", (FILE)),                                      \
      assemble_name ((FILE), (NAME)),                                   \
      fprintf ((FILE), "," HOST_WIDE_INT_PRINT_UNSIGNED"\n", (SIZE)))


/******************************************************************************
 * Run-time Target
 ******************************************************************************/

#define TARGET_CPU_CPP_BUILTINS() \
  { \
    builtin_define_std ("minirisc");			\
    builtin_define_std ("MINIRISC");			\
    if (TARGET_LITTLE_ENDIAN)				\
      builtin_define ("__MINIRISC_LITTLE_ENDIAN__");	\
    else						\
      builtin_define ("__MINIRISC_BIG_ENDIAN__");		\
  }


/******************************************************************************
 * Misc.
 ******************************************************************************/

#define HAS_LONG_UNCOND_BRANCH true

/* The maximum number of bytes that a single instruction can move
   quickly between memory and registers or between two memory
   locations.  */
#define MOVE_MAX 1

/* All load operations zero extend.  */
#define LOAD_EXTEND_OP(MEM) ZERO_EXTEND

/* An alias for the machine mode for pointers.  */
#define Pmode QImode

/* An alias for the machine mode used for memory references to
   functions being called, in `call' RTL expressions.  */
#define FUNCTION_MODE QImode

/* An alias for a machine mode name.  This is the machine mode that
   elements of a jump-table should have.  */
#define CASE_VECTOR_MODE QImode

/* Use DWARF 2 debugging information by default.  */
#define DWARF2_DEBUGGING_INFO
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

#endif /* GCC_MINIRISC_H */
