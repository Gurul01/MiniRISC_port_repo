/* TODO: BFD minirisc implementation */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"

static reloc_howto_type* minirisc_reloc_type_lookup(bfd* abfd, bfd_reloc_code_real_type code)
{
    return 0;
}

static reloc_howto_type* minirisc_reloc_name_lookup(bfd* abfd, const char *name)
{
    return 0;
}

#define bfd_elf32_bfd_reloc_type_lookup minirisc_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup minirisc_reloc_name_lookup

/*These thing as expected to be provided*/
#define ELF_ARCH         bfd_arch_minirisc
#define ELF_MAXPAGESIZE  0x4000
#define ELF_MACHINE_CODE EM_MINIRISC

#undef TARGET_LITTLE_SYM
#define TARGET_LITTLE_SYM minirisc_elf32_vec

#undef TARGET_LITTLE_NAME
#define TARGET_LITTLE_NAME "elf32-minirisc"

/*Has to be at the end*/
#include "elf32-target.h"