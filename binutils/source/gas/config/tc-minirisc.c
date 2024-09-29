/* minirisc assembler not functional yet. TODO */
/*Comments for the functions in Part 1b: 13:30*/

#include "as.h"
#include "bfd.h"

const char *md_shortopts = "";
struct option md_longopts [] = {};
size_t md_longopts_size = sizeof(md_longopts);


const char comment_chars[] = "";
const char line_comment_chars[] = "//";
const char line_separator_chars[] = ";";


const char EXP_CHARS[] = "";
const char FLT_CHARS[] = "";


const pseudo_typeS md_pseudo_table[] =
{
  { (char*)0, (void(*)(int))0, 0 }
};


int md_parse_option(int c, const char *arg)
{
 return 0;
}

void md_show_usage(FILE *stream)
{
 printf(stream, "\nMiniRISC options: none available yet\n");
}

void md_begin(void)
{
 return;
}

void md_assemble(char *insn_str)
{
  return;
}

symbolS *md_undefined_symbol(char *name)
{
    return 0;
}

const char *md_atof(int type, char *lit, int *size)
{
    return 0;
}

valueT md_section_align(asection *seg, valueT val)
{
    return 0;
}

void md_convert_frag(bfd *abfd, asection *seg, fragS *fragp)
{
    as_fatal(_("unexpected_call"));
    return;
}

void md_apply_fix(fixS *fixp, valueT *val, segT seg)
{
    return;
}

arelent *tc_gen_reloc(asection *seg, fixS *fixp)
{
    return 0;
}

long md_pcrel_from(fixS *fixp)
{
    as_fatal(_("unexpected_call"));
    return 0;
}

int md_estimate_size_before_relax(fragS *fragp, asection *seg)
{
    as_fatal(_("unexpected_call"));
    return 0;
}