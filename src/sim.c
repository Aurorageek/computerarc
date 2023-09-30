#include <stdio.h>
#include "shell.h"




void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
      // 从内存中读取当前 PC 地址处的指令
uint32_t instruction = mem_read_32(CURRENT_STATE.PC);


// 提取指令的操作码（opcode）
uint32_t opcode = (instruction >> 26) & 0x3F;



// 提取指令的目标寄存器、源寄存器和立即数等字段
uint32_t rs = (instruction >> 21) & 0x1f;
uint32_t rt = (instruction >> 16) & 0x1f;

//R用
uint32_t rd = (instruction >> 11) & 0x1f;
uint32_t shamt = (instruction >> 6) & 0x1f; 
uint32_t funccode = instruction & 0x3f;

uint32_t J_targetadr = instruction & 0x3ffffff; //获取跳转目标地址低26位（J用）

uint32_t imm = instruction & 0xffff; //获取立即数字段低16位(I用)

    // 根据操作码执行相应的操作
    switch (opcode) 
    {
        case 0x0: 
        {//确定为R型指令(27个)
            switch (funccode) 
            {
            case 0x20: {// add
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;
            }

            case 0x21:{ // addu
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;
            }

            case 0x22:{ // sub
                 NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;
            }

            case 0x23: // subu
                {  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x24: // and
                { NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x25: // or
                 {NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x26: // xor
                 {NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;
                 }

            case 0x27: // nor
                 { NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x2A: // slt
                 {NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rs] < (int32_t)CURRENT_STATE.REGS[rt] ? 1 : 0;
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x2B: // sltu
            {NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt] ? 1 : 0;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

            case 0x00: // sll
                 {NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x02: // srl
              { NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x03: // sra
               {  NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> shamt;
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x04: // sllv
              { NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << (CURRENT_STATE.REGS[rs] & 0x1F);
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x06: // srlv
                { NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> (CURRENT_STATE.REGS[rs] & 0x1F);
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x07: // srav
                { NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> (CURRENT_STATE.REGS[rs] & 0x1F);
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;}

            case 0x08: // jr
                // 实际操作中会跳转到寄存器 rs 中存储的地址
                {NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
                break;}

            case 0x09: // jalr
                // 类似于 jr，但会将返回地址存储在 rd 中
                {NEXT_STATE.REGS[rd] = CURRENT_STATE.PC + 4;
                NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
                break;
               }

            case 0x10: // mfhi
            {NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

            case 0x11: // mthi
            {NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

            case 0x12: // mflo
            {NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

            case 0x13: // mtlo
            {NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

            case 0x18: // mult
            {
                int64_t result = (int64_t)CURRENT_STATE.REGS[rs] * (int64_t)CURRENT_STATE.REGS[rt];
                NEXT_STATE.LO = (uint32_t)(result & 0xFFFFFFFF);
                NEXT_STATE.HI = (uint32_t)((result >> 32) & 0xFFFFFFFF);
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;
            }

            case 0x19: // multu
            {
                uint64_t result = (uint64_t)CURRENT_STATE.REGS[rs] * (uint64_t)CURRENT_STATE.REGS[rt];
                NEXT_STATE.LO = (uint32_t)(result & 0xFFFFFFFF);
                NEXT_STATE.HI = (uint32_t)((result >> 32) & 0xFFFFFFFF);
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                 break;
            }
            case 0x1A: // div
            {
                int32_t rs_value = (int32_t)CURRENT_STATE.REGS[rs];
                int32_t rt_value = (int32_t)CURRENT_STATE.REGS[rt];
                
                if (rt_value != 0) {
                    NEXT_STATE.LO = rs_value / rt_value;
                    NEXT_STATE.HI = rs_value % rt_value;
                } else {
                    // 处理除以零异常
                    // 这里可以根据需要进行处理，比如触发异常或者其他操作
                    printf(" error! divisor is zero!");
                }

                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;
            }
            case 0x1B: // divu
            {
                uint32_t rs_value = CURRENT_STATE.REGS[rs];
                uint32_t rt_value = CURRENT_STATE.REGS[rt];
                
                if (rt_value != 0) {
                    NEXT_STATE.LO = rs_value / rt_value;
                    NEXT_STATE.HI = rs_value % rt_value;
                } else {
                    // 处理除以零异常
                    // 这里可以根据需要进行处理，比如触发异常或者其他操作
                    printf(" error! divisor is zero!");
                }

                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;
            }
            
            
            case 0xc: //syscall
            {
                if (CURRENT_STATE.REGS[2] == 0x0a) {//v0在reg[2]中
                        RUN_BIT = FALSE;
                    } 
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    
                    break;
            }

            default:{
                printf("Unknown function code: 0x%02X\n", instruction);
                break;
            }

            }
            break;

        }
        //J型（2个）
        case 0x02: // J
        {NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) | (J_targetadr << 2);
        break;}

        case 0x03: // JAL
        {NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 8; // 存储返回地址
        NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) | (J_targetadr << 2);
        break;}



        //I型(24个)

        case 0x01: // REGIMM
        {
                switch (rt) {
                case 0x00: // BLTZ小于0分支
                    {if ((int32_t)CURRENT_STATE.REGS[rs] < 0) {
                        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4; // 存储返回地址
                        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2); // 跳转到目标地址
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;}

                case 0x01: // BGEZ  大于等于0分支
                    {if ((int32_t)CURRENT_STATE.REGS[rs] >= 0) {
                        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4; // 存储返回地址
                        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2); // 跳转到目标地址
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;}

                case 0x10: // BLTZAL小于0跳转并链接
                    {if ((int32_t)CURRENT_STATE.REGS[rs] < 0) {
                        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4; // 存储返回地址
                        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2); // 跳转到目标地址
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;}

                case 0x11: // BGEZAL大于等于0跳转并链接
                    {if ((int32_t)CURRENT_STATE.REGS[rs] >= 0) {
                        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4; // 存储返回地址
                        NEXT_STATE.PC = CURRENT_STATE.PC + (imm << 2); // 跳转到目标地址
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;}
                    
                        }
        break;
        }

        case 0x04: // BEQ
            {if (CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4 + (imm << 2);
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;}

        case 0x05: // BNE
            {if (CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4 + (imm << 2);
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;}

        case 0x07:{ // BGTZ大于0分支
        if(rt==0)
         {
        if ((int32_t)CURRENT_STATE.REGS[rs] > 0) {
                NEXT_STATE.PC = CURRENT_STATE.PC +4+ (imm << 2); // 跳转到目标地址
         } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
         }
         }
         else
         {
             printf("illegal BGTZ rt");
        }
         break;
        }

        case 0x06: {// BLEZ小于等于0分支
        if(rt==0){
            if ((int32_t)CURRENT_STATE.REGS[rs] <= 0) {
                NEXT_STATE.PC = CURRENT_STATE.PC +4+ (imm << 2); // 跳转到目标地址
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            
        }
            else
            {
                printf("illegal BGTZ rt");
            }
            break;
        }

        case 0x08: // ADDI
            {NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + imm;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        case 0x09: // ADDIU
           { NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + imm;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        case 0x23: // LW
           { NEXT_STATE.REGS[rt] = mem_read_32(CURRENT_STATE.REGS[rs] + imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        case 0x21: // LH
            { uint32_t address = CURRENT_STATE.REGS[rs] + (int32_t)imm;

	   
	     uint16_t half = mem_read_32(address) & 0xffff;
	     int32_t data0 = *((int8_t*)&half); 
	    uint32_t data= *((uint32_t*)&data0); // 转换为32位数
	    // 将读取的半字数据符号扩展为32位，并写入目标寄存器
	    CURRENT_STATE.REGS[rt] = data;
	    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        case 0x25: // LHU
            {uint32_t address = CURRENT_STATE.REGS[rs] + (int32_t)imm;

	    // 从内存读取有符号半字数据
	     uint16_t half = mem_read_32(address) & 0xffff;
	     CURRENT_STATE.REGS[rt] = (uint32_t)half;
	     NEXT_STATE.PC = CURRENT_STATE.PC + 4;
	     
            break;}

        case 0x20: // LB
             { uint32_t address = CURRENT_STATE.REGS[rs] + (int32_t)imm;

	   
	     uint8_t quart = mem_read_32(address) & 0xff;
	     int32_t data0 = *((int8_t*)&quart); 
	    uint32_t data= *((uint32_t*)&data0); // 转换为32位数
	    // 将读取的半字数据符号扩展为32位，并写入目标寄存器
	    CURRENT_STATE.REGS[rt] = data;
	    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}


        case 0x24: // LBU
             {uint32_t address = CURRENT_STATE.REGS[rs] + (int32_t)imm;

	    // 从内存读取有符号半字数据
	     uint8_t quart = mem_read_32(address) & 0xff;
	     CURRENT_STATE.REGS[rt] = (uint32_t)quart;
	     NEXT_STATE.PC = CURRENT_STATE.PC + 4;
	     
            break;}

        case 0x2B: // SW
            {mem_write_32(CURRENT_STATE.REGS[rs] + imm, CURRENT_STATE.REGS[rt]);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        case 0x29: // SH
            { 
            uint32_t address = CURRENT_STATE.REGS[rs] + (int32_t)imm;
            uint32_t val = (mem_read_32(address) & 0xffff0000) |
                           (CURRENT_STATE.REGS[rt] & 0xffff);
            mem_write_32(address, val);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        case 0x28: // SB
            { 
            uint32_t address = CURRENT_STATE.REGS[rs] + (int32_t)imm;
            uint32_t val = (mem_read_32(address) & 0xffffff00) |
                           (CURRENT_STATE.REGS[rt] & 0xff);

            mem_write_32(address, val);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            
            break;}

        

        case 0x0A: // SLTI
           { NEXT_STATE.REGS[rt] = (int32_t)CURRENT_STATE.REGS[rs] < imm ? 1 : 0;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        case 0x0B: // SLTIU
           { NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] < (uint32_t)imm ? 1 : 0;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        

        case 0x0C: // ANDI
            {NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & imm;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        case 0x0D: // ORI
           { NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | imm;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        case 0x0E: // XORI
            {NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ imm;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}

        case 0x0F: // LUI
        { NEXT_STATE.REGS[rt] = imm << 16;
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        break;}

      
    } 
}