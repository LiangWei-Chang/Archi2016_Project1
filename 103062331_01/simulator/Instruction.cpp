/***************************************************

	File: Instruction.cpp

	Author: PinYo

***************************************************/

#include "Instruction.h"
#include "GlobalVar.h"
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

void R_format(string op, int rs, int rt, int rd){
	if(op == "add")	CPURegister::reg[rd].value = CPURegister::reg[rs].value + CPURegister::reg[rt].value;
	else if(op == "addu") CPURegister::reg[rd].value = CPURegister::reg[rs].value + CPURegister::reg[rt].value; // Need Modified
	else if(op == "sub") CPURegister::reg[rd].value = CPURegister::reg[rs].value - CPURegister::reg[rt].value;
	else if(op == "and") CPURegister::reg[rd].value = CPURegister::reg[rs].value & CPURegister::reg[rt].value;
	else if(op == "or")	CPURegister::reg[rd].value = CPURegister::reg[rs].value | CPURegister::reg[rt].value;
	else if(op == "xor") CPURegister::reg[rd].value = CPURegister::reg[rs].value ^ CPURegister::reg[rt].value;
	else if(op == "nor")	CPURegister::reg[rd].value = ~(CPURegister::reg[rs].value | CPURegister::reg[rt].value);
	else if(op == "nand") CPURegister::reg[rd].value = ~(CPURegister::reg[rs].value & CPURegister::reg[rt].value);
	else if(op == "slt") CPURegister::reg[rd].value = (CPURegister::reg[rs].value < CPURegister::reg[rt].value);
	CPURegister::PC.value += 4;
}

void R_format2(string op, int rt, int rd, int C){
	if(op == "sll")	CPURegister::reg[rd].value = CPURegister::reg[rt].value << C;
	else if(op == "srl") CPURegister::reg[rd].value = CPURegister::reg[rt].value >> C;
	else if(op == "sra") CPURegister::reg[rd].value = CPURegister::reg[rt].value >> C; // Need Modified
	CPURegister::PC.value += 4;
}

void R_format3(string op, int rs){
	CPURegister::PC.value = CPURegister::reg[rs].value;
}

void I_format(string op, int rs, int rt, int C){
	if(op == "addi") CPURegister::reg[rt].value = CPURegister::reg[rs].value + C;
	else if(op == "addiu") CPURegister::reg[rt].value = CPURegister::reg[rs].value + C; // Need Modified
	else if(op == "lw"){
		int Word[32];
		for(int i=4; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[rs+C+(4-i)][7-j];
		CPURegister::reg[rt].value = Bin2Dec(Word, 32);
	}
	else if(op == "lh"){ // Need Modified
		int Word[16];
		for(int i=2; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[rs+C+(2-i)][7-j];
		CPURegister::reg[rt].value = Bin2Dec(Word, 16);
	} 
	else if(op == "lhu"){ // Need Modified
		int Word[16];
		for(int i=2; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[rs+C+(2-i)][7-j];
		CPURegister::reg[rt].value = Bin2Dec(Word, 16);
	}
	else if(op == "lb"){ // Need Modified
		int Word[8];
		for(int i=0; i<8; i++)
			Word[i] = DataMemory::Memory[rs+C][i];
		CPURegister::reg[rt].value = Bin2Dec(Word, 8);
	}
	else if(op == "lbu"){ // Need Modified
		int Word[8];
		for(int i=0; i<8; i++)
			Word[i] = DataMemory::Memory[rs+C][i];
		CPURegister::reg[rt].value = Bin2Dec(Word, 8);
	}
	else if(op == "sw"){
		bitset<32> bs;
		bs = CPURegister::reg[rt].value;
		for(int i=4; i>0; i--)
			for(int j=7; j>=0; j--)
				DataMemory::Memory[rs+C+(4-i)][j] = bs[i*8-1-(7-j)];
	}	
	else if(op == "sh"){ // Need Modified
		bitset<32> bs;
		bs = CPURegister::reg[rt].value;
		for(int i=2; i>0; i--)
			for(int j=7; j>=0; j--)
				DataMemory::Memory[rs+C+(2-i)][j] = bs[i*8-1-(7-j)];
	}
	else if(op == "sb"){ // Need Modified
		bitset<32> bs;
		bs = CPURegister::reg[rt].value;
		for(int i=7; i>=0; i--)
			DataMemory::Memory[rs+C][i] = bs[i];
	}
	else if(op == "andi") CPURegister::reg[rt].value = CPURegister::reg[rs].value & C;
	else if(op == "ori") CPURegister::reg[rt].value = CPURegister::reg[rs].value | C;
	else if(op == "nori") CPURegister::reg[rt].value = CPURegister::reg[rs].value ^ C;
	else if(op == "slti") CPURegister::reg[rt].value = (CPURegister::reg[rs].value < C);
	else if(op == "beq"){ // Need Modified
		if(CPURegister::reg[rs].value==CPURegister::reg[rt].value){
			CPURegister::PC.value = CPURegister::PC.value + 4 + 4*C;
			return;
		}
	}
	else if(op == "bne"){ // Need Modified
		if(CPURegister::reg[rs].value!=CPURegister::reg[rt].value){
			CPURegister::PC.value = CPURegister::PC.value + 4 + 4*C;
			return;
		}
	}
	CPURegister::PC.value += 4;
}

void I_format2(string op, int rt, int C){
	CPURegister::reg[rt].value = C << 16;
}

void I_format3(string op, int rs, int C){
	if(CPURegister::reg[rs].value > 0) CPURegister::PC.value = CPURegister::PC.value + 4 + 4*C; // Need Modified
}

void J_format(string op, int C){
	// Need Modified
}