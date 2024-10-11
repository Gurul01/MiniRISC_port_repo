#ifndef _MINIRISC_OPCODES_H
#define _MINIRISC_OPCODES_H

#define MINIRISC_NUM_REGISTERS 16

#ifndef MINIRISC_BYTES_SLOT_INSTRUCTION
#define MINIRISC_BYTES_SLOT_INSTRUCTION 2
#endif

#define OP_B_TYPE_PREFIX 0b1111

enum minirisc_opcode
{
    // Different meaing in case of A and B (here A and B type are not refering to the last operands type aka reg/const)
    OP_LDSP_LOAD  = 0b1101,
    // Different meaing in case of A and B (here A and B type are not refering to the last operands type aka reg/const)
    OP_STSP_STORE = 0b1001,
    OP_MOV   = 0b1100, 
    OP_ADD   = 0b0000, 
    OP_ADC   = 0b0001, 
    OP_SUB   = 0b0010, 
    OP_SBC   = 0b0011, 
    OP_CMP   = 0b1010, 
    OP_CMC   = 0b1011, 
    OP_AND   = 0b0100, 
    OP_OR    = 0b0101,  
    OP_XOR   = 0b0110, 
    OP_TST   = 0b1000, 
    // Different meaing in case of A and B (here A and B type are not refering to the last operands type aka reg/const)
    OP_SWP_SHIFT = 0b0111, 
    // OP_SL0   = 0b, 
    // OP_SL1   = 0b, 
    // OP_SR0   = 0b, 
    // OP_SR1   = 0b, 
    // OP_ASR   = 0b, 
    // OP_ROL   = 0b, 
    // OP_ROR   = 0b, 
    // OP_RLC   = 0b, 
    // OP_RRC   = 0b, 
    OP_CTRL = 0b1110,
    // OP_JMP   = 0b, 
    // OP_JZ    = 0b,  
    // OP_JNZ   = 0b, 
    // OP_JC    = 0b,  
    // OP_JNC   = 0b, 
    // OP_JN    = 0b,  
    // OP_JNN   = 0b, 
    // OP_JV    = 0b,  
    // OP_JNV   = 0b, 
    // OP_JL    = 0b,  
    // OP_JLE   = 0b, 
    // OP_JH    = 0b,  
    // OP_JHE   = 0b, 
    // OP_JSE   = 0b, 
    // OP_JG    = 0b,    
    // OP_JSR   = 0b, 
    // This is the OP_CTRL_NO_DATA in the opcode_defs.vh
    OP_DATALESS = 0b1111
    // OP_RTS   = 0b, 
    // OP_RTI   = 0b, 
    // OP_CLI   = 0b,
    // OP_STI   = 0b
};

enum minirisc_ctrl_code{
    SL0_CTRL = 0b0000,
    SL1_CTRL = 0b0100,
    SR0_CTRL = 0b0001,
    SR1_CTRL = 0b0101,
    ASR_CTRL = 0b1001,
    ROL_CTRL = 0b0010,
    ROR_CTRL = 0b0011,
    RLC_CTRL = 0b0110,
    RRC_CTRL = 0b0111,

    JMP_CTRL = 0b0000,
    JZ_CTRL  = 0b0001,
    JNZ_CTRL = 0b0010,
    JC_CTRL  = 0b0011,
    JNC_CTRL = 0b0100,
    JN_CTRL  = 0b0101,
    JNN_CTRL = 0b0110,
    JV_CTRL  = 0b0111,
    JNV_CTRL = 0b1000,
    JL_CTRL  = 0b1010,
    JLE_CTRL = 0b1011,
    JH_CTRL  = 0b1100,
    JHE_CTRL = 0b1101,
    JSE_CTRL = 0b1110,
    JG_CTRL  = 0b1111,
    JSR_CTRL = 0b1001,

    RTS_CTRL = 0b1010,
    RTI_CTRL = 0b1011,
    CLI_CTRL = 0b1100,
    STI_CTRL = 0b1101
};


// enum minirisc_admode
// {
//     AM_0 = 0,
//     AM_1 = 1
// };

// Every field is 2x wider compared to the final instruction
typedef struct
{
    __uint8_t  opcode;
    __uint8_t  rX_or_ctrl;
    __uint16_t immed;
} minirisc_insn_A_type;

// Every field is 2x wider compared to the final instruction
typedef struct
{
    __uint8_t prefix;
    __uint8_t rX_or_ctrl;
    __uint8_t opcode;
    __uint8_t rY_or_ctrl;
} minirisc_insn_B_type;

// The width of minirisc_insn_A_type and minirisc_insn_B_type has to match
typedef union minirisc_slot_insn
{
    minirisc_insn_A_type A_type;
    minirisc_insn_B_type B_type;
} minirisc_slot_insn;

#endif /* _MINIRISC_OPCODES_H */