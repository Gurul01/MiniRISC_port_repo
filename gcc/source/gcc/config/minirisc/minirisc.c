#define IN_TARGET_CODE 1

/* Here is a list of includes we do not need all of them yet,
   but we provide it anyway for future extensions. */

 #include "config.h"
 #include "system.h"
 #include "coretypes.h"
 #include "backend.h"
 #include "target.h"
 #include "rtl.h"
 #include "tree.h"
 #include "gimple.h"
 #include "cfghooks.h"
 #include "df.h"
 #include "memmodel.h"
 #include "tm_p.h"
 #include "stringpool.h"
 #include "attribs.h"
 #include "optabs.h"
 #include "regs.h"
 #include "emit-rtl.h"
 #include "recog.h"
 #include "diagnostic-core.h"
 #include "cfgrtl.h"
 #include "output.h"
 #include "fold-const.h"
 #include "stor-layout.h"
 #include "calls.h"
 #include "varasm.h"
 #include "alias.h"
 #include "explow.h"
 #include "expr.h"
 #include "reload.h"
 #include "Langhooks.h"
 #include "gimplify.h"
 #include "builtins.h"
 #include "dumpfile.h"
 #include "hw-doloop.h"
 #include "rtl-iter.h"
 #include "tm-constrs.h"

/* NOTE, this file has to be included after the ones above */
#include "target-def.h"

/* Forward declaration of hooks we assign/implement here */
static bool minirisc_lra_p(void);
static bool minirisc_legitimate_address_p(machine_mode mode, rtx x, bool strict);
static bool minirisc_must_pass_in_stack(machine_mode mode, const_tree type);

static bool minirisc_function_value_regno_p(const unsigned int regno);
static rtx minirisc_function_value(const_tree ret_type, const_tree fn_type, bool out);

static rtx minirisc_function_arg(cumulative_args_t ca, machine_mode mode, const_tree type, bool named);
static void chill_function_arg_advance(cumulative_args_t ca, machine_mode mode, const_tree type, bool named);

static void minirisc_init_libfuncs(void);

static rtx minirisc_libcall_value(machine_mode mode, const_rtx fun);

static void minirisc_print_operand(FILE *file, rtx op, int letter);
static void minirisc_print_operand_address(FILE *file, machine_mode mode, rtx addr);

/* A set of codegen hooks we implement for our target in this file.
Things that need to be globally visible are additionally exported in 'minirisc-protos.h' */
#undef TARGET LRA P
#define TARGET_LRA_P minirisc_lra_p

#undef TARGET_LEGITIMATE_ADDRESS_P
#define TARGET_LEGITIMATE_ADDRESS_P minirisc_legitimate_address_p

#undef TARGET_FUNCTION_ VALUE
#define TARGET_FUNCTION_VALUE minirisc_function_value

#undef TARGET_FUNCTION_VALUE_REGNO_P
#define TARGET_ FUNCTION_VALUE_REGNO_P minirisc_function_value_regno_p

#undef TARGET_FUNCTION_ARG
#define TARGET_FUNCTIONARG minirisc_function_arg

#undef TARGET_FUNCTION ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE chill_function_arg_advance

#undef TARGET_PRINT_OPERAND
#define TARGET_PRINT_OPERAND minirisc_print_operand

#undef TARGET PRINT OPERAND ADDRESS
#define TARGET_PRINT_OPERAND_ADDRESS minirisc_print_operand_address

#undef TARGET_INIT_LIBFUNCS
#define TARGET_INIT_LIBFUNCS minirisc_init_libfuncs

#undef TARGET_ LIBCALL_VALUE
#define TARGET_LIBCALL_VALUE minirisc_libcall_value

#undef TARGET MUST PASS IN STACK
#define TARGET_MUST_PASS_IN_STACK minirisc_must_pass_in_stack

/* NOTE, we also need to provide a struct for machine functions,
the compilation breaks without it */
struct GTY(()) machine_function
{
  int reg_val;
};

struct gcc_target targetm = TARGET_INITIALIZER;

/******************************************************************************/
/* Now implement the hooks we have assigned above                             */
/******************************************************************************/
static bool minirisc_lra_p(void)
{
    /* stick to the old reloader for now */
    return false;
}

/******************************************************************************/
static bool minirisc_legitimate_address_p(machine_mode mode, rtx x, bool strict)
{
    return true;
}

/******************************************************************************/
static bool minirisc_must_pass_in_stack(machine_mode mode, const_tree type)
{
    return false;
}

/******************************************************************************/
static bool minirisc_function_value_regno_p(const unsigned int regno)
{
  return (RET_VALUE_REGNUM == regno);
}

/******************************************************************************/
static rtx minirisc_function_value(const_tree ret_type, const_tree fn_type, bool out)
{
    return NULL_RTX;
}

/******************************************************************************/
static rtx minirisc_function_arg(cumulative_args_t ca, machine_mode mode, const_tree type, bool named)
{
    return NULL_RTX;
}

/******************************************************************************/
static void chill_function_arg_advance(cumulative_args_t ca, machine_mode mode, const_tree type, bool named)
{
    return;
}

/******************************************************************************/
static void minirisc_init_libfuncs(void)
{
    return;
}

/******************************************************************************/
static rtx minirisc_libcall_value(machine_mode mode, const_rtx fun)
{
    return NULL_RTX;
}

/******************************************************************************/
static void minirisc_print_operand(FILE *file, rtx op, int letter)
{
    return;
}

/******************************************************************************/
static void minirisc_print_operand_address(FILE *file, machine_mode mode, rtx addr)
{
    return;
}

/******************************************************************************/
/* Now implement hooks we assign in 'minirisc.h'                                 */
/******************************************************************************/
int minirisc_valid_regno_for_base_p(int regno)
{
    return 1;
}

/******************************************************************************/
int minirisc_valid_regno_for_index_p(int regno)
{
    return minirisc_valid_regno_for_base_p(regno);
}

/******************************************************************************/
enum reg_class minirisc_regno_to_class(int regno)
{
    return reg_class::GP_REGS;
}

/******************************************************************************/
HOST_WIDE_INT minirisc_initial_elimination_offset(int from, int to)
{
    return 0;
}

/******************************************************************************/
minirisc_init_cumulative_args(CUMULATIVE_ARGS *ca, tree fn_type, rtx libname, tree fn_decl, int num_named)
{
    return;
}

/******************************************************************************/
/* Implement the dummy condition we use in minirisc.md                        */
/******************************************************************************/
bool minirisc_dummy_insn_cond(machine_mode mode)
{
    return true;
}

/* NOTE, this file is auto-generated in gec/build and has to be
included at the end of this file */
#include "gt-minirisc.h"