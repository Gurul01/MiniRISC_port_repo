#ifndef _ELF_MINIRISC_H
#define _ELF_MINIRISC_H

#include "elf/reloc-macros.h"

/*A small subset of supported relocation types*/
START_RELOC_NUMBERS (elf_minirisc_reloc_type)
    RELOC_NUMBER  (R_MINIRISC_NONE,     0)
    RELOC_NUMBER  (R_MINIRISC_32,       1)
    RELOC_NUMBER  (R_MINIRISC_RELATIVE, 2)
END_RELOC_NUMBERS (R_MINIRISC_MAX)

#endif /*_ELF_MINIRISC_H*/