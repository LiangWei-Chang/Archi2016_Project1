/***************************************************

	File: Instruction.cpp

	Author: PinYo

***************************************************/

#include "Instruction.h"
#include "GlobalVar.h"
#include <vector>
#include <bitset>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

unsigned int Bin2Dec(int *Word, int Bits, bool Signed){ // **Consider negative num**
	int sum = 0, power = 1, neg[32];
	memset(neg, 0, sizeof(neg));
	if(Signed){
		if(Word[Bits-1]==1){
			for(int i=0; i<Bits; i++)
				neg[i] = !Word[i];
			int carry = 1, temp;
			for(int i=0; i<Bits; i++){
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
	else if(op == "sra"){
		string StrHex = CPURegister::reg[rd].Hex();
		int SignBit = StrHex[2]-'0', Word[32];
		int temp = CPURegister::reg[rt].value >> C;
		bitset<32> bs = temp;
		for(int i=31; i>(31-C); i--) bs[i] = SignBit;
		for(int i=31; i>=0; i--) Word[i] = bs[i];
		CPURegister::reg[rd].value = Bin2Dec(Word, 32, true);
	}
	CPURegister::PC.value += 4;
}

void R_format3(string op, int rs){
	CPURegister::PC.value = CPURegister::reg[rs].value;
}

void I_format(string op, int rs, int rt, int C){
	if(op == "addi") CPURegister::reg[rt].value = CPURegister::reg[rs].value + C;
	else if(op == "addiu"){
		unsigned int C2 = C;
		CPURegister::reg[rt].value = CPURegister::reg[rs].value + C2;
	}
	else if(op == "lw"){
		int Word[32];
		memset(Word, 0, sizeof(Word));
		for(int i=4; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[rs+C+(4-i)][7-j];
		CPURegister::reg[rt].value = Bin2Dec(Word, 32, true);
	}
	else if(op == "lh"){ 
		int Word[32];
		memset(Word, 0, sizeof(Word));
		for(int i=2; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[rs+C+(2-i)][7-j];
		CPURegister::reg[rt].value = Bin2Dec(Word, 16, true);
	} 
	else if(op == "lhu"){ 
		int Word[32];
		memset(Word, 0, sizeof(Word));
		for(int i=2; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[rs+C+(2-i)][7-j];
		CPURegister::reg[rt].value = Bin2Dec(Word, 16, false);
	}
	else if(op == "lb"){ 
		int Word[32];
		memset(Word, 0, sizeof(Word));
		for(int i=0; i<8; i++)
			Word[i] = DataMemory::Memory[rs+C][i];
		CPURegister::reg[rt].value = Bin2Dec(Word, 8, true);
	}
	else if(op == "lbu"){
		int Word[32];
		memset(Word, 0, sizeof(Word));
		for(int i=0; i<8; i++)
			Word[i] = DataMemory::Memory[rs+C][i];
		CPURegister::reg[rt].value = Bin2Dec(Word, 8, false);
	}
	else if(op == "sw"){
		bitset<32> bs;
		bs = CPURegister::reg[rt].value;
		for(int i=4; i>0; i--)
			for(int j=7; j>=0; j--)
				DataMemory::Memory[rs+C+(4-i)][j] = bs[i*8-1-(7-j)];
	}	
	else if(op == "sh"){ 
		bitset<32> bs;
		bs = CPURegister::reg[rt].value;
		for(int i=2; i>0; i--)
			for(int j=7; j>=0; j--)
				DataMemory::Memory[rs+C+(2-i)][j] = bs[i*8-1-(7-j)];
	}
	else if(op == "sb"){
		bitset<32> bs;
		bs = CPURegister::reg[rt].value;
		for(int i=7; i>=0; i--)
			DataMemory::Memory[rs+C][i] = bs[i];
	}
	else if(op == "andi"){
		unsigned int C2 = C;
		CPURegister::reg[rt].value = CPURegister::reg[rs].value & C2;
	}
	else if(op == "ori"){
		unsigned int C2 = C;
		CPURegister::reg[rt].value = CPURegister::reg[rs].value | C2;
	}
	else if(op == "nori"){
		unsigned int C2 = C;
		CPURegister::reg[rt].value = CPURegister::reg[rs].value ^ C2;
	}
	else if(op == "slti") CPURegister::reg[rt].value = (CPURegister::reg[rs].value < C);
	else if(op == "beq"){ 
		if(CPURegister::reg[rs].value==CPURegister::reg[rt].value){
			CPURegister::PC.value = CPURegister::PC.value + 4 + 4*C;
			return;
		}
	}
	else if(op == "bne"){ 
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
	if(CPURegister::reg[rs].value > 0) CPURegister::PC.value = CPURegister::PC.value + 4 + 4*C;
}

void J_format(string op, unsigned int C){
	// Need Modified
}