/***************************************************

	File: Instruction.cpp

	Author: PinYo

***************************************************/

#include "Instruction.h"
#include "Memory.h"
#include <vector>
#include <bitset>

using namespace std;

int Bin2Dec(int *Word, int Bits){ // **Consider negative num**
	int sum = 0, power = 1;
	for(int i=0; i<Bits; i++){
		sum += (Word[i] * power);
		power *= 2;
	}
	return sum;
}

void R_format(string op, int rs, int rt, int rd, Register *reg, Register &PC){
	if(op == "add")	reg[rd].value = reg[rs].value + reg[rt].value;
	else if(op == "addu") reg[rd].value = reg[rs].value + reg[rt].value; // Need Modified
	else if(op == "sub") reg[rd].value = reg[rs].value - reg[rt].value;
	else if(op == "and") reg[rd].value = reg[rs].value & reg[rt].value;
	else if(op == "or")	reg[rd].value = reg[rs].value | reg[rt].value;
	else if(op == "xor") reg[rd].value = reg[rs].value ^ reg[rt].value;
	else if(op == "nor")	reg[rd].value = ~(reg[rs].value | reg[rt].value);
	else if(op == "nand") reg[rd].value = ~(reg[rs].value & reg[rt].value);
	else if(op == "slt") reg[rd].value = (reg[rs].value < reg[rt].value);
	PC.value += 4;
}

void R_format2(string op, int rt, int rd, int C, Register *reg, Register &PC){
	if(op == "sll")	reg[rd].value = reg[rt].value << C;
	else if(op == "srl") reg[rd].value = reg[rt].value >> C;
	else if(op == "sra") reg[rd].value = reg[rt].value >> C; // Need Modified
	PC.value += 4;
}

void R_format3(string op, int rs, Register *reg, Register &PC){
	PC.value = reg[rs].value;
}

void I_format(string op, int rs, int rt, int C, Register *reg, Register &PC){
	if(op == "addi") reg[rt].value = reg[rs].value + C;
	else if(op == "addiu") reg[rt].value = reg[rs].value + C; // Need Modified
	else if(op == "lw"){
		int Word[32];
		for(int i=4; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[rs+C+(4-i)][7-j];
		reg[rt].value = Bin2Dec(Word, 32);
	}
	else if(op == "lh"){ // Need Modified
		int Word[16];
		for(int i=2; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[rs+C+(2-i)][7-j];
		reg[rt].value = Bin2Dec(Word, 16);
	} 
	else if(op == "lhu"){ // Need Modified
		int Word[16];
		for(int i=2; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[rs+C+(2-i)][7-j];
		reg[rt].value = Bin2Dec(Word, 16);
	}
	else if(op == "lb"){ // Need Modified
		int Word[8];
		for(int i=0; i<8; i++)
			Word[i] = DataMemory::Memory[rs+C][i];
		reg[rt].value = Bin2Dec(Word, 8);
	}
	else if(op == "lbu"){ // Need Modified
		int Word[8];
		for(int i=0; i<8; i++)
			Word[i] = DataMemory::Memory[rs+C][i];
		reg[rt].value = Bin2Dec(Word, 8);
	}
	else if(op == "sw"){
		bitset<32> bs;
		bs = reg[rt].value;
		for(int i=4; i>0; i--)
			for(int j=7; j>=0; j--)
				DataMemory::Memory[rs+C+(4-i)][j] = bs[i*8-1-(7-j)];
	}	
	else if(op == "sh"){ // Need Modified
		bitset<32> bs;
		bs = reg[rt].value;
		for(int i=2; i>0; i--)
			for(int j=7; j>=0; j--)
				DataMemory::Memory[rs+C+(2-i)][j] = bs[i*8-1-(7-j)];
	}
	else if(op == "sb"){ // Need Modified
		bitset<32> bs;
		bs = reg[rt].value;
		for(int i=7; i>=0; i--)
			DataMemory::Memory[rs+C][i] = bs[i];
	}
	else if(op == "andi") reg[rt].value = reg[rs].value & C;
	else if(op == "ori") reg[rt].value = reg[rs].value | C;
	else if(op == "nori") reg[rt].value = reg[rs].value ^ C;
	else if(op == "slti") reg[rt].value = (reg[rs].value < C);
	else if(op == "beq"){ // Need Modified
		if(reg[rs].value==reg[rt].value){
			PC.value = PC.value + 4 + 4*C;
			return;
		}
	}
	else if(op == "bne"){ // Need Modified
		if(reg[rs].value!=reg[rt].value){
			PC.value = PC.value + 4 + 4*C;
			return;
		}
	}
	PC.value += 4;
}

void I_format2(string op, int rt, int C, Register *reg, Register &PC){
	reg[rt].value = C << 16;
}

void I_format3(string op, int rs, int C, Register *reg, Register &PC){
	if(reg[rs].value > 0) PC.value = PC.value + 4 + 4*C; // Need Modified
}

void J_format(string op, int C, Register *reg, Register &PC){
	// Need Modified
}