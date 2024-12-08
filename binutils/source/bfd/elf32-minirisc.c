/* TODO: BFD minirisc implementation */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"

/*Include the newly created relocation types*/
#include "elf/minirisc.h"

/*Relocation map struct*/
struct minirisc_relocation_map
{
    bfd_reloc_code_real_type bfd_reloc_val;
    unsigned char elf_reloc_val;
};

/*Actual relocation map*/
static const struct minirisc_relocation_map minirisc_reloc_map[] =
{
    { BFD_RELOC_NONE,              R_MINIRISC_NONE    },
    { BFD_RELOC_8,                 R_MINIRISC_8       },
    { BFD_RELOC_MINIRISC_RELATIVE, R_MINIRISC_RELATIVE},
};


/******************************************************************************/
/*Prototype for custom handling of relative relocations*/
static bfd_reloc_status_type minirisc_elf_relative_reloc(bfd *abfd,
                                                         arelent *r_entry,
                                                         asymbol *sym,
                                                         void *data,
                                                         asection *input_sec,
                                                         bfd *output_bfd,
                                                         char **msg);

/******************************************************************************/

/*A table of howto's*/
static reloc_howto_type minirisc_elf_howto_table[] =
{
    HOWTO (R_MINIRISC_NONE,      // type                            
        0,                       // rightshift   
        1,                       // size(0 = byte, 1 = short, 2 = long)
        8,                       // bitsize   
        false,                   // pc_relative       
        0,                       // bitpos   
        complain_overflow_dont,  // complain_on_overflow                        
        bfd_elf_generic_reloc,   // special_function   
        "R_MINIRISC_NONE",       // name   
        true,                    // partial_inplace       
        0,                       // src_mask   
        0,                       // dst_mask   
        false),                  // pcrel_offset        

    HOWTO (R_MINIRISC_8,             // type                            
        0,                           // rightshift   
        1,                           // size(0 = byte, 1 = short, 2 = long)
        //The reason why this next one is not 8 is to prevent going out of reloc range in case of linking
        11,                          // bitsize    
        false,                       // pc_relative           
        0,                           // bitpos   
        complain_overflow_bitfield,  // complain_on_overflow                                        
        bfd_elf_generic_reloc,       // special_function                       
        "R_MINIRISC_8",              // name                 
        true,                        // partial_inplace           
        0,                           // src_mask   
        0x00ff,                      // dst_mask            
        false),                      // pcrel_offset             

    HOWTO (R_MINIRISC_RELATIVE,     // type                              
        0,                          // rightshift   
        1,                          // size(0 = byte, 1 = short, 2 = long)
        8,                          // bitsize    
        true,                       // pc_relative          
        0,                          // bitpos   
        complain_overflow_bitfield, // complain_on_overflow                                        
        bfd_elf_generic_reloc,      // special_function                       
        "R_LM8_RELATIVE",           // name                   
        true,                       // partial_inplace           
        0xff,                       // src_mask  - not sure about the scr mask          
        0x00ff,                     // dst_mask            
        true),                      // pcrel_offset            
};

/*Without the following functions defined, ld will crash when performing relocations*/
static bool minirisc_info_to_howto_rela(bfd *abfd, arelent *cache_ptr, Elf_Internal_Rela *dst)
{
    unsigned int r_type = ELF32_R_TYPE(dst->r_info);

    if(r_type >= (unsigned int)R_MINIRISC_MAX)
    {
        _bfd_error_handler(_("%p: unsupported relocation type %d"), abfd, r_type);

        bfd_set_error(bfd_error_bad_value);
        return false;
    }

    cache_ptr->howto = &minirisc_elf_howto_table[r_type];
    return true;
}

/*Look up a howto by relocation's type*/
static reloc_howto_type* minirisc_reloc_type_lookup(bfd* abfd, bfd_reloc_code_real_type code)
{
    unsigned int i;
    unsigned int howto_index;

    printf("Looking up a howto by relocation's type: %d\n", code);

    for(i = 0; i < R_MINIRISC_MAX; i++)
    {
        if(code == minirisc_reloc_map[i].bfd_reloc_val)
        {
            howto_index = minirisc_reloc_map[i].elf_reloc_val;
            return &minirisc_elf_howto_table[howto_index];
        }
    }
    return 0;
}

/*Look up a howto by relocation's name*/
static reloc_howto_type* minirisc_reloc_name_lookup(bfd* abfd, const char *name)
{
    unsigned int i;

    printf("Looking up a howto by relocation's name: %s\n", name);

    for(i = 0; i < R_MINIRISC_MAX; i++)
    {
        if((minirisc_elf_howto_table[i].name != 0) && (0 == strcasecmp(minirisc_elf_howto_table[i].name, name)))
        {
            return &minirisc_elf_howto_table[i];
        }
    }
    return 0;
}

/* If we want a custom handling for a particualr relocation type,
   we have to specify a funtion in the howto table.
   Howaver, most things can be handled a generic way.*/
static bfd_reloc_status_type minirisc_elf_relative_reloc(bfd *abfd,
                                                         arelent *r_entry,
                                                         asymbol *sym,
                                                         void *data,
                                                         asection *input_sec,
                                                         bfd *output_bfd,
                                                         char **msg)
{
    return bfd_elf_generic_reloc(abfd, r_entry, sym, data, input_sec, output_bfd, msg);
}

#define elf_info_to_howto               minirisc_info_to_howto_rela
#define elf_info_to_howto_rel           NULL

#define bfd_elf32_bfd_reloc_type_lookup minirisc_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup minirisc_reloc_name_lookup

/*These thing as expected to be provided*/
#define ELF_ARCH         bfd_arch_minirisc
#define ELF_MAXPAGESIZE  0x400
#define ELF_MACHINE_CODE EM_MINIRISC

#undef TARGET_LITTLE_SYM
#define TARGET_LITTLE_SYM minirisc_elf32_vec

#undef TARGET_LITTLE_NAME
#define TARGET_LITTLE_NAME "elf32-minirisc"

/*Has to be at the end*/
#include "elf32-target.h"