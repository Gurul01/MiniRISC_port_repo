/* Target Code for mrmr16
   Copyright (C) 2008-2021 Free Software Foundation, Inc.

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

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "df.h"
#include "regs.h"
#include "memmodel.h"
#include "emit-rtl.h"
#include "diagnostic-core.h"
#include "output.h"
#include "stor-layout.h"
#include "varasm.h"
#include "calls.h"
#include "expr.h"
#include "builtins.h"
//#include "dbxout.h"
#include "ggc.h"

/* This file should be included last.  */
#include "target-def.h"

void mrmr16_split_symbolic_move (rtx, rtx);

/* Per-function machine data.  */
struct GTY(()) machine_function
{
  /* Number of bytes saved on the stack for callee saved registers.  */
  int callee_saved_reg_size;

  /* Number of bytes saved on the stack for local variables.  */
  int local_vars_size;

  /* The sum of 2 sizes: locals vars and padding byte for saving the
   * registers.  Used in expand_prologue () and expand_epilogue().  */
  int size_for_adjusting_sp;

  /* True if current function is an interrupt function.  */
  bool interrupt_handler_p;

  /* True if attributes on current function have been checked.  */
  bool attributes_checked_p;
};

/* Zero initialization is OK for all current fields.  */
static struct machine_function *
mrmr16_init_machine_status (void)
{
  return ggc_cleared_alloc<machine_function> ();
}

/* The TARGET_OPTION_OVERRIDE worker.  */
static void
mrmr16_option_override (void)
{
  /* Set the per-function-data initializer.  */
  init_machine_status = mrmr16_init_machine_status;
}

static tree mrmr16_handle_type_attribute (tree *, tree, tree, int, bool *);

/* Defining target-specific uses of __attribute__.  */
static const struct attribute_spec mrmr16_attribute_table[] =
    {
        /* Syntax: { name, min_len, max_len, decl_required, type_required,
           function_type_required, affects_type_identity, handler,
           exclude } */

        /* This attribute generates prologue/epilogue for interrupt handlers.  */
        { "interrupt", 0, 1, false, true, true, false,
          mrmr16_handle_type_attribute, NULL },

        /* The last attribute spec is set to be NULL.  */
        { NULL,	0,  0, false, false, false, false, NULL, NULL }
    };


/* Compute the size of the local area and the size to be adjusted by the
 * prologue and epilogue.  */
static void
mrmr16_compute_frame (void)
{
  /* For aligning the local variables.  */
  int stack_alignment = STACK_BOUNDARY / BITS_PER_UNIT;
  int padding_locals;
  int regno;

  /* Padding needed for each element of the frame.  */
  cfun->machine->local_vars_size = get_frame_size ();

  /* Align to the stack alignment.  */
  padding_locals = cfun->machine->local_vars_size % stack_alignment;
  if (padding_locals)
    padding_locals = stack_alignment - padding_locals;

  cfun->machine->local_vars_size += padding_locals;

  cfun->machine->callee_saved_reg_size = 0;

  /* Save callee-saved registers.  */
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    if (df_regs_ever_live_p (regno) && (! call_used_or_fixed_reg_p (regno)))
      cfun->machine->callee_saved_reg_size += 1;

  cfun->machine->size_for_adjusting_sp =
    crtl->args.pretend_args_size
    + cfun->machine->local_vars_size
    + (ACCUMULATE_OUTGOING_ARGS
       ? (HOST_WIDE_INT) crtl->outgoing_args_size : 0);
}

static void
mrmr16_decrease_sp (rtx insn, int in_frame_build)
{
  rtx num = gen_rtx_CONST_INT(QImode, 1);
  insn = emit_insn (gen_subqi3 (stack_pointer_rtx, stack_pointer_rtx, num));
  if(in_frame_build == 1)
	  RTX_FRAME_RELATED_P (insn) = 1;
}

static void
mrmr16_increase_sp (rtx insn, int in_frame_build)
{
  rtx num = gen_rtx_CONST_INT(QImode, 1);
  insn = emit_insn (gen_addqi3 (stack_pointer_rtx, stack_pointer_rtx, num));
	if(in_frame_build == 1)
	  RTX_FRAME_RELATED_P (insn) = 1;
}

void
mrmr16_push_emit (rtx reg_to_push_from, rtx insn = NULL, int in_frame_build = 0)
{
  rtx reg_sp = gen_rtx_REG (QImode, MRMR16_SP);
  rtx dst = reg_sp;
  rtx src = reg_to_push_from;

  mrmr16_decrease_sp (insn, in_frame_build);

  // if (((GET_CODE (dst) == MEM) &&
    //  ((GET_CODE (XEXP (dst, 0)) == SYMBOL_REF) ||
      // (GET_CODE (XEXP (dst, 0)) == CONST)))    ||
    // ((GET_CODE (src) == MEM) &&
    //  ((GET_CODE (XEXP (src, 0)) == SYMBOL_REF) ||
      // (GET_CODE (XEXP (src, 0)) == CONST))))
  // {
    // 
    // mrmr16_split_symbolic_move (dst, src);
  // }
  // else
  // {
    insn = emit_move_insn (gen_rtx_MEM (QImode, reg_sp), reg_to_push_from);
    //emit_insn (gen_movqi_internal  (gen_rtx_MEM (QImode, reg_sp), reg_to_push_from));
    
  // }
}

void
mrmr16_pop_emit (rtx reg_to_pop_to, rtx insn = NULL, int in_frame_build = 0)
{
  rtx reg_sp = gen_rtx_REG (QImode, MRMR16_SP);
  rtx dst = reg_to_pop_to;
  rtx src = reg_sp;

// if (((GET_CODE (dst) == MEM) &&
//      ((GET_CODE (XEXP (dst, 0)) == SYMBOL_REF) ||
//       (GET_CODE (XEXP (dst, 0)) == CONST)))    ||
//     ((GET_CODE (src) == MEM) &&
//      ((GET_CODE (XEXP (src, 0)) == SYMBOL_REF) ||
//       (GET_CODE (XEXP (src, 0)) == CONST))))
//   {
    
//     mrmr16_split_symbolic_move (dst, src);
//   }
//   else
//   {
    insn = emit_move_insn (reg_to_pop_to, gen_rtx_MEM (QImode, reg_sp));
    //emit_insn (gen_movqi_internal  (reg_to_pop_to, gen_rtx_MEM (QImode, reg_sp)));
  // }

  mrmr16_increase_sp (insn, in_frame_build);
}

void
mrmr16_expand_prologue (void)
{
  int regno;
  rtx insn;

  mrmr16_compute_frame ();

  //mrmr16_decrease_sp(insn);
  //insn = emit_insn (gen_movqi_push (hard_frame_pointer_rtx));
  mrmr16_push_emit(hard_frame_pointer_rtx, insn, 1);
  //RTX_FRAME_RELATED_P (insn) = 1;
  insn = emit_move_insn (hard_frame_pointer_rtx, stack_pointer_rtx);
  RTX_FRAME_RELATED_P (insn) = 1;

  if (flag_stack_usage_info)
    current_function_static_stack_size = cfun->machine->size_for_adjusting_sp;

  /* Save callee-saved registers.  */
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    {
      if (df_regs_ever_live_p (regno)
	  && !call_used_or_fixed_reg_p (regno))
	{
    //mrmr16_decrease_sp(insn);
	  //insn = emit_insn (gen_movqi_push (gen_rtx_REG (Pmode, regno)));
    mrmr16_push_emit(gen_rtx_REG (Pmode, regno), insn, 1);
	  //RTX_FRAME_RELATED_P (insn) = 1;
	}
    }

  if (cfun->machine->size_for_adjusting_sp > 0)
    {
      int i = cfun->machine->size_for_adjusting_sp;
      while ((i >= 255) && (i <= 510))
	{
          rtx reg = gen_rtx_REG (QImode, MRMR16_R5);
          insn = emit_move_insn (reg, GEN_INT (255));
          RTX_FRAME_RELATED_P (insn) = 1;
	  insn = emit_insn (gen_subqi3 (stack_pointer_rtx,
					stack_pointer_rtx,
					reg));
	  RTX_FRAME_RELATED_P (insn) = 1;
	  i -= 255;
	}
      if (i <= 255)
	{
          rtx reg = gen_rtx_REG (QImode, MRMR16_R5);
          insn = emit_move_insn (reg, GEN_INT (i));
          RTX_FRAME_RELATED_P (insn) = 1;
	  insn = emit_insn (gen_subqi3 (stack_pointer_rtx,
					stack_pointer_rtx,
					reg));
	  RTX_FRAME_RELATED_P (insn) = 1;
	}
      else
	{
	  rtx reg = gen_rtx_REG (QImode, MRMR16_R5);
	  insn = emit_move_insn (reg, GEN_INT (i));
	  RTX_FRAME_RELATED_P (insn) = 1;
	  insn = emit_insn (gen_subqi3 (stack_pointer_rtx,
					stack_pointer_rtx,
					reg));
	  RTX_FRAME_RELATED_P (insn) = 1;
	}
    }
}

void
mrmr16_expand_epilogue (void)
{
  int regno;
  rtx reg;
  rtx insn;

  if (cfun->machine->size_for_adjusting_sp > 0)
  {
      int i = cfun->machine->size_for_adjusting_sp;
      while ((i >= 255) && (i <= 510))
      {
          reg = gen_rtx_REG (QImode, MRMR16_R5);
          insn = emit_move_insn (reg, GEN_INT (255));
          RTX_FRAME_RELATED_P (insn) = 1;
	  insn = emit_insn (gen_addqi3 (stack_pointer_rtx,
					stack_pointer_rtx,
					reg));
	  RTX_FRAME_RELATED_P (insn) = 1;
	  i -= 255;
      }
      if (i <= 255)
      {
          reg = gen_rtx_REG (QImode, MRMR16_R5);
          insn = emit_move_insn (reg, GEN_INT (i));
          RTX_FRAME_RELATED_P (insn) = 1;
	  insn = emit_insn (gen_addqi3 (stack_pointer_rtx,
					stack_pointer_rtx,
					reg));
	  RTX_FRAME_RELATED_P (insn) = 1;
      }
      else
      {
	  reg = gen_rtx_REG (QImode, MRMR16_R5);
	  insn = emit_move_insn (reg, GEN_INT (i));
	  RTX_FRAME_RELATED_P (insn) = 1;
	  insn = emit_insn (gen_addqi3 (stack_pointer_rtx,
					stack_pointer_rtx,
					reg));
	  RTX_FRAME_RELATED_P (insn) = 1;
      }
  }

  if (cfun->machine->callee_saved_reg_size != 0)
    {
      reg = gen_rtx_REG (Pmode, MRMR16_R5);
      for (regno = FIRST_PSEUDO_REGISTER; regno-- > 0; )
	if (!call_used_or_fixed_reg_p (regno)
	    && df_regs_ever_live_p (regno))
	  {
	    rtx preg = gen_rtx_REG (Pmode, regno);
	    //emit_insn (gen_movqi_pop (reg, preg));
      mrmr16_pop_emit (preg, insn, 1);
      //mrmr16_increase_sp(insn);
	  }
    }

  emit_move_insn (stack_pointer_rtx, hard_frame_pointer_rtx);
  reg = gen_rtx_REG (Pmode, MRMR16_R5);
  //emit_insn (gen_movqi_pop (reg, hard_frame_pointer_rtx));
  mrmr16_pop_emit (hard_frame_pointer_rtx, insn, 1);
  //mrmr16_increase_sp(insn);

  if (cfun->machine->interrupt_handler_p)
    emit_jump_insn (gen_iret ());
  else
    emit_jump_insn (gen_ret ());
}

/* Implements the macro INITIAL_ELIMINATION_OFFSET, return the OFFSET.  */
int
mrmr16_initial_elimination_offset (int from, int to)
{
  int ret;

  if ((from) == FRAME_POINTER_REGNUM && (to) == HARD_FRAME_POINTER_REGNUM)
    {
      /* Compute this since we need to use cfun->machine->local_vars_size.  */
      mrmr16_compute_frame ();
      ret = -cfun->machine->callee_saved_reg_size;
    }
  else if ((from) == ARG_POINTER_REGNUM && (to) == HARD_FRAME_POINTER_REGNUM)
    {
      ret = 0x00;
    }
  else
    {
      abort ();
    }

  return ret;
}

/* Worker function for TARGET_RETURN_IN_MEMORY.  */
static bool
mrmr16_return_in_memory (const_tree type, const_tree fntype ATTRIBUTE_UNUSED)
{
  const HOST_WIDE_INT size = int_size_in_bytes (type);
  return (size == -1 || size > 1 * UNITS_PER_WORD);
}

/* Return non-zero if the function argument described by ARG is to be
   passed by reference.  */
static bool
mrmr16_pass_by_reference (cumulative_args_t, const function_arg_info &arg)
{
  if (arg.aggregate_type_p ())
    return true;
  unsigned HOST_WIDE_INT size = arg.type_size_in_bytes ();
  return size > (2 * 6);
}

/* Some function arguments will only partially fit in the registers
   that hold arguments.  Given a new arg, return the number of bytes
   that fit in argument passing registers.  */
static int
mrmr16_arg_partial_bytes (cumulative_args_t cum_v, const function_arg_info &arg)
{
  return 0;
}

/* Return the next register to be used to hold a function argument or
   NULL_RTX if there's no more space.  */
static rtx
mrmr16_function_arg (cumulative_args_t cum_v, const function_arg_info &arg)
{
  return NULL_RTX;
}

static bool
mrmr16_must_pass_in_stack (const function_arg_info &arg)
{
  return true;
}

#define MRMR16_FUNCTION_ARG_SIZE(MODE, TYPE)	\
  ((MODE) != BLKmode ? GET_MODE_SIZE (MODE)	\
   : (unsigned) int_size_in_bytes (TYPE))

static void
mrmr16_function_arg_advance (cumulative_args_t cum_v,
                           const function_arg_info &arg)
{
  return;
}

/* Helper function for `mrmr16_legitimate_address_p'.  */
static bool
mrmr16_reg_ok_for_base_p (const_rtx reg, bool strict_p)
{
  int regno = REGNO (reg);

  if (strict_p)
    return HARD_REGNO_OK_FOR_BASE_P (regno)
      || HARD_REGNO_OK_FOR_BASE_P (reg_renumber[regno]);
  else
    return !HARD_REGISTER_NUM_P (regno)
      || HARD_REGNO_OK_FOR_BASE_P (regno);
}

/* Worker function for TARGET_LEGITIMATE_ADDRESS_P.  */
static bool
mrmr16_legitimate_address_p (machine_mode mode ATTRIBUTE_UNUSED,
                           rtx x, bool strict_p,
                           addr_space_t as)
{
  gcc_assert (ADDR_SPACE_GENERIC_P (as));

  if (GET_CODE(x) == PLUS)
      /* && REG_P (XEXP (x, 0)) */
      /* && mrmr16_reg_ok_for_base_p (XEXP (x, 0), strict_p) */
      /* && CONST_INT_P (XEXP (x, 1)) */
      /* && IN_RANGE (INTVAL (XEXP (x, 1)), -32768, 32767)) */
    return false;
  if (REG_P (x) && mrmr16_reg_ok_for_base_p (x, strict_p))
    return true;
  if (GET_CODE (x) == SYMBOL_REF
      || GET_CODE (x) == LABEL_REF
      || GET_CODE (x) == CONST)
    return true;
  return false;
}

/* Worker function for TARGET_SETUP_INCOMING_VARARGS.  */
static void
mrmr16_setup_incoming_varargs (cumulative_args_t cum_v,
                             const function_arg_info &,
                             int *pretend_size, int no_rtl)
{
  CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
  int regno;
  int regs = 6 - *cum;

  *pretend_size = regs < 0 ? 0 : GET_MODE_SIZE (QImode) * regs;

  if (no_rtl)
    return;

  for (regno = *cum; regno < 6; regno++)
    {
      rtx reg = gen_rtx_REG (QImode, regno);
      rtx slot = gen_rtx_PLUS (Pmode,
                               gen_rtx_REG (QImode, ARG_POINTER_REGNUM),
                               GEN_INT (UNITS_PER_WORD * (3 + (regno-2))));

      emit_move_insn (gen_rtx_MEM (QImode, slot), reg);
    }
}

/* Return the fixed registers used for condition codes.  */
static bool
mrmr16_fixed_condition_code_regs (unsigned int *p1, unsigned int *p2)
{
  *p1 = CC_REG;
  *p2 = INVALID_REGNUM;
  return true;
}

/* Define how to find the value returned by a function.
   VALTYPE is the data type of the value (as a tree).
   If the precise function being called is known, FUNC is its
   FUNCTION_DECL; otherwise, FUNC is 0.
   We always return values in register 0 for mrmr16.  */
static rtx
mrmr16_function_value (const_tree valtype,
                     const_tree fntype_or_decl ATTRIBUTE_UNUSED,
                     bool outgoing ATTRIBUTE_UNUSED)
{
  return gen_rtx_REG (TYPE_MODE (valtype), MRMR16_R0);
}

/* Define how to find the value returned by a library function.
   We always return values in register 0 for mrmr16.  */
static rtx
mrmr16_libcall_value (machine_mode mode,
                    const_rtx fun ATTRIBUTE_UNUSED)
{
  return gen_rtx_REG (mode, MRMR16_R0);
}

/* Handle TARGET_FUNCTION_VALUE_REGNO_P.
   We always return values in register 0 for mrmr16.  */
static bool
mrmr16_function_value_regno_p (const unsigned int regno)
{
  return (regno == MRMR16_R0);
}

/* Worker function for TARGET_STATIC_CHAIN.  */
static rtx
mrmr16_static_chain (const_tree ARG_UNUSED (fndecl_or_type), bool incoming_p)
{
  rtx addr, mem;

  if (incoming_p)
    addr = plus_constant (Pmode, arg_pointer_rtx, 1 * UNITS_PER_WORD);
  else
    addr = plus_constant (Pmode, stack_pointer_rtx, -UNITS_PER_WORD);

  mem = gen_rtx_MEM (Pmode, addr);
  MEM_NOTRAP_P (mem) = 1;

  return mem;
}

/* Return true for memory offset addresses between -32768 and 32767.  */
bool
mrmr16_offset_address_p (rtx x)
{
  x = XEXP (x, 0);

  if (GET_CODE (x) == PLUS)
    {
      x = XEXP (x, 1);
      if (GET_CODE (x) == CONST_INT)
	{
	  unsigned int v = INTVAL (x) & 0xFFFF8000;
	  return (v == 0xFFFF8000 || v == 0x00000000);
	}
    }
  return 0;
}

void
mrmr16_split_symbolic_move (rtx dst, rtx src)
{
  if ((GET_CODE (dst) == MEM) &&
      ((GET_CODE (XEXP (dst, 0)) == SYMBOL_REF) ||
       (GET_CODE (XEXP (dst, 0)) == CONST)))
    {
      rtx temp = gen_rtx_REG (QImode, MRMR16_R5);
      emit_move_insn (temp, XEXP (dst, 0));
      emit_move_insn (gen_rtx_MEM (QImode, temp), src);
    }

  if ((GET_CODE (src) == MEM) &&
      ((GET_CODE (XEXP (src, 0)) == SYMBOL_REF) ||
       (GET_CODE (XEXP (src, 0)) == CONST)))
    {
      emit_move_insn (dst, XEXP (src, 0));
      emit_move_insn (dst, gen_rtx_MEM (QImode, dst));
    }
}

/* Verify type based attributes.  NODE is the what the attribute is being
   applied to.  NAME is the attribute name.  ARGS are the attribute args.
   FLAGS gives info about the context.  NO_ADD_ATTRS should be set to true if
   the attribute should be ignored.  */
static tree
mrmr16_handle_type_attribute (tree *node ATTRIBUTE_UNUSED, tree name, tree args,
                            int flags ATTRIBUTE_UNUSED, bool *no_add_attrs)
{
  return NULL_TREE;
}

/* Return true if function TYPE is an interrupt function.  */
static bool
mrmr16_interrupt_type_p (tree type)
{
  return lookup_attribute ("interrupt", TYPE_ATTRIBUTES (type)) != NULL;
}

/* Implement `TARGET_SET_CURRENT_FUNCTION'.  */
/* Sanity checking for function attributes.  */
static void
mrmr16_set_current_function (tree decl)
{
  if (decl == NULL_TREE
      || current_function_decl == NULL_TREE
      || current_function_decl == error_mark_node
      || ! cfun->machine
      || cfun->machine->attributes_checked_p)
    return;

  cfun->machine->interrupt_handler_p
    = mrmr16_interrupt_type_p (TREE_TYPE (decl));

  if (cfun->machine->interrupt_handler_p)
    {
      tree ret = TREE_TYPE (TREE_TYPE (decl));
      tree args = TYPE_ARG_TYPES (TREE_TYPE (decl));

      if (TREE_CODE (ret) != VOID_TYPE)
	error ("%qs function cannot return a value", "interrupt");

      if (args && TREE_CODE (TREE_VALUE (args)) != VOID_TYPE)
	error ("%qs function cannot have arguments", "interrupt");
    }

  /* Don't print the above diagnostics more than once.  */
  cfun->machine->attributes_checked_p = 1;
}

#define LOSE_AND_RETURN(msgid, x)		\
  do						\
    {						\
      mrmr16_operand_lossage (msgid, x);          \
      return;					\
    } while (0)

/* Emit an error message when we're in an asm, and a fatal error for
   "normal" insns.  Formatted output isn't easily implemented, since we
   use output_operand_lossage to output the actual message and handle the
   categorization of the error.  */
static void
mrmr16_operand_lossage (const char *msgid, rtx op)
{
  debug_rtx (op);
  output_operand_lossage ("%s", msgid);
}

/* The PRINT_OPERAND_ADDRESS worker.  */
static void
mrmr16_print_operand_address (FILE *file, machine_mode, rtx x)
{
  switch (GET_CODE (x))
    {
    case REG:
      fprintf (file, "%s", reg_names[REGNO (x)]);
      break;
    default:
      output_addr_const (file, x);
      break;
    }
}

/* The PRINT_OPERAND worker.  */
static void
mrmr16_print_operand (FILE *file, rtx x, int code)
{
  rtx operand = x;

  /* New code entries should just be added to the switch below.  If
     handling is finished, just return.  If handling was just a
     modification of the operand, the modified operand should be put in
     "operand", and then do a break to let default handling
     (zero-modifier) output the operand.  */

  switch (code)
    {
    case 0:
      /* No code, print as usual.  */
      break;

    default:
      LOSE_AND_RETURN ("invalid operand modifier letter", x);
    }

  /* Print an operand as without a modifier letter.  */
  switch (GET_CODE (operand))
    {
    case REG:
      if (REGNO (operand) > MRMR16_FP)
	internal_error ("internal error: bad register: %d", REGNO (operand));
      fprintf (file, "%s", reg_names[REGNO (operand)]);
      return;

    case MEM:
      output_address (GET_MODE (XEXP (operand, 0)), XEXP (operand, 0));
      return;

    default:
      /* No need to handle all strange variants, let output_addr_const
	 do it for us.  */
      if (CONSTANT_P (operand))
	{
	  output_addr_const (file, operand);
	  return;
	}

      LOSE_AND_RETURN ("unexpected operand", x);
    }
}

/* The Global `targetm' Variable.  */

/* Initialize the GCC target structure.  */

#undef TARGET_LRA_P
#define TARGET_LRA_P hook_bool_void_false


/******************************************************************************
 * Run-time Target
 ******************************************************************************/

#undef  TARGET_OPTION_OVERRIDE
#define TARGET_OPTION_OVERRIDE mrmr16_option_override


/******************************************************************************
 * Storage Layout
 ******************************************************************************/

#undef  TARGET_CONSTANT_ALIGNMENT
#define TARGET_CONSTANT_ALIGNMENT constant_alignment_word_strings


/******************************************************************************
 * Stack and Calling
 ******************************************************************************/

/* Frame Registers */

#undef  TARGET_STATIC_CHAIN
#define TARGET_STATIC_CHAIN mrmr16_static_chain

/* Elimination */

#undef  TARGET_FRAME_POINTER_REQUIRED
#define TARGET_FRAME_POINTER_REQUIRED hook_bool_void_true

/* Stack Arguments */

#undef  TARGET_PROMOTE_PROTOTYPES
#define TARGET_PROMOTE_PROTOTYPES hook_bool_const_tree_true

/* Register Arguments */

#undef  TARGET_FUNCTION_ARG
#define TARGET_FUNCTION_ARG         mrmr16_function_arg
#undef  TARGET_MUST_PASS_IN_STACK
#define TARGET_MUST_PASS_IN_STACK   mrmr16_must_pass_in_stack
#undef  TARGET_ARG_PARTIAL_BYTES
#define TARGET_ARG_PARTIAL_BYTES    mrmr16_arg_partial_bytes
#undef  TARGET_PASS_BY_REFERENCE
#define TARGET_PASS_BY_REFERENCE    mrmr16_pass_by_reference
#undef  TARGET_FUNCTION_ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE mrmr16_function_arg_advance

/* Scalar Return */

#undef  TARGET_FUNCTION_VALUE
#define TARGET_FUNCTION_VALUE         mrmr16_function_value
#undef  TARGET_LIBCALL_VALUE
#define TARGET_LIBCALL_VALUE          mrmr16_libcall_value
#undef  TARGET_FUNCTION_VALUE_REGNO_P
#define TARGET_FUNCTION_VALUE_REGNO_P mrmr16_function_value_regno_p

/* Aggregate Return */

#undef  TARGET_RETURN_IN_MEMORY
#define TARGET_RETURN_IN_MEMORY mrmr16_return_in_memory


/******************************************************************************
 * Varargs
 ******************************************************************************/

#undef  TARGET_SETUP_INCOMING_VARARGS
#define TARGET_SETUP_INCOMING_VARARGS mrmr16_setup_incoming_varargs


/******************************************************************************
 * Addressing Modes
 ******************************************************************************/

/* #undef  TARGET_LEGITIMATE_ADDRESS_P */
/* #define TARGET_LEGITIMATE_ADDRESS_P mrmr16_legitimate_address_p */
#undef  TARGET_ADDR_SPACE_LEGITIMATE_ADDRESS_P
#define TARGET_ADDR_SPACE_LEGITIMATE_ADDRESS_P mrmr16_legitimate_address_p


/******************************************************************************
 * Condition Codes
 ******************************************************************************/

#undef	TARGET_FIXED_CONDITION_CODE_REGS
#define	TARGET_FIXED_CONDITION_CODE_REGS mrmr16_fixed_condition_code_regs


/******************************************************************************
 * Assembler Format
 ******************************************************************************/

#undef  TARGET_PRINT_OPERAND
#define TARGET_PRINT_OPERAND mrmr16_print_operand
#undef  TARGET_PRINT_OPERAND_ADDRESS
#define TARGET_PRINT_OPERAND_ADDRESS mrmr16_print_operand_address

/* These are for printing type pseudo-ops, e.g. '.byte', '.short' etc. */
/* #undef TARGET_ASM_BYTE_OP */
/* #define TARGET_ASM_BYTE_OP "\t" */
/* #undef TARGET_ASM_ALIGNED_HI_OP */
/* #define TARGET_ASM_ALIGNED_HI_OP "\t" */
/* #undef TARGET_ASM_ALIGNED_SI_OP */
/* #define TARGET_ASM_ALIGNED_SI_OP "\t" */
/* #undef TARGET_ASM_ALIGNED_DI_OP */
/* #define TARGET_ASM_ALIGNED_DI_OP "\t" */


/******************************************************************************
 * Sections
 ******************************************************************************/

#undef TARGET_ASM_SELECT_SECTION
#define TARGET_ASM_SELECT_SECTION default_elf_select_section


/******************************************************************************
 * Target Attributes
 ******************************************************************************/

#undef TARGET_ATTRIBUTE_TABLE
#define TARGET_ATTRIBUTE_TABLE mrmr16_attribute_table


/******************************************************************************
 * Misc.
 ******************************************************************************/

#undef  TARGET_SET_CURRENT_FUNCTION
#define TARGET_SET_CURRENT_FUNCTION mrmr16_set_current_function


struct gcc_target targetm = TARGET_INITIALIZER;

#include "gt-mrmr16.h"