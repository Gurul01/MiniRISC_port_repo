#include "sysdep.h"
#include "opcode/minirisc.h"
#include "disassemble.h"
#include "bfd.h"

#include <stdio.h>
#include <assert.h>

#define MAX_INSN_STRING_LENGTH 14
#define HEX_DATA_STRING_POS 18

static void minirisc_pad_string(char str[HEX_DATA_STRING_POS])
{
    int i;
    int len = strlen(str);

    if(len < HEX_DATA_STRING_POS)
    {
        for(i = len; i < HEX_DATA_STRING_POS; i++)
        {
            str[i] = ' ';
        }
        
        str[HEX_DATA_STRING_POS] = '\0';
    }
}

static void minirisc_decode_insn(char insn_buf[MINIRISC_BYTES_SLOT_INSTRUCTION], minirisc_slot_insn *insn)
{
    assert(insn != 0);

    if(((insn_buf[1] >> 4) & 0xF) == OP_B_TYPE_PREFIX)
    {
        insn->B_type.prefix     = ((insn_buf[1] >> 4) & 0xF);
        insn->B_type.rX_or_ctrl = ((insn_buf[1] >> 0) & 0xF);
        insn->B_type.opcode     = ((insn_buf[0] >> 4) & 0xF);
        insn->B_type.rY_or_ctrl = ((insn_buf[0] >> 0) & 0xF);
    }
    else
    {
        insn->A_type.opcode     = ((insn_buf[1] >> 4) & 0xF);
        insn->A_type.rX_or_ctrl = ((insn_buf[1] >> 0) & 0xF);
        insn->A_type.immed      = ((insn_buf[0] >> 0) & 0xFF);
    }
}

static void minirisc_print_insn(minirisc_slot_insn *insn)
{
    assert(insn != 0);

    int i;
    char insn_buf[HEX_DATA_STRING_POS];
    char OP_string[5] = "";
    unsigned int rX = 0;
    unsigned int rY = 0;
    unsigned int imm = 0;

    if((insn->B_type.prefix & 0xF) == OP_B_TYPE_PREFIX)
    {
        /********************************************************************************************************************************************/
        // B type
        /********************************************************************************************************************************************/
        switch(insn->B_type.opcode)
        {
            case OP_LDSP_LOAD: // LOAD instruction
                
                strcpy(OP_string, "LOAD");
                rX = insn->B_type.rX_or_ctrl;
                rY = insn->B_type.rY_or_ctrl;

                sprintf(insn_buf, "%s r%d r%d", OP_string, rX, rY);
                break;

            case OP_STSP_STORE: //STORE instruction
                
                strcpy(OP_string, "STORE");
                rX = insn->B_type.rX_or_ctrl;
                rY = insn->B_type.rY_or_ctrl;

                sprintf(insn_buf, "%s r%d r%d", OP_string, rY, rX);
                break;

        /* Both addressing modes --------------------------------------------------------*/
            case OP_MOV: if(insn->B_type.opcode == OP_MOV) { strcpy(OP_string, "MOV"); }
            case OP_ADD: if(insn->B_type.opcode == OP_ADD) { strcpy(OP_string, "ADD"); }
            case OP_ADC: if(insn->B_type.opcode == OP_ADC) { strcpy(OP_string, "ADC"); }
            case OP_SUB: if(insn->B_type.opcode == OP_SUB) { strcpy(OP_string, "SUB"); }
            case OP_SBC: if(insn->B_type.opcode == OP_SBC) { strcpy(OP_string, "SBC"); }
            case OP_CMP: if(insn->B_type.opcode == OP_CMP) { strcpy(OP_string, "CMP"); }
            case OP_CMC: if(insn->B_type.opcode == OP_CMC) { strcpy(OP_string, "CMC"); }
            case OP_AND: if(insn->B_type.opcode == OP_AND) { strcpy(OP_string, "AND"); }
            case OP_OR:  if(insn->B_type.opcode == OP_OR)  { strcpy(OP_string, "OR"); }
            case OP_XOR: if(insn->B_type.opcode == OP_XOR) { strcpy(OP_string, "XOR"); }
            case OP_TST: if(insn->B_type.opcode == OP_TST) { strcpy(OP_string, "TST"); }
                
                rX = insn->B_type.rX_or_ctrl;
                rY = insn->B_type.rY_or_ctrl;

                sprintf(insn_buf, "%s r%d r%d", OP_string, rY, rX);
                break;


            /********************************************************/
            /*                     ONE OPERAND                      */
            /********************************************************/

        /* Only one reg as operand --------------------------------------------------------*/
            case OP_SWP_SHIFT:
                if(insn->B_type.rY_or_ctrl == SL0_CTRL) { strcpy(OP_string, "SL0"); }
                if(insn->B_type.rY_or_ctrl == SL1_CTRL) { strcpy(OP_string, "SL1"); }
                if(insn->B_type.rY_or_ctrl == SR0_CTRL) { strcpy(OP_string, "SR0"); }
                if(insn->B_type.rY_or_ctrl == SR1_CTRL) { strcpy(OP_string, "SR1"); }
                if(insn->B_type.rY_or_ctrl == ASR_CTRL) { strcpy(OP_string, "ASR"); }
                if(insn->B_type.rY_or_ctrl == ROL_CTRL) { strcpy(OP_string, "ROL"); }
                if(insn->B_type.rY_or_ctrl == ROR_CTRL) { strcpy(OP_string, "ROR"); }
                if(insn->B_type.rY_or_ctrl == RLC_CTRL) { strcpy(OP_string, "RLC"); }
                if(insn->B_type.rY_or_ctrl == RRC_CTRL) { strcpy(OP_string, "RRC"); }
                
                rX = insn->B_type.rX_or_ctrl;

                sprintf(insn_buf, "%s r%d", OP_string, rX);
                break;

            case OP_CTRL:
        /* Either a reg or an imm --------------------------------------------------------*/
                if(insn->B_type.rX_or_ctrl == JMP_CTRL) { strcpy(OP_string, "JMP"); }
                if(insn->B_type.rX_or_ctrl == JZ_CTRL)  { strcpy(OP_string, "JZ");  }
                if(insn->B_type.rX_or_ctrl == JNZ_CTRL) { strcpy(OP_string, "JNZ"); }
                if(insn->B_type.rX_or_ctrl == JC_CTRL)  { strcpy(OP_string, "JC");  }
                if(insn->B_type.rX_or_ctrl == JNC_CTRL) { strcpy(OP_string, "JNC"); }
                if(insn->B_type.rX_or_ctrl == JN_CTRL)  { strcpy(OP_string, "JN");  }
                if(insn->B_type.rX_or_ctrl == JNN_CTRL) { strcpy(OP_string, "JNN"); }
                if(insn->B_type.rX_or_ctrl == JV_CTRL)  { strcpy(OP_string, "JV");  }
                if(insn->B_type.rX_or_ctrl == JNV_CTRL) { strcpy(OP_string, "JNV"); }
                if(insn->B_type.rX_or_ctrl == JL_CTRL)  { strcpy(OP_string, "JL");  }
                if(insn->B_type.rX_or_ctrl == JLE_CTRL) { strcpy(OP_string, "JLE"); }
                if(insn->B_type.rX_or_ctrl == JH_CTRL)  { strcpy(OP_string, "JH");  }
                if(insn->B_type.rX_or_ctrl == JHE_CTRL) { strcpy(OP_string, "JHE"); }
                if(insn->B_type.rX_or_ctrl == JSE_CTRL) { strcpy(OP_string, "JSE"); }
                if(insn->B_type.rX_or_ctrl == JG_CTRL)  { strcpy(OP_string, "JG");  }
                if(insn->B_type.rX_or_ctrl == JSR_CTRL) { strcpy(OP_string, "JSR"); }
                
                rY = insn->B_type.rY_or_ctrl;

                sprintf(insn_buf, "%s r%d", OP_string, rY);
                break;


            /********************************************************/
            /*                     ZERO OPERAND                     */
            /********************************************************/  

        /* No operand at all --------------------------------------------------------*/
            case OP_DATALESS:
                if(insn->B_type.rX_or_ctrl == RTS_CTRL) { strcpy(OP_string, "RTS"); }
                if(insn->B_type.rX_or_ctrl == RTI_CTRL) { strcpy(OP_string, "RTI"); }
                if(insn->B_type.rX_or_ctrl == CLI_CTRL) { strcpy(OP_string, "CLI"); }
                if(insn->B_type.rX_or_ctrl == STI_CTRL) { strcpy(OP_string, "STI"); }

                sprintf(insn_buf, "%s", OP_string);
                break;

            default:
                printf("Error: B_type opcode unrecognized: %d\n", insn->B_type.opcode);
        }

    }
    else
    {
        /********************************************************************************************************************************************/
        // A type
        /********************************************************************************************************************************************/
        switch (insn->A_type.opcode)
        {
            /********************************************************/
            /*                     TWO OPERANDS                     */
            /********************************************************/

        /* Only one kind of addressing --------------------------------------------------------*/
            case OP_LDSP_LOAD: // LDSP instruction
                
                strcpy(OP_string, "LDSP");
                rX  = insn->A_type.rX_or_ctrl;
                imm = insn->A_type.immed;

                sprintf(insn_buf, "%s r%d %d", OP_string, rX, imm);
                break;

            case OP_STSP_STORE: // STSP instruction

                strcpy(OP_string, "STSP");
                rX  = insn->A_type.rX_or_ctrl;
                imm = insn->A_type.immed;

                sprintf(insn_buf, "%s r%d %d", OP_string, rX, imm);
                break;


        /* Both addressing modes --------------------------------------------------------*/
            case OP_MOV : if(insn->A_type.opcode == OP_MOV) { strcpy(OP_string, "MOV"); }
            case OP_ADD : if(insn->A_type.opcode == OP_ADD) { strcpy(OP_string, "ADD"); }
            case OP_ADC : if(insn->A_type.opcode == OP_ADC) { strcpy(OP_string, "ADC"); }
            case OP_SUB : if(insn->A_type.opcode == OP_SUB) { strcpy(OP_string, "SUB"); }
            case OP_SBC : if(insn->A_type.opcode == OP_SBC) { strcpy(OP_string, "SBC"); }
            case OP_CMP : if(insn->A_type.opcode == OP_CMP) { strcpy(OP_string, "CMP"); }
            case OP_CMC : if(insn->A_type.opcode == OP_CMC) { strcpy(OP_string, "CMC"); }
            case OP_AND : if(insn->A_type.opcode == OP_AND) { strcpy(OP_string, "AND"); }
            case OP_OR  : if(insn->A_type.opcode == OP_OR)  { strcpy(OP_string, "OR");  }
            case OP_XOR : if(insn->A_type.opcode == OP_XOR) { strcpy(OP_string, "XOR"); }
            case OP_TST : if(insn->A_type.opcode == OP_TST) { strcpy(OP_string, "TST"); }

                rX  = insn->A_type.rX_or_ctrl;
                imm = insn->A_type.immed;

                sprintf(insn_buf, "%s r%d %d", OP_string, rX, imm);
                break;


            /********************************************************/
            /*                     ONE OPERAND                      */
            /********************************************************/

        /* Only one reg as operand --------------------------------------------------------*/
            case OP_SWP_SHIFT:

                strcpy(OP_string, "SWP");
                rX  = insn->A_type.rX_or_ctrl;

                sprintf(insn_buf, "%s r%d", OP_string, rX);
                break;


        /* Either a reg or an imm --------------------------------------------------------*/
            case OP_CTRL:
                if(insn->A_type.rX_or_ctrl == JMP_CTRL) { strcpy(OP_string, "JMP"); }
                if(insn->A_type.rX_or_ctrl == JZ_CTRL)  { strcpy(OP_string, "JZ");  }
                if(insn->A_type.rX_or_ctrl == JNZ_CTRL) { strcpy(OP_string, "JNZ"); }
                if(insn->A_type.rX_or_ctrl == JC_CTRL)  { strcpy(OP_string, "JC");  }
                if(insn->A_type.rX_or_ctrl == JNC_CTRL) { strcpy(OP_string, "JNC"); }
                if(insn->A_type.rX_or_ctrl == JN_CTRL)  { strcpy(OP_string, "JN");  }
                if(insn->A_type.rX_or_ctrl == JNN_CTRL) { strcpy(OP_string, "JNN"); }
                if(insn->A_type.rX_or_ctrl == JV_CTRL)  { strcpy(OP_string, "JV");  }
                if(insn->A_type.rX_or_ctrl == JNV_CTRL) { strcpy(OP_string, "JNV"); }
                if(insn->A_type.rX_or_ctrl == JL_CTRL)  { strcpy(OP_string, "JL");  }
                if(insn->A_type.rX_or_ctrl == JLE_CTRL) { strcpy(OP_string, "JLE"); }
                if(insn->A_type.rX_or_ctrl == JH_CTRL)  { strcpy(OP_string, "JH");  }
                if(insn->A_type.rX_or_ctrl == JHE_CTRL) { strcpy(OP_string, "JHE"); }
                if(insn->A_type.rX_or_ctrl == JSE_CTRL) { strcpy(OP_string, "JSE"); }
                if(insn->A_type.rX_or_ctrl == JG_CTRL)  { strcpy(OP_string, "JG");  }
                if(insn->A_type.rX_or_ctrl == JSR_CTRL) { strcpy(OP_string, "JSR"); }

                imm = insn->A_type.immed;

                sprintf(insn_buf, "%s %d", OP_string, imm);
                break;

            default:
                printf("Error: A_type opcode unrecognized: %d\n", insn->A_type.opcode);
        }
    }

    minirisc_pad_string(insn_buf);

    printf("%s", insn_buf);
}


int print_insn_minirisc(bfd_vma addr, disassemble_info *info)
{
    int bytes_read = MINIRISC_BYTES_SLOT_INSTRUCTION;

    char insn_buf[MINIRISC_BYTES_SLOT_INSTRUCTION];
    minirisc_slot_insn insn;

    memset(&insn, 0, sizeof(minirisc_slot_insn));

    if( (*info->read_memory_func)(addr, insn_buf, MINIRISC_BYTES_SLOT_INSTRUCTION, info) )
    {
        (*info->fprintf_func)(info->stream, "Error: attempted to read more instruction bytes (%d) than available (%d)", MINIRISC_BYTES_SLOT_INSTRUCTION, info->buffer_length);
        return -1;
    }

    minirisc_decode_insn(insn_buf, &insn);
    minirisc_print_insn(&insn);

    return bytes_read;
}