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
 #include "langhooks.h"
 #include "gimplify.h"
 #include "builtins.h"
 #include "dumpfile.h"
 #include "hw-doloop.h"
 #include "rtl-iter.h"
 #include "tm-constrs.h"

/* NOTE, this file has to be included after the ones above */
#include "target-def.h"

static function_arg_info arg_info;

/* Forward declaration of hooks we assign/implement here */
static bool minirisc_lra_p(void);
static bool minirisc_legitimate_address_p(machine_mode mode, rtx x, bool strict); //const function_arg_info &arg
static bool minirisc_must_pass_in_stack(const function_arg_info &arg);

static bool minirisc_function_value_regno_p(const unsigned int regno);
static rtx minirisc_function_value(const_tree ret_type, const_tree fn_type, bool out);

static rtx minirisc_function_arg(cumulative_args_t ca, const function_arg_info &arg);
static void chill_function_arg_advance(cumulative_args_t ca, const function_arg_info &arg);

static void minirisc_init_libfuncs(void);

static rtx minirisc_libcall_value(machine_mode mode, const_rtx fun);

static void minirisc_print_operand(FILE *file, rtx op, int letter);
static void minirisc_print_operand_address(FILE *file, machine_mode mode, rtx addr);

/* A set of codegen hooks we implement for our target in this file.
Things that need to be globally visible are additionally exported in 'minirisc-protos.h' */
#undef TARGET_LRA_P
#define TARGET_LRA_P minirisc_lra_p

#undef TARGET_LEGITIMATE_ADDRESS_P
#define TARGET_LEGITIMATE_ADDRESS_P minirisc_legitimate_address_p

#undef TARGET_FUNCTION_VALUE
#define TARGET_FUNCTION_VALUE minirisc_function_value

#undef TARGET_FUNCTION_VALUE_REGNO_P
#define TARGET_FUNCTION_VALUE_REGNO_P minirisc_function_value_regno_p

#undef TARGET_FUNCTION_ARG
#define TARGET_FUNCTION_ARG minirisc_function_arg

#undef TARGET_FUNCTION_ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE chill_function_arg_advance

#undef TARGET_PRINT_OPERAND
#define TARGET_PRINT_OPERAND minirisc_print_operand

#undef TARGET_PRINT_OPERAND_ADDRESS
#define TARGET_PRINT_OPERAND_ADDRESS minirisc_print_operand_address

#undef TARGET_INIT_LIBFUNCS
#define TARGET_INIT_LIBFUNCS minirisc_init_libfuncs

#undef TARGET_LIBCALL_VALUE
#define TARGET_LIBCALL_VALUE minirisc_libcall_value

#undef TARGET_MUST_PASS_IN_STACK
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
/* Need to tell what is considered to be a 'legitimate address'. For now
   we support only basic addressing, which includes registers, constants
   and their combinations. */
/******************************************************************************/
static bool minirisc_legitimate_address_p(machine_mode mode, rtx x, bool strict)
{
    int base_code;
    int off_code;

    if(HImode == mode)
    {
        /* single operand addressing, register or immediate */
        if(REG == GET_CODE(x) || CONST_INT == GET_CODE(x))
        {
            return true;
        }
        else if(PLUS == GET_CODE(x))
        {
            /* base + offset addressing */
            base_code = GET_CODE(XEXP(x, 0));
            off_code  = GET_CODE(XEXP(x, 1));

            if((REG == off_code || CONST_INT == off_code) && (REG == base_code))
            {
                return true;
            }
        }
    }

    return false;
}

/******************************************************************************/
static bool minirisc_must_pass_in_stack(const function_arg_info &arg)
{
    return false;
}

/******************************************************************************/
static bool minirisc_function_value_regno_p(const unsigned int regno)
{
  return (RET_VALUE_REGNUM == regno);
}

/******************************************************************************/
/* Integral types up to 'int' are handled in the SImode. For the rest a dummy
   reg rtx is created. */
/******************************************************************************/
static rtx minirisc_function_value(const_tree ret_type, const_tree fn_type, bool out)
{
    if(INTEGRAL_TYPE_P(ret_type) && TYPE_PRECISION(ret_type) < BITS_PER_WORD)
        return gen_rtx_REG(QImode, RET_VALUE_REGNUM);
    else return gen_rtx_REG(TYPE_MODE(ret_type), RET_VALUE_REGNUM);
}

/******************************************************************************/
static rtx minirisc_function_arg(cumulative_args_t ca, const function_arg_info &arg)
{
    return NULL_RTX;
}

/******************************************************************************/
static void chill_function_arg_advance(cumulative_args_t ca, const function_arg_info &arg)
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
/* Print instruction operands properly */
/******************************************************************************/
static void minirisc_print_operand(FILE *file, rtx op, int letter)
{
    if(op != 0)
    {
        if(REG == GET_CODE(op))
        {
            fprintf(file, "%s", reg_names[REGNO(op)]);
        }
        else if(MEM == GET_CODE(op))
        {
            output_address(GET_MODE(op), XEXP(op, 0));
        }
        else
        {
            /* this includes constants and all other constant
               expressions including labels, symbols and floating
               points as integers */
            output_addr_const(file, op);
        }
    }
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
void minirisc_init_cumulative_args(CUMULATIVE_ARGS *ca, tree fn_type, rtx libname, tree fn_decl, int num_named)
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

/******************************************************************************/

bool minirisc_valid_movsi_insn(machine_mode mode, rtx operands[2])
{
    return true;
}

/******************************************************************************/

void minirisc_expand_movsi(rtx *operands)
{
    if(MEM == GET_CODE(operands[0]) && MEM == GET_CODE(operands[1]))
    {
        operands[1] = force_reg(QImode, operands[1]);
    }

    //emit_insn(gen_movsi_insn(operands[0], operands[1]));
    emit_move_insn(operands[0], operands[1]);
}

/* NOTE, this file is auto-generated in gec/build and has to be
included at the end of this file */
#include "gt-minirisc.h"