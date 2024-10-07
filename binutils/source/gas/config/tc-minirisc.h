#ifndef TC_MINIRISC_H
#define TC_MINIRISC_H

#define TARGET_FORMAT "elf32-minirisc"
#define TARGET_ARCH bfd_arch_minirisc
#define TARGET_MACH bfd_mach_minirisc

/*Littel endian support only*/
#define TARGET_BYTES_BIG_ENDIAN 0

#define md_register_arithmetic 0

#define md_parse_name(str, expr, m, c) minirisc_parse_name(str, expr, c)

/*Customizable handling of expression operands*/
#define md_operand(X)
#define md_number_to_chars number_to_chars_littleendian
#define WORKING_DOT_WORD

/********************************************************/
/*                     TWO OPERANDS                     */
/********************************************************/
/* Only one kind of addressing */
#define LDSP  O_md1   // Only reg - imm  //-----------
#define STSP  O_md2   // Only imm - reg  //-----------

#define LOAD  O_md3 // Only reg - reg    //-----------
#define STORE O_md4 // Only reg - reg    //-----------

/* Both addressing modes *******/
#define MOV  O_md5                       //-----------

#define ADD  O_add                       //-REUSE
#define ADC  O_md6                       //-----------
#define SUB  O_subtract                  //-REUSE
#define SBC  O_md7                       //-----------
#define CMP  O_md8                       //-----------
#define CMC  O_md9                       //-----------

#define AND  O_bit_and                   //-REUSE
#define OR   O_bit_inclusive_or          //-REUSE
#define XOR  O_bit_exclusive_or          //-REUSE
#define TST  O_md10                      //-----------

/********************************************************/
/*                     ONE OPERAND                      */
/********************************************************/
/* Only one reg as operand ******/
#define SWP  O_md11                      //-----------
#define SL0  O_left_shift                //-REUSE
#define SL1  O_md12                      //-----------
#define SR0  O_right_shift               //-REUSE
#define SR1  O_md13                      //-----------
#define ASR  O_md14                      //-----------
#define ROL  O_md15                      //-----------
#define ROR  O_md16                      //-----------
#define RLC  O_md17                      //-----------
#define RRC  O_md18                      //-----------

/* Either a reg or an imm ******/
#define JMP  O_md19                      //-----------
#define JZ   O_eq                        //-REUSE
#define JNZ  O_ne                        //-REUSE
#define JC   O_lt                        //-REUSE
#define JNC  O_ge                        //-REUSE
#define JN   O_md20                      //-----------
#define JNN  O_md21                      //-----------
#define JV   O_md22                      //-----------
#define JNV  O_md23                      //-----------
#define JL   O_md24                      //-----------
#define JLE  O_md25                      //-----------
#define JH   O_md26                      //-----------
#define JHE  O_md27                      //-----------
#define JSE  O_le                        //-REUSE
#define JG   O_gt                        //-REUSE
                    
#define JSR  O_md28                      //-----------               

/********************************************************/
/*                     ZERO OPERAND                     */
/********************************************************/        
/* No operand at all ***********/
#define RTS  O_md29                      //-----------
#define RTI  O_md30                      //-----------
#define STI  O_md31                      //-----------
#define CLI  O_md32                      //-----------



#endif