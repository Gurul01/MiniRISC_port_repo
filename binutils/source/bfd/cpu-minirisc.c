#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"

const bfd_arch_info_type bfd_minirisc_arch =
{
    8,


    16,


    16,


    bfd_arch_minirisc,


    bfd_mach_minirisc,


    "minirisc",


    "minirisc",


    1,


    true,


    bfd_default_compatible,


    bfd_default_scan,


    bfd_arch_default_fill,


    NULL,


    0

};