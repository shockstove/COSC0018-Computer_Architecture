#include <stdio.h>
#include "shell.h"

uint32_t sign_extended(uint32_t imm,uint32_t bits)
{
    uint32_t temp;
    temp = imm & ((1 << bits) - 1);
    if (temp & (1 << (bits - 1))) {
        temp |= 0xFFFFFFFF << bits;
    }
    return temp;
}

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */

    uint32_t instruction=mem_read_32(CURRENT_STATE.PC);
    uint32_t op=instruction>>26;
    uint32_t rs=instruction>>21&0b11111;
    uint32_t rt=instruction>>16&0b11111;
    uint32_t rd=instruction>>11&0b11111;
    uint32_t imm=instruction&0b1111111111111111;
    uint32_t targ=instruction&0b11111111111111111111111111;
    uint32_t shamt=instruction>>6&0b111111;
    uint32_t funct=instruction&0b111111;
    uint32_t vaddr,temp;
    switch (op)
    {
    case 0b000000:
        switch (funct)
        {
        case 0b000000://sll
            NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rt]<<shamt;
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b000010://srl
            NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rt]>>shamt;
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b000011://sra
            NEXT_STATE.REGS[rd]=(signed)CURRENT_STATE.REGS[rt]>>shamt;
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b000100://sllv
            NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rt]<<(CURRENT_STATE.REGS[rs]&0b11111);
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b000110://srlv
            NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rt]>>(CURRENT_STATE.REGS[rs]&0b11111);
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b000111://srav
            NEXT_STATE.REGS[rd]=(signed)CURRENT_STATE.REGS[rt]>>(CURRENT_STATE.REGS[rs]&0b11111);
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b001000://jr
            NEXT_STATE.PC=CURRENT_STATE.REGS[rs];
            break;
        case 0b001001://jalr
            NEXT_STATE.REGS[31]=CURRENT_STATE.PC+4;
            NEXT_STATE.PC=CURRENT_STATE.REGS[rs];
            break;
        case 0b100000://add
        case 0b100001://addu
            NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]+CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b100010://sub
        case 0b100011://subu
            NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]-CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b101010://slt
            if((signed)CURRENT_STATE.REGS[rs]<(signed)CURRENT_STATE.REGS[rt])
                NEXT_STATE.REGS[rd]=1;
            else 
                NEXT_STATE.REGS[rd]=0;
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b101011://sltu
            if(CURRENT_STATE.REGS[rs]<CURRENT_STATE.REGS[rt])
                NEXT_STATE.REGS[rd]=1;
            else 
                NEXT_STATE.REGS[rd]=0;
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b011000://mult
            NEXT_STATE.LO=(signed)CURRENT_STATE.REGS[rs]*(signed)CURRENT_STATE.REGS[rt];
            NEXT_STATE.HI=(int64_t)((signed)CURRENT_STATE.REGS[rs]*(signed)CURRENT_STATE.REGS[rt])>>32;
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b011001://multu
            NEXT_STATE.LO=CURRENT_STATE.REGS[rs]*CURRENT_STATE.REGS[rt];
            NEXT_STATE.HI=(int64_t)(CURRENT_STATE.REGS[rs]*CURRENT_STATE.REGS[rt])>>32;
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b011010://div
            NEXT_STATE.LO=(signed)CURRENT_STATE.REGS[rs]/(signed)CURRENT_STATE.REGS[rt];
            NEXT_STATE.HI=(signed)CURRENT_STATE.REGS[rs]%(signed)CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b011011://divu
            NEXT_STATE.LO=CURRENT_STATE.REGS[rs]/CURRENT_STATE.REGS[rt];
            NEXT_STATE.HI=CURRENT_STATE.REGS[rs]%CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b100100://and
            NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]&CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b100101://or
            NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]|CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b100110://xor
            NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]^CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
        case 0b100111://nor
            NEXT_STATE.REGS[rd]=~(CURRENT_STATE.REGS[rs]|CURRENT_STATE.REGS[rt]);
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b010000://mfhi
            NEXT_STATE.REGS[rd]=CURRENT_STATE.HI;
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b010010://mflo
            NEXT_STATE.REGS[rd]=CURRENT_STATE.LO;
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b010001://mthi
            NEXT_STATE.HI=CURRENT_STATE.REGS[rs];
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b010011://mtlo
            NEXT_STATE.LO=CURRENT_STATE.REGS[rs];
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b001100://syscall
            if(CURRENT_STATE.REGS[2]==0x0a)
            RUN_BIT=FALSE;
            else NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        default:
            break;
        }
        break;
    case 0b001000://addi 不考虑溢出
    case 0b001001://addiu 
        NEXT_STATE.REGS[rt]=CURRENT_STATE.REGS[rs]+sign_extended(imm,16);
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
        case 0b001010://slti
        if(CURRENT_STATE.REGS[rs]<(signed)sign_extended(imm,16))
            NEXT_STATE.REGS[rt]=1;
        else 
            NEXT_STATE.REGS[rt]=0;
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b001011://sltiu
        if(CURRENT_STATE.REGS[rs]<sign_extended(imm,16))
            NEXT_STATE.REGS[rt]=1;
        else 
            NEXT_STATE.REGS[rt]=0;
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b001100://andi
        NEXT_STATE.REGS[rt]=CURRENT_STATE.REGS[rs]&0xffff;
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b001110://xori
        NEXT_STATE.REGS[rt]=CURRENT_STATE.REGS[rs]^imm;
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b001101://ori
        NEXT_STATE.REGS[rt]=CURRENT_STATE.REGS[rs]|imm;
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b001111://lui
        NEXT_STATE.REGS[rt]=imm<<16;
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b000100://beq
        if(CURRENT_STATE.REGS[rs]==CURRENT_STATE.REGS[rt])
            NEXT_STATE.PC=CURRENT_STATE.PC+sign_extended(imm<<2,18);
        else 
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b000111://bgtz
        if(CURRENT_STATE.REGS[rs]>>31==0&&CURRENT_STATE.REGS[rs]!=0)
            NEXT_STATE.PC=CURRENT_STATE.PC+sign_extended(imm<<2,18);
        else
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break; 
    case 0b000001:
        switch (rt)
        {
        case 0b10001://bgezal
            NEXT_STATE.REGS[31]=CURRENT_STATE.PC+4;
        case 0b00001://bgez
            if((signed)CURRENT_STATE.REGS[rs]>=0)
                NEXT_STATE.PC=CURRENT_STATE.PC+sign_extended(imm<<2,18);
            else 
                NEXT_STATE.PC=CURRENT_STATE.PC+4;
            break;
        case 0b10000://bltzal
            NEXT_STATE.REGS[31]=CURRENT_STATE.PC+4;
        case 0://bltz
            if((signed)CURRENT_STATE.REGS[rs]<0)
                NEXT_STATE.PC=CURRENT_STATE.PC+sign_extended(imm<<2,18);
            else 
                NEXT_STATE.PC=CURRENT_STATE.PC+4;
        default:
            break;
        }
        break;
    case 0b000010://j
        NEXT_STATE.PC=(CURRENT_STATE.PC&0xf0000000)|targ<<2;
        break;
    case 0b000011://jal
        NEXT_STATE.PC=(CURRENT_STATE.PC&0xf0000000)|targ<<2;
        NEXT_STATE.REGS[31]=CURRENT_STATE.PC+4;
        break;
    case 0b000101://bne
        if(CURRENT_STATE.REGS[rs]!=CURRENT_STATE.REGS[rt])
            NEXT_STATE.PC=CURRENT_STATE.PC+sign_extended(imm<<2,18);
        else
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b000110://blez
        if(CURRENT_STATE.REGS[rs]>>31==1||CURRENT_STATE.REGS[rs]==0)
            NEXT_STATE.PC=CURRENT_STATE.PC+sign_extended(imm<<2,18);
        else 
            NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b100000://lb
        vaddr=sign_extended(imm,16)+CURRENT_STATE.REGS[rs];
        temp=mem_read_32(vaddr);
        NEXT_STATE.REGS[rt]=sign_extended(temp&0xff,8);
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b100001://lh
        vaddr=sign_extended(imm,16)+CURRENT_STATE.REGS[rs];
        temp=mem_read_32(vaddr);
        NEXT_STATE.REGS[rt]=sign_extended(temp&0xffff,16);
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b100011://lw
        vaddr=sign_extended(imm,16)+CURRENT_STATE.REGS[rs];
        temp=mem_read_32(vaddr);
        NEXT_STATE.REGS[rt]=temp;
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b100100://lbu
        vaddr=sign_extended(imm,16)+CURRENT_STATE.REGS[rs];
        temp=mem_read_32(vaddr);
        NEXT_STATE.REGS[rt]=temp&0xff;
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b100101://lhu
        vaddr=sign_extended(imm,16)+CURRENT_STATE.REGS[rs];
        temp=mem_read_32(vaddr);
        NEXT_STATE.REGS[rt]=temp&0xffff;
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b101000://sb
        vaddr=sign_extended(imm,16)+CURRENT_STATE.REGS[rs];
        temp=mem_read_32(vaddr)&0xffffff00|CURRENT_STATE.REGS[rt]&0x000000ff;
        mem_write_32(vaddr,temp);
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b101001://sh
        vaddr=sign_extended(imm,16)+CURRENT_STATE.REGS[rs];
        temp=mem_read_32(vaddr)&0xffff0000|CURRENT_STATE.REGS[rt]&0x0000ffff;
        mem_write_32(vaddr,temp);
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    case 0b101011://sw
        vaddr=sign_extended(imm,16)+CURRENT_STATE.REGS[rs];
        temp=CURRENT_STATE.REGS[rt]&0x0000ffff;
        mem_write_32(vaddr,temp);
        NEXT_STATE.PC=CURRENT_STATE.PC+4;
        break;
    default:
        break;
    }
    NEXT_STATE.REGS[0]=0;
    return;
}
