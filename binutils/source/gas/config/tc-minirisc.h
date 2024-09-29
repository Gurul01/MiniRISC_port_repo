#ifndef TC_MINIRISC_H
#define TC_MINIRISC_H

#define TARGET_FORMAT "elf32-minirisc"
#define TARGET_ARCH bfd_arch_minirisc
#define TARGET_MACH bfd_mach_minirisc

/*Littel endian support only*/
#define TARGET_BYTES_BIG_ENDIAN 0

/*Customizable handling of expression operands*/
#define md_operand(X)

#define md_number_to_chars number_to_chars_littleendian

#define WORKING_DOT_WORD

#endif