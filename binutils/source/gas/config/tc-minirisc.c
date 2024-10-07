/* minirisc assembler not functional yet. TODO */
/*Comments for the functions in Part 1b: 13:30*/

#include "as.h"
#include "bfd.h"
#include "opcode/minirisc.h"

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

//TODO:
typedef struct reg_type{
    char name[4];
    uint8_t number;
} reg_type;
reg_type reg_array[16];

htab_t reg_table; //struct hash_control *reg_table;

int md_parse_option(int c, const char *arg)
{
 return 0;
}

void md_show_usage(FILE *stream)
{
 printf(stream, "\nMiniRISC options: none available yet\n");
}

static hashval_t hash_func (const void *entry)
{
    return htab_hash_string ((const char *)entry);
}

static int eq_func (const void *entry1, const void *entry2)
{
    return strcmp ((const char *)entry1, (const char *)entry2) == 0;
}

/*************************************************************/
/*   Check if the name is register (r0-r15)                  */
/*************************************************************/
static int minirisc_parse_register(const char *name, expressionS *resultP)
{
    // symbolS *reg_sym = hash_find(reg_table, name); //bfd_hash_lookup

    // if(reg_sym != 0)
    // {
    //     resultP->X_op = O_register;
    //     resultP->X_add_number = S_GET_VALUE(reg_sym);
    //     return 1;
    // }

    // Lookup in the hash table
    // void **slot = htab_find_slot (reg_table, name, NO_INSERT);
    // if (slot && *slot != NULL)
    // {
    //     symbolS *reg_sym = (symbolS *) *slot;

    //     resultP->X_op = O_register;
    //     resultP->X_add_number = S_GET_VALUE(reg_sym);

    //     printf("Reached_parse_reg: %s\n", name);
    //     return 1;
    // }

    for(int i = 0; i < MINIRISC_NUM_REGISTERS; i++)
    {
        if(0 == strcmp(name, reg_array[i].name))
        {
            resultP->X_op = O_register;
            resultP->X_add_number = reg_array[i].number;

            printf("Reached_parse_reg: %s\n", name);
            return 1;
        }
    }

    return 0;
}

/*************************************************************/
/*   Catch 'jump'     */
/*************************************************************/
// static int minirisc_parse_branch(const char *name, expressionS *resultP, char *next_char)
// {
//     expressionS op_expr;

//     if( strcmp(name, "jmp") )
//         return 0;

//     (void) restore_line_pointer(*next_char);
//     SKIP_WHITESPACE();

//     if('(' != *input_line_pointer)
//         as_warn("no '(' after jmp");

//     memset(&op_expr, 0, sizeof(expressionS));
//     expression(&op_expr);

//     resultP->X_add_symbol = make_expr_symbol(&op_expr);
//     resultP->X_op = O_jump;

//     *next_char = *input_line_pointer;
//     symbol_get_value_expression
//     return 1;
// }

/*************************************************************/
/*   If non of the others is true then we have a symbol      */
/*     (we assume)                                           */
/*************************************************************/
static int minirisc_parse_symbol(const char *name, expressionS *resultP)
{
    symbolS *sym = symbol_find_or_make(name);
    know(sym != 0);

    resultP->X_op = O_symbol;
    resultP->X_add_symbol = sym;

    /* A hack to avoid 'make_expr_symbol' turn the node into 'O_constant' */
    resultP->X_add_number = 1;

     printf("Reached_parse_symbol: %s\n", name);
    return 1;
}

static int minirisc_parse_opcode(const char *name, expressionS *resultP, char *next_char)
{
    operatorT opcode;

    expressionS *first_operand;
    expressionS *second_operand;
   
    gas_assert(name != 0 && resultP != 0);

    if(      0 == strcmp(name, "LDSP") || 0 == strcmp(name, "ldsp"))
    {
        opcode = LDSP;
    }
    else if( 0 == strcmp(name, "STSP") || 0 == strcmp(name, "stsp"))
    {
        opcode = STSP;
    }
    else if( 0 == strcmp(name, "LOAD") || 0 == strcmp(name, "load"))
    {
        opcode = LOAD;
    }
    else if( 0 == strcmp(name, "STORE") || 0 == strcmp(name, "store"))
    {
        opcode = STORE;
    }
    else if( 0 == strcmp(name, "MOV")  || 0 == strcmp(name, "mov"))
    {
        opcode = MOV;
    }
    else if( 0 == strcmp(name, "ADD")  || 0 == strcmp(name, "add"))
    {
        opcode = ADD;
    }
    else if( 0 == strcmp(name, "ADC")  || 0 == strcmp(name, "adc"))
    {
        opcode = ADC;
    }
    else if( 0 == strcmp(name, "SUB")  || 0 == strcmp(name, "sub"))
    {
        opcode = SUB;
    }
    else if( 0 == strcmp(name, "CMC")  || 0 == strcmp(name, "cmc"))
    {
        opcode = CMC;
    }
    else if( 0 == strcmp(name, "AND")  || 0 == strcmp(name, "and"))
    {
        opcode = AND;
    }
    else if( 0 == strcmp(name, "OR")  || 0 == strcmp(name, "or"))
    {
        opcode = OR;
    }
    else if( 0 == strcmp(name, "XOR")  || 0 == strcmp(name, "xor"))
    {
        opcode = XOR;
    }
    else if( 0 == strcmp(name, "TST")  || 0 == strcmp(name, "tst"))
    {
        opcode = TST;
    }
    else if( 0 == strcmp(name, "SWP")  || 0 == strcmp(name, "swp"))
    {
        opcode = SWP;
    }
    else if( 0 == strcmp(name, "SL0")  || 0 == strcmp(name, "sl0"))
    {
        opcode = SL0;
    }
    else if( 0 == strcmp(name, "SL1")  || 0 == strcmp(name, "sl1"))
    {
        opcode = SL1;
    }
    else if( 0 == strcmp(name, "SR0")  || 0 == strcmp(name, "sr0"))
    {
        opcode = SR0;
    }
    else if( 0 == strcmp(name, "SR1")  || 0 == strcmp(name, "sr1"))
    {
        opcode = SR1;
    }
    else if( 0 == strcmp(name, "ASR")  || 0 == strcmp(name, "asr"))
    {
        opcode = ASR;
    }
    else if( 0 == strcmp(name, "ROL")  || 0 == strcmp(name, "rol"))
    {
        opcode = ROL;
    }
    else if( 0 == strcmp(name, "ROR")  || 0 == strcmp(name, "ror"))
    {
        opcode = ROR;
    }
    else if( 0 == strcmp(name, "RLC")  || 0 == strcmp(name, "rlc"))
    {
        opcode = RLC;
    }
    else if( 0 == strcmp(name, "RRC")  || 0 == strcmp(name, "rrc"))
    {
        opcode = RRC;
    }
    else if( 0 == strcmp(name, "JMP")  || 0 == strcmp(name, "jmp"))
    {
        opcode = JMP;
    }
    else if( 0 == strcmp(name, "JZ")  || 0 == strcmp(name, "jz"))
    {
        opcode = JZ;
    }
    else if( 0 == strcmp(name, "JNZ")  || 0 == strcmp(name, "jnz"))
    {
        opcode = JNZ;
    }
    else if( 0 == strcmp(name, "JC")  || 0 == strcmp(name, "jc"))
    {
        opcode = JC;
    }
    else if( 0 == strcmp(name, "JNC")  || 0 == strcmp(name, "jnc"))
    {
        opcode = JNC;
    }
    else if( 0 == strcmp(name, "JN")  || 0 == strcmp(name, "jn"))
    {
        opcode = JN;
    }
    else if( 0 == strcmp(name, "JNN")  || 0 == strcmp(name, "jnn"))
    {
        opcode = JNN;
    }
    else if( 0 == strcmp(name, "JV")  || 0 == strcmp(name, "jv"))
    {
        opcode = JV;
    }
    else if( 0 == strcmp(name, "JNV")  || 0 == strcmp(name, "jnv"))
    {
        opcode = JNV;
    }
    else if( 0 == strcmp(name, "JL")  || 0 == strcmp(name, "jl"))
    {
        opcode = JL;
    }
    else if( 0 == strcmp(name, "JLE")  || 0 == strcmp(name, "jle"))
    {
        opcode = JLE;
    }
    else if( 0 == strcmp(name, "JH")  || 0 == strcmp(name, "jh"))
    {
        opcode = JH;
    }
    else if( 0 == strcmp(name, "JHE")  || 0 == strcmp(name, "jhe"))
    {
        opcode = JHE;
    }
    else if( 0 == strcmp(name, "JSE")  || 0 == strcmp(name, "jse"))
    {
        opcode = JSE;
    }
    else if( 0 == strcmp(name, "JG")  || 0 == strcmp(name, "jg"))
    {
        opcode = JG;
    }
    else if( 0 == strcmp(name, "JSR")  || 0 == strcmp(name, "jsr"))
    {
        opcode = JSR;
    }
    else if( 0 == strcmp(name, "RTS")  || 0 == strcmp(name, "rts"))
    {
        opcode = RTS;
    }
    else if( 0 == strcmp(name, "RTI")  || 0 == strcmp(name, "rti"))
    {
        opcode = RTI;
    }
    else if( 0 == strcmp(name, "STI")  || 0 == strcmp(name, "sti"))
    {
        opcode = STI;
    }
    else if( 0 == strcmp(name, "CLI")  || 0 == strcmp(name, "cli"))
    {
        opcode = CLI;
    }
    else
    {
        opcode = O_absent;
    }

    /* Setting the opcode for the instruction build */
    resultP->X_op = opcode;

    // Filling up the operands for the given instruction
    // Here the type of the operands reg/imm/memaddr is not relevant
    switch(opcode)
    {
        // The instructions with two operands
        case LDSP:
        case STSP:
        case LOAD:
        case STORE:
        case MOV:
        case ADD:
        case ADC:
        case SUB:
        case SBC:
        case CMP:
        case CMC:
        case AND:
        case OR:
        case XOR:
        case TST:
            (void) restore_line_pointer(*next_char);
            //input_line_pointer = next_char;

            SKIP_ALL_WHITESPACE();
            first_operand = (expressionS*)malloc(sizeof(expressionS));
            memset(first_operand, 0, sizeof(expressionS));
            expression(first_operand);
            resultP->X_add_symbol = make_expr_symbol(first_operand);

            SKIP_ALL_WHITESPACE();
            second_operand = (expressionS*)malloc(sizeof(expressionS));
            memset(second_operand, 0, sizeof(expressionS));
            expression(second_operand);
            resultP->X_op_symbol = make_expr_symbol(second_operand);

            // Dummy
            resultP->X_add_number = 1;

            // while(*input_line_pointer != '\n')
            // {
            //     input_line_pointer++;
            // }
            // input_line_pointer++;
            *next_char = *input_line_pointer; //next_char = input_line_pointer;
            break;

        // The instructions with one operand
        case SWP:
        case SL0:
        case SL1:
        case SR0:
        case SR1:
        case ASR:
        case ROL:
        case ROR:
        case RLC:
        case RRC:
        case JMP:
        case JZ: 
        case JNZ:
        case JC: 
        case JNC:
        case JN: 
        case JNN:
        case JV: 
        case JNV:
        case JL: 
        case JLE:
        case JH: 
        case JHE:
        case JSE:
        case JG: 
        case JSR:
            (void) restore_line_pointer(*next_char);
            //input_line_pointer = next_char;

            SKIP_ALL_WHITESPACE();
            first_operand = (expressionS*)malloc(sizeof(expressionS));
            memset(first_operand, 0, sizeof(expressionS));
            expression(first_operand);
            resultP->X_add_symbol = make_expr_symbol(first_operand);

            // Dummy
            //resultP->X_op_symbol  = 0;
            resultP->X_add_number = 1;

            // while(*input_line_pointer != '\n')
            // {
            //     input_line_pointer++;
            // }   
            // input_line_pointer++;
            *next_char = *input_line_pointer; //next_char = input_line_pointer;
            break;

        // The instructions with zero operands
        case RTS:
        case RTI:
        case STI:
        case CLI:
            // Dummy
            //resultP->X_add_symbol = 0;
            //resultP->X_op_symbol  = 0;
            resultP->X_add_number = 1;

            break;

        default:
            printf("Couldn't find symbol with strcmp!");
            return 0;
    }

    printf("Reached_parse_opcode: %d\n", opcode);

    return 1;
}

/*************************************************************/
/*   Catch assembler keywords, hook custom name checking     */
/*************************************************************/
// int minirisc_parse_name(const char *name, expressionS *resultP, char *next_char)
// {
//     gas_assert(name != 0 && resultP != 0);

//     if(minirisc_parse_register(name, resultP) != 0)
//         return 1;
//     /* if(minirisc_parse_port(name, resultP) != 0)
//         return 1; */
//     if(minirisc_parse_branch(name, resultP, next_char) != 0)
//         return 1;
//     if(minirisc_parse_symbol(name, resultP) != 0)
//         return 1;

//     return 0;
// }
int minirisc_parse_name(const char *name, expressionS *resultP, char *next_char)
{
    gas_assert(name != 0 && resultP != 0);

    if(minirisc_parse_register(name, resultP) != 0)
        return 1;

    if(minirisc_parse_opcode(name, resultP, next_char) != 0)
        return 1;

    if(minirisc_parse_symbol(name, resultP) != 0)
        return 1;

    return 0;
}

/*************************************************************/
/*   Arch specific init                                      */
/*   Setting up the symbols for the reg table                */
/*************************************************************/

void md_begin(void)
{
    int i;
    reg_table = htab_create (MINIRISC_NUM_REGISTERS, hash_func, eq_func, NULL);//reg_table = hash_new();

    // for(i = 0; i < MINIRISC_NUM_REGISTERS; i++)
    // {
    //     char name[4];

    //     sprintf(name, "r%d", i);

    //     symbolS *reg_sym = symbol_new(name, reg_section, i, &zero_address_frag);

    //     if(hash_insert(reg_table, S_GET_NAME(reg_sym), (PTR)reg_sym))
    //     {
    //         as_fatal(_("failed to create register symbols"));
    //     }
    // }

    for (i = 0; i < MINIRISC_NUM_REGISTERS; i++)
    {
        // char name[4];
        // sprintf(name, "r%d", i);

        // // Create a new symbol for each register
        // symbolS *reg_sym = symbol_new (name, reg_section, &zero_address_frag, i);

        // // Insert the symbol into the hash table
        // void **slot = htab_find_slot (reg_table, S_GET_NAME(reg_sym), INSERT);
        // if (slot == NULL || *slot != NULL)
        // {
        //     as_fatal (_("failed to create register symbols"));
        // }

        // *slot = (void *)reg_sym;

        reg_array[i].number = i;
        sprintf(reg_array[i].name, "r%d", i);
    }
}

static void minirisc_emit_insn(minirisc_slot_insn *insn)
{
    gas_assert(insn != 0);

    uint16_t e_insn = 0;
    char *frag = frag_more(MINIRISC_BYTES_SLOT_INSTRUCTION);

    if((insn->B_type.prefix & 0xF) == OP_B_TYPE_PREFIX)
    {
        e_insn |= ((insn->B_type.prefix & 0xF)     << (4*3));
        e_insn |= ((insn->B_type.rX_or_ctrl & 0xF) << (4*2));
        e_insn |= ((insn->B_type.opcode & 0xF)     << (4*1));
        e_insn |= ((insn->B_type.rY_or_ctrl & 0xF) << (4*0));
    }
    else
    {
        e_insn |= ((insn->A_type.opcode & 0xF)     << (4*3));
        e_insn |= ((insn->A_type.rX_or_ctrl & 0xF) << (4*2));
        e_insn |= ((insn->A_type.immed & 0xFF)     << (4*0));
    }

     printf("Reached_4: num0: %d\n", *insn);
     printf("Reached_4: num: %d\n", e_insn);

    md_number_to_chars(frag, e_insn, MINIRISC_BYTES_SLOT_INSTRUCTION);
}

/**************************************************************/
/* Main assemble hook, called once for each instracion string */
/**************************************************************/
void md_assemble(char *insn_str)
{
    printf("Assembling instruction string: %s\n", insn_str);

    minirisc_slot_insn *insn;

    expressionS *whole_instr;
    expressionS *first_op;
    expressionS *second_op;

    operatorT opcode;
    uint8_t ctrl_field;

    int error = 0;

    // int ins_done = 0;
    // int assign_pos;
    // char *assign;
    insn        = (minirisc_slot_insn*)malloc(sizeof(minirisc_insn_A_type));
    whole_instr = (expressionS*)malloc(sizeof(expressionS));

    input_line_pointer = insn_str;
    SKIP_ALL_WHITESPACE();
    memset(whole_instr, 0, sizeof(expressionS));
    expression(whole_instr);

    switch(whole_instr->X_op)
    {
        /********************************************************/
        /*                     TWO OPERANDS                     */
        /********************************************************/

    /* Only one kind of addressing --------------------------------------------------------*/
        case LDSP:
            gas_assert((whole_instr->X_add_symbol != 0) && (whole_instr->X_op_symbol != 0));

            first_op = symbol_get_value_expression(whole_instr->X_add_symbol);
            second_op = symbol_get_value_expression(whole_instr->X_op_symbol);

            if((first_op->X_op != O_register) || (second_op->X_op != O_constant))
            {
                as_bad("LDSP usage: first op has to be reg, second op has to be const");
                error = 1;
            }

            insn->A_type.opcode = OP_LDSP_LOAD;
            insn->A_type.rX_or_ctrl = first_op->X_add_number;
            insn->A_type.immed = second_op->X_add_number;

            break;

        case STSP:
            gas_assert((whole_instr->X_add_symbol != 0) && (whole_instr->X_op_symbol != 0));

            first_op = symbol_get_value_expression(whole_instr->X_add_symbol);
            second_op = symbol_get_value_expression(whole_instr->X_op_symbol);

            if((first_op->X_op != O_constant) || (second_op->X_op != O_register))
            {
                as_bad("STSP usage: first op has to be const, second op has to be reg");
                error = 1;
            }

            insn->A_type.opcode = OP_STSP_STORE;
            insn->A_type.rX_or_ctrl = second_op->X_add_number;
            insn->A_type.immed = first_op->X_add_number;

            break;

        case LOAD:
            gas_assert((whole_instr->X_add_symbol != 0) && (whole_instr->X_op_symbol != 0));

            first_op = symbol_get_value_expression(whole_instr->X_add_symbol);
            second_op = symbol_get_value_expression(whole_instr->X_op_symbol);

            if((first_op->X_op != O_register) || (second_op->X_op != O_register))
            {
                as_bad("LOAD usage: first op has to be reg, second op has to be reg");
                error = 1;
            }

            insn->B_type.prefix = OP_B_TYPE_PREFIX;
            insn->B_type.rX_or_ctrl = first_op->X_add_number;
            insn->B_type.opcode = OP_LDSP_LOAD;
            insn->B_type.rY_or_ctrl = second_op->X_add_number;

            break;
        case STORE:
                        gas_assert((whole_instr->X_add_symbol != 0) && (whole_instr->X_op_symbol != 0));

            first_op = symbol_get_value_expression(whole_instr->X_add_symbol);
            second_op = symbol_get_value_expression(whole_instr->X_op_symbol);

            if((first_op->X_op != O_register) || (second_op->X_op != O_register))
            {
                as_bad("STORE usage: first op has to be reg, second op has to be reg");
                error = 1;
            }

            insn->B_type.prefix = OP_B_TYPE_PREFIX;
            insn->B_type.rX_or_ctrl = second_op->X_add_number;
            insn->B_type.opcode = OP_STSP_STORE;
            insn->B_type.rY_or_ctrl = first_op->X_add_number;

            break;

    /* Both addressing modes --------------------------------------------------------*/
        case MOV: if(whole_instr->X_op == MOV) { opcode = OP_MOV; }
        case ADD: if(whole_instr->X_op == ADD) { opcode = OP_ADD; }
        case ADC: if(whole_instr->X_op == ADC) { opcode = OP_ADC; }
        case SUB: if(whole_instr->X_op == SUB) { opcode = OP_SUB; }
        case SBC: if(whole_instr->X_op == SBC) { opcode = OP_SBC; }
        case CMP: if(whole_instr->X_op == CMP) { opcode = OP_CMP; }
        case CMC: if(whole_instr->X_op == CMC) { opcode = OP_CMC; }
        case AND: if(whole_instr->X_op == AND) { opcode = OP_AND; }
        case OR:  if(whole_instr->X_op == OR)  { opcode = OP_OR;  }
        case XOR: if(whole_instr->X_op == XOR) { opcode = OP_XOR; }
        case TST: if(whole_instr->X_op == TST) { opcode = OP_TST; }
            gas_assert((whole_instr->X_add_symbol != 0) && (whole_instr->X_op_symbol != 0));

            first_op = symbol_get_value_expression(whole_instr->X_add_symbol);
            second_op = symbol_get_value_expression(whole_instr->X_op_symbol);

            if((first_op->X_op == O_register) && (second_op->X_op == O_constant))
            {
                insn->A_type.opcode = opcode;
                insn->A_type.rX_or_ctrl = first_op->X_add_number;
                insn->A_type.immed = second_op->X_add_number;
            }
            else if((first_op->X_op == O_register) && (second_op->X_op == O_register))
            {
                insn->B_type.prefix = OP_B_TYPE_PREFIX;
                insn->B_type.rX_or_ctrl = first_op->X_add_number;
                insn->B_type.opcode = opcode;
                insn->B_type.rY_or_ctrl = second_op->X_add_number;
            }
            else
            {
                as_bad("%s usage: first op has to be reg, second op has to be either reg or const", whole_instr->X_op);
                error = 1;
            }

            break;


        /********************************************************/
        /*                     ONE OPERAND                      */
        /********************************************************/

    /* Only one reg as operand --------------------------------------------------------*/
        case SWP:
            gas_assert(whole_instr->X_add_symbol != 0);

            first_op = symbol_get_value_expression(whole_instr->X_add_symbol);

            if(first_op->X_op == O_register)
            {
                insn->A_type.opcode = OP_SWP_SHIFT;
                insn->A_type.rX_or_ctrl = first_op->X_add_number;
                insn->A_type.immed = 0;
            }
            else
            {
                as_bad("%s usage: the first (an only) op has to be reg", whole_instr->X_op);
                error = 1;
            }

            break;

        case SL0: if(whole_instr->X_op == SL0) { ctrl_field = 0b0000; }
        case SL1: if(whole_instr->X_op == SL1) { ctrl_field = 0b0100; }
        case SR0: if(whole_instr->X_op == SR0) { ctrl_field = 0b0001; }
        case SR1: if(whole_instr->X_op == SR1) { ctrl_field = 0b0101; }
        case ASR: if(whole_instr->X_op == ASR) { ctrl_field = 0b1001; }
        case ROL: if(whole_instr->X_op == ROL) { ctrl_field = 0b0010; }
        case ROR: if(whole_instr->X_op == ROR) { ctrl_field = 0b0011; }
        case RLC: if(whole_instr->X_op == RLC) { ctrl_field = 0b0110; }
        case RRC: if(whole_instr->X_op == RRC) { ctrl_field = 0b0111; }
            gas_assert(whole_instr->X_add_symbol != 0);

            first_op = symbol_get_value_expression(whole_instr->X_add_symbol);

            if(first_op->X_op == O_register)
            {
                insn->B_type.prefix = OP_B_TYPE_PREFIX;
                insn->B_type.rX_or_ctrl = first_op->X_add_number;
                insn->B_type.opcode = OP_SWP_SHIFT;
                insn->B_type.rY_or_ctrl = ctrl_field;
            }
            else
            {
                as_bad("%s usage: the first (an only) op has to be reg", whole_instr->X_op);
                error = 1;
            }

            break;

    /* Either a reg or an imm --------------------------------------------------------*/
        case JMP: if(whole_instr->X_op == JMP) { ctrl_field = 0b0000; }
        case JZ:  if(whole_instr->X_op == JZ)  { ctrl_field = 0b0001; }
        case JNZ: if(whole_instr->X_op == JNZ) { ctrl_field = 0b0010; }
        case JC:  if(whole_instr->X_op == JC)  { ctrl_field = 0b0011; }
        case JNC: if(whole_instr->X_op == JNC) { ctrl_field = 0b0100; }
        case JN:  if(whole_instr->X_op == JN)  { ctrl_field = 0b0101; }
        case JNN: if(whole_instr->X_op == JNN) { ctrl_field = 0b0110; }
        case JV:  if(whole_instr->X_op == JV)  { ctrl_field = 0b0111; }
        case JNV: if(whole_instr->X_op == JNV) { ctrl_field = 0b1000; }
        case JL:  if(whole_instr->X_op == JL)  { ctrl_field = 0b1010; }
        case JLE: if(whole_instr->X_op == JLE) { ctrl_field = 0b1011; }
        case JH:  if(whole_instr->X_op == JH)  { ctrl_field = 0b1100; }
        case JHE: if(whole_instr->X_op == JHE) { ctrl_field = 0b1101; }
        case JSE: if(whole_instr->X_op == JSE) { ctrl_field = 0b1110; }
        case JG:  if(whole_instr->X_op == JG)  { ctrl_field = 0b1111; }
        case JSR: if(whole_instr->X_op == JSR) { ctrl_field = 0b1001; }
             gas_assert(whole_instr->X_add_symbol != 0);

            first_op = symbol_get_value_expression(whole_instr->X_add_symbol);

            if(first_op->X_op == O_constant)
            {
                insn->A_type.opcode = OP_CTRL;
                insn->A_type.rX_or_ctrl = ctrl_field;
                insn->A_type.immed = first_op->X_add_number;
            }
            else if(first_op->X_op == O_register)
            {
                insn->B_type.prefix = OP_B_TYPE_PREFIX;
                insn->B_type.rX_or_ctrl = ctrl_field;
                insn->B_type.opcode = OP_CTRL;
                insn->B_type.rY_or_ctrl = first_op->X_add_number;
            }
            else
            {
                as_bad("%s usage: the first (an only) op has to be reg", whole_instr->X_op);
                error = 1;
            }

            break;


        /********************************************************/
        /*                     ZERO OPERAND                     */
        /********************************************************/  

    /* No operand at all --------------------------------------------------------*/
        case RTS: if(whole_instr->X_op == RTS) { ctrl_field = 0b1010; }
        case RTI: if(whole_instr->X_op == RTI) { ctrl_field = 0b1011; }
        case CLI: if(whole_instr->X_op == CLI) { ctrl_field = 0b1100; }
        case STI: if(whole_instr->X_op == STI) { ctrl_field = 0b1101; }
            insn->B_type.prefix = OP_B_TYPE_PREFIX;
            insn->B_type.rX_or_ctrl = ctrl_field;
            insn->B_type.opcode = OP_DATALESS;
            insn->B_type.rY_or_ctrl = 0;

            break;

        default:
            as_bad("No such opcode: '%s'\n", whole_instr->X_op);
    }

    minirisc_emit_insn(insn);
    free(insn);

    if(error != 0)
    {
        printf("Couldn't construct instuction: '%s'\n", insn_str);
        as_fatal(_("Couldn't construct instuction!"));
    }
    else
    {
        printf("Instruction written!\n");
    }

    return;
}

symbolS *md_undefined_symbol(char *name)
{
    as_bad("Undefined symbol '%s'\n", name);
    return 0;
}

const char *md_atof(int type, char *lit, int *size)
{
    return 0;
}

valueT md_section_align(asection *seg, valueT size)
{
    // int align = bfd_gert_section_alignment(stdoutput, seg);
    // int new_size = ((size + (1 << align) - 1) & -(1 << align));

    // return new_size;

    // Get the section alignment as a power of 2
    int align = seg->alignment_power;

    // Calculate the new size, rounded up to the nearest alignment boundary
    valueT new_size = ((size + (1 << align) - 1) & ~((1 << align) - 1));

    return new_size;
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