#ifndef __MINIRISC_PROTOS_H__
#define __MINIRISC_PROTOS_H__

/* Messy here. Export target hooks we provide/implement in 'minirisc.h'
   and 'minirisc.c' that need to be globally visible.
   If you provide this file after configure and keep getting the same error, you may want to reconfigure/rebuild */

extern bool minirisc_dummy_insn_cond(machine_mode);
extern enum reg_class minirisc_regno_to_class(int);
extern int minirisc_valid_regno_for_base_p(int);
extern int minirisc_valid_regno_for_index_p(int);
extern void minirisc_init_cumulative_args(CUMULATIVE_ARGS *ca, tree fn_type, rtx libname, tree fn_decl, int num_named);
extern HOST_WIDE_INT minirisc_initial_elimination_offset(int, int);

#endif /*__MINIRISC_PROTOS_H__ */