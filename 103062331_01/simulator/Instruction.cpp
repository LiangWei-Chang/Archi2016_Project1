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

void NumberOverflowDetect(int temp, int A, int B){
	bitset<32> bs1 = temp, bs2 = A, bs3 = B;
	if(bs2[31]==bs3[31] && bs2[31]!=bs1[31])
		Terminal::error_type[1] = true;
}

void AddressOverflowDetect(int K){
	if(K >= 1024 || K < 0){
		Terminal::error_type[2] = true;
		Terminal::Halt = true;
	}
}

void DataMisaligned(int K, int bytes){
	if(K % bytes != 0){
		Terminal::error_type[3] = true;
		Terminal::Halt = true;
	}
}

void R_format(string op, int rs, int rt, int rd){
	if(rd==0){
		Terminal::error_type[0] = true;
		CPURegister::PC.value += 4;
		return;
	}
	if(op == "add"){
		int temp = CPURegister::reg[rs].value + CPURegister::reg[rt].value;
		NumberOverflowDetect(temp, CPURegister::reg[rs].value, CPURegister::reg[rt].value);
		CPURegister::reg[rd].value = temp;
	}
	else if(op == "addu") CPURegister::reg[rd].value = CPURegister::reg[rs].value + CPURegister::reg[rt].value;
	else if(op == "sub"){
		int temp = CPURegister::reg[rs].value - CPURegister::reg[rt].value;
		NumberOverflowDetect(temp, CPURegister::reg[rs].value, CPURegister::reg[rt].value);
		CPURegister::reg[rd].value = temp;
	}
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
	else if(op == "srl"){
		if(rd==0){
			Terminal::error_type[0] = true;
			CPURegister::PC.value += 4;
			return;
		}
		int Word[32], temp = CPURegister::reg[rt].value >> C;
		bitset<32> bs = temp;
		for(int i=31; i>(31-C); i--) bs[i] = 0;
		for(int i=31; i>=0; i--) Word[i] = bs[i];
		CPURegister::reg[rd].value = Bin2Dec(Word, 32, true);
	}
	else if(op == "sra"){
		if(rd==0){
			Terminal::error_type[0] = true;
			CPURegister::PC.value += 4;
			return;
		}
		CPURegister::reg[rd].value = CPURegister::reg[rt].value >> C;
	}
	CPURegister::PC.value += 4;
}

void R_format3(string op, int rs){
	CPURegister::PC.value = CPURegister::reg[rs].value;
}

void I_format(string op, int rs, int rt, int C){
	if(rt==0 && op!="sw" && op!="sh" && op!="sb" && op!="beq" && op!="bne"){
		Terminal::error_type[0] = true;
		CPURegister::PC.value += 4;
		return;
	}
	if(op == "addi"){
		int temp = CPURegister::reg[rs].value + C;
		NumberOverflowDetect(temp, CPURegister::reg[rs].value, C);
		CPURegister::reg[rt].value = temp;
	}
	else if(op == "addiu"){
		unsigned int C2 = C;
		CPURegister::reg[rt].value = CPURegister::reg[rs].value + C2;
	}
	else if(op == "lw"){
		int Word[32];
		memset(Word, 0, sizeof(Word));
		NumberOverflowDetect(CPURegister::reg[rs].value+C+3, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C+3);
		DataMisaligned(CPURegister::reg[rs].value+C, 4);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		for(int i=4; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[CPURegister::reg[rs].value+C+(4-i)][7-j];
		CPURegister::reg[rt].value = Bin2Dec(Word, 32, true);
	}
	else if(op == "lh"){ 
		int Word[32];
		memset(Word, 0, sizeof(Word));
		NumberOverflowDetect(CPURegister::reg[rs].value+C+1, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C+1);
		DataMisaligned(CPURegister::reg[rs].value+C, 2);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		for(int i=2; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[CPURegister::reg[rs].value+C+(2-i)][7-j];
		CPURegister::reg[rt].value = Bin2Dec(Word, 16, true);
	} 
	else if(op == "lhu"){ 
		int Word[32];
		memset(Word, 0, sizeof(Word));
		NumberOverflowDetect(CPURegister::reg[rs].value+C+1, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C+1);
		DataMisaligned(CPURegister::reg[rs].value+C, 2);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		for(int i=2; i>0; i--)
			for(int j=0; j<8; j++)
				Word[i*8-1-j] = DataMemory::Memory[CPURegister::reg[rs].value+C+(2-i)][7-j];
		CPURegister::reg[rt].value = Bin2Dec(Word, 16, false);
	}
	else if(op == "lb"){ 
		int Word[32];
		memset(Word, 0, sizeof(Word));
		NumberOverflowDetect(CPURegister::reg[rs].value+C, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C);
		if(Terminal::error_type[2]) return;
		for(int i=0; i<8; i++)
			Word[i] = DataMemory::Memory[CPURegister::reg[rs].value+C][i];
		CPURegister::reg[rt].value = Bin2Dec(Word, 8, true);
	}
	else if(op == "lbu"){
		int Word[32];
		memset(Word, 0, sizeof(Word));
		NumberOverflowDetect(CPURegister::reg[rs].value+C, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C);
		if(Terminal::error_type[2]) return;
		for(int i=0; i<8; i++)
			Word[i] = DataMemory::Memory[CPURegister::reg[rs].value+C][i];
		CPURegister::reg[rt].value = Bin2Dec(Word, 8, false);
	}
	else if(op == "sw"){
		bitset<32> bs;
		bs = CPURegister::reg[rt].value;
		NumberOverflowDetect(CPURegister::reg[rs].value+C+3, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C+3);
		DataMisaligned(CPURegister::reg[rs].value+C, 4);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		for(int i=4; i>0; i--){
			DataMemory::Memory[CPURegister::reg[rs].value+C+(4-i)].clear();
			for(int j=0; j<8; j++)
				DataMemory::Memory[CPURegister::reg[rs].value+C+(4-i)].push_back(bs[(i-1)*8+j]);
		}
	}	
	else if(op == "sh"){ 
		bitset<32> bs;
		bs = CPURegister::reg[rt].value;
		NumberOverflowDetect(CPURegister::reg[rs].value+C+1, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C+1);
		DataMisaligned(CPURegister::reg[rs].value+C, 2);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		for(int i=2; i>0; i--){
			DataMemory::Memory[CPURegister::reg[rs].value+C+(2-i)].clear();
			for(int j=0; j<8; j++)
				DataMemory::Memory[CPURegister::reg[rs].value+C+(2-i)].push_back(bs[(i-1)*8+j]);
		}
	}
	else if(op == "sb"){
		bitset<32> bs;
		bs = CPURegister::reg[rt].value;
		NumberOverflowDetect(CPURegister::reg[rs].value+C, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C);
		if(Terminal::error_type[2]) return;
		DataMemory::Memory[CPURegister::reg[rs].value+C].clear();
		for(int j=0; j<8; j++)
				DataMemory::Memory[CPURegister::reg[rs].value+C].push_back(bs[j]);
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
			NumberOverflowDetect(CPURegister::reg[rs].value+4*C+4, CPURegister::reg[rs].value, 4*C);
			CPURegister::PC.value = CPURegister::PC.value + 4 + 4*C;
			return;
		}
	}
	else if(op == "bne"){ 
		if(CPURegister::reg[rs].value!=CPURegister::reg[rt].value){
			NumberOverflowDetect(CPURegister::reg[rs].value+4*C+4, CPURegister::reg[rs].value, 4*C);
			CPURegister::PC.value = CPURegister::PC.value + 4 + 4*C;
			return;
		}
	}
	CPURegister::PC.value += 4;
}

void I_format2(string op, int rt, int C){
	if(rt==0){
		Terminal::error_type[0] = true;
		CPURegister::PC.value += 4;
		return;
	}
	CPURegister::reg[rt].value = C << 16;
}

void I_format3(string op, int rs, int C){
	NumberOverflowDetect(CPURegister::reg[rs].value+4*C+4, CPURegister::reg[rs].value, 4*C);
	if(CPURegister::reg[rs].value > 0) CPURegister::PC.value = CPURegister::PC.value + 4 + 4*C;
}

void J_format(string op, unsigned int C){
	if(op == "jal") CPURegister::reg[31].value = CPURegister::PC.value + 4;	 
	bitset<32> bs1, bs2;
	int Word[32];
	memset(Word, 0, sizeof(Word));
	CPURegister::PC.value += 4;
	C = C << 2;
	bs1 = CPURegister::PC.value;
	bs2 = C;
	for(int i=31; i>27; i--) Word[i] = bs1[i];
	for(int i=27; i>1; i--) Word[i] = bs2[i];
	CPURegister::PC.value = Bin2Dec(Word, 32, false);
}