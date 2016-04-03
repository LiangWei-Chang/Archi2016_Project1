/***************************************************

	File: Binary2Assembly.cpp

	Author: PinYo

***************************************************/

#include "Binary2Assembly.h"
#include "Instruction.h"
#include "GlobalVar.h"
#include <vector>
#include <iostream>
using namespace std;

int CalC(int Word, bool Signed){
	if(Signed)
		return (Word << 16) >> 16;
	return (unsigned)(Word << 16) >> 16;
};

void Binary2Assembly(int Word){
	int opcode = 0, power = 1, rs = 0, rt = 0, rd = 0, shamt = 0, funct = 0;
	unsigned int C = 0;
	// Calculate opcode
	opcode = ((unsigned int) Word) >> 26;
	// Calculate rs
	rs = ((unsigned int) (Word << 6)) >> 27;
	// Calculate rt
	rt = ((unsigned int) (Word << 11)) >> 27;
	// Calculate rd
	rd = ((unsigned int) (Word << 16)) >> 27;
	// Calculate shamt
	shamt = ((unsigned int) (Word << 21)) >> 27;
	// Calculate funct
	funct = ((unsigned int) (Word << 26)) >> 26;
	
	// Transfer to Assembly
	switch (opcode){
		case 0:
			switch (funct){
				case 32:
					R_format("add", rs, rt, rd);
					break;
				case 33:
					R_format("addu", rs, rt, rd);
					break;
				case 34:
					R_format("sub", rs, rt, rd);
					break;
				case 36:
					R_format("and", rs, rt, rd);
					break;
				case 37:
					R_format("or", rs, rt, rd);
					break;
				case 38:
					R_format("xor", rs, rt, rd);
					break;
				case 39:
					R_format("nor", rs, rt, rd);
					break;
				case 40:
					R_format("nand", rs, rt, rd);
					break;
				case 42:
					R_format("slt", rs, rt, rd);
					break;
				case 0:
					R_format2("sll", rt, rd, shamt);
					break;
				case 2:
					R_format2("srl", rt, rd, shamt);
					break;
				case 3:
					R_format2("sra", rt, rd, shamt);
					break;
				case 8:
					R_format3("jr", rs);
					break;
			}
			break;
		case 8:
			I_format("addi", rs, rt, CalC(Word, true));
			break;
		case 9:
			I_format("addiu", rs, rt, CalC(Word, true));
			break;
		case 35:
			I_format("lw", rs, rt, CalC(Word, true));
			break;
		case 33:
			I_format("lh", rs, rt, CalC(Word, true));
			break;
		case 37:
			I_format("lhu", rs, rt, CalC(Word, true));
			break;
		case 32:
			I_format("lb", rs, rt, CalC(Word, true));
			break;
		case 36:
			I_format("lbu", rs, rt, CalC(Word, true));
			break;
		case 43:
			I_format("sw", rs, rt, CalC(Word, true));
			break;
		case 41:
			I_format("sh", rs, rt, CalC(Word, true));
			break;
		case 40:
			I_format("sb", rs, rt, CalC(Word, true));
			break;
		case 15:
			I_format2("lui", rt, CalC(Word, true));
			break;
		case 12:
			I_format("andi", rs, rt, CalC(Word, false));
			break;
		case 13:
			I_format("ori", rs, rt, CalC(Word, false));
			break;
		case 14:
			I_format("nori", rs, rt, CalC(Word, false));
			break;
		case 10:
			I_format("slti", rs, rt, CalC(Word, true));
			break;
		case 4:
			I_format("beq", rs, rt, CalC(Word, true));
			break;
		case 5:
			I_format("bne", rs, rt, CalC(Word, true));
			break;
		case 7:
			I_format3("bgtz", rs, CalC(Word, true));
			break;
		case 2:
			C = (unsigned)(Word << 6) >> 6;
			J_format("j", C);
			break;
		case 3:
			C = (unsigned)(Word << 6) >> 6;
			J_format("jal", C);
			break;
		case 63:
			Terminal::Halt = true;
			break;
	}
}