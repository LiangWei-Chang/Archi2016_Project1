/***************************************************

	File: Binary2Assembly.cpp

	Author: PinYo

***************************************************/

#include "Binary2Assembly.h"
#include "Instruction.h"
#include "GlobalVar.h"
#include <vector>
using namespace std;

unsigned int CalC(vector<int> Word, bool Signed){
	int sum = 0, power = 1, neg[16];
	if(Signed){
		if(Word[15]==1){
			for(int i=0; i<16; i++)
				neg[i] = !Word[i];
			int carry = 1, temp;
			for(int i=0; i<16; i++){
				temp = (neg[i]+carry) % 2;
				carry = (neg[i]+carry) / 2;
				neg[i] = temp;
				sum += (neg[i] * power);
				power *= 2;
			}
			sum *= (-1);
			return sum;
		}
	}
	for(int i=0; i<16; i++){
		sum += (Word[i] * power);
		power *= 2;
	}
	return sum;
};

void Binary2Assembly(vector<int> Word){
	int opcode = 0, power = 1, rs = 0, rt = 0, rd = 0, shamt = 0, funct = 0, next_addr;
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
					R_format2("sll", rt, rd, CalC(Word, true));
					break;
				case 2:
					R_format2("srl", rt, rd, CalC(Word, true));
					break;
				case 3:
					R_format2("sra", rt, rd, CalC(Word, true));
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
			I_format("addiu", rs, rt, CalC(Word, false));
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
			J_format("j", CalC(Word, false));
			break;
		case 3:
			J_format("jal", CalC(Word, false));
			break;
		case 63:
			CPURegister::PC.value = 0xFFFF;
	}
}