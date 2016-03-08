/***************************************************

	File: Binary2Assembly.cpp

	Author: PinYo

***************************************************/

#include "Binary2Assembly.h"
#include "Instruction.h"
#include <vector>
using namespace std;

void Binary2Assembly(vector<int> Word, Register *reg, Register &PC){
	int opcode = 0, power = 1, rs = 0, rt = 0, rd = 0, shamt = 0, funct = 0, C = 0, next_addr;
	// Calculate opcode
	for(int i=0; i<6; i++){
		opcode += (Word[26+i]*power);
		power *= 2;
	}
	// Calculate rs
	power = 1;
	for(int i=0; i<5; i++){
		rs += (Word[21+i]*power);
		power *= 2;
	}
	// Calculate rt
	power = 1;
	for(int i=0; i<5; i++){
		rt += (Word[16+i]*power);
		power *= 2;
	}
	// Calculate rd
	power = 1;
	for(int i=0; i<5; i++){
		rd += (Word[11+i]*power);
		power *= 2;
	}
	// Calculate shamt
	power = 1;
	for(int i=0; i<5; i++){
		shamt += (Word[6+i]*power);
		power *= 2;
	}
	// Calculate funct
	power = 1;
	for(int i=0; i<6; i++){
		funct += (Word[i]*power);
		power *= 2;
	}
	// Calculate C
	power = 1;
	for(int i=0; i<16; i++){
		C += (Word[i]*power);
		power *= 2;
	}
	// Transfer to Assembly
	switch (opcode){
		case 0:
			switch (funct){
				case 32:
					R_format("add", rs, rt, rd, reg, PC);
					break;
				case 33:
					R_format("addu", rs, rt, rd, reg, PC);
					break;
				case 34:
					R_format("sub", rs, rt, rd, reg, PC);
					break;
				case 36:
					R_format("and", rs, rt, rd, reg, PC);
					break;
				case 37:
					R_format("or", rs, rt, rd, reg, PC);
					break;
				case 38:
					R_format("xor", rs, rt, rd, reg, PC);
					break;
				case 39:
					R_format("nor", rs, rt, rd, reg, PC);
					break;
				case 40:
					R_format("nand", rs, rt, rd, reg, PC);
					break;
				case 42:
					R_format("slt", rs, rt, rd, reg, PC);
					break;
				case 0:
					R_format2("sll", rt, rd, C, reg, PC);
					break;
				case 2:
					R_format2("srl", rt, rd, C, reg, PC);
					break;
				case 3:
					R_format2("sra", rt, rd, C, reg, PC);
					break;
				case 8:
					R_format3("jr", rs, reg, PC);
					break;
			}
			break;
		case 8:
			I_format("addi", rs, rt, C, reg, PC);
			break;
		case 9:
			I_format("addiu", rs, rt, C, reg, PC);
			break;
		case 35:
			I_format("lw", rs, rt, C, reg, PC);
			break;
		case 33:
			I_format("lh", rs, rt, C, reg, PC);
			break;
		case 37:
			I_format("lhu", rs, rt, C, reg, PC);
			break;
		case 32:
			I_format("lb", rs, rt, C, reg, PC);
			break;
		case 36:
			I_format("lbu", rs, rt, C, reg, PC);
			break;
		case 43:
			I_format("sw", rs, rt, C, reg, PC);
			break;
		case 41:
			I_format("sh", rs, rt, C, reg, PC);
			break;
		case 40:
			I_format("sb", rs, rt, C, reg, PC);
			break;
		case 15:
			I_format2("lui", rt, C, reg, PC);
			break;
		case 12:
			I_format("andi", rs, rt, C, reg, PC);
			break;
		case 13:
			I_format("ori", rs, rt, C, reg, PC);
			break;
		case 14:
			I_format("nori", rs, rt, C, reg, PC);
			break;
		case 10:
			I_format("slti", rs, rt, C, reg, PC);
			break;
		case 4:
			I_format("beq", rs, rt, C, reg, PC);
			break;
		case 5:
			I_format("bne", rs, rt, C, reg, PC);
			break;
		case 7:
			I_format3("bgtz", rs, C, reg, PC);
			break;
		case 2:
			J_format("j", C, reg, PC);
			break;
		case 3:
			J_format("jal", C, reg, PC);
			break;
		case 63:
			PC.value = 0xFFFF;
	}
}