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

void NumberOverflowDetect(int temp, int A, int B){
	bitset<32> bs1 = temp, bs2 = A, bs3 = B;
	if(bs2[31]==bs3[31] && bs2[31]!=bs1[31])
		Terminal::error_type[1] = true;
}

void AddressOverflowDetect(int K, int bytes){
	if(K < 0 || K >= 1024 || K+bytes >= 1024 || K+bytes < 0){
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
		CPURegister::PC += 4;
	}
	if(op == "add"){
		int temp = CPURegister::reg[rs] + CPURegister::reg[rt];
		NumberOverflowDetect(temp, CPURegister::reg[rs], CPURegister::reg[rt]);
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rd] = temp;
	}
	else if(op == "addu"){
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rd] = CPURegister::reg[rs] + CPURegister::reg[rt];
	}
	else if(op == "sub"){
		int temp = CPURegister::reg[rs] - CPURegister::reg[rt];
		NumberOverflowDetect(temp, CPURegister::reg[rs], (~CPURegister::reg[rt])+1);
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rd] = temp;
	}
	else if(op == "and"){
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rd] = CPURegister::reg[rs] & CPURegister::reg[rt];
	}
	else if(op == "or"){
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rd] = CPURegister::reg[rs] | CPURegister::reg[rt];
	}
	else if(op == "xor"){
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rd] = CPURegister::reg[rs] ^ CPURegister::reg[rt];
	}
	else if(op == "nor"){
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rd] = ~(CPURegister::reg[rs] | CPURegister::reg[rt]);
	}
	else if(op == "nand"){
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rd] = ~(CPURegister::reg[rs] & CPURegister::reg[rt]);
	}
	else if(op == "slt"){
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rd] = (CPURegister::reg[rs] < CPURegister::reg[rt]);
	}
	CPURegister::PC += 4;
}

void R_format2(string op, int rt, int rd, int C){
	if(op == "sll"){
		if(rd==0){
			if(rt!=0 || C!=0) Terminal::error_type[0] = true;
			CPURegister::PC += 4;
			return;
		}
		CPURegister::reg[rd] = CPURegister::reg[rt] << C;
	}
	else if(op == "srl"){
		if(rd==0){
			Terminal::error_type[0] = true;
			CPURegister::PC += 4;
			return;
		}
		CPURegister::reg[rd] = (unsigned int)CPURegister::reg[rt] >> C;
	}
	else if(op == "sra"){
		if(rd==0){
			Terminal::error_type[0] = true;
			CPURegister::PC += 4;
			return;
		}
		CPURegister::reg[rd] = CPURegister::reg[rt] >> C;
	}
	CPURegister::PC += 4;
}

void R_format3(string op, int rs){
	CPURegister::PC = CPURegister::reg[rs];
}

void I_format(string op, int rs, int rt, int C){
	if(rt==0 && op!="sw" && op!="sh" && op!="sb" && op!="beq" && op!="bne"){
		Terminal::error_type[0] = true;
		CPURegister::PC += 4;
	}
	if(op == "addi"){
		int temp = CPURegister::reg[rs] + C;
		NumberOverflowDetect(temp, CPURegister::reg[rs], C);
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rt] = temp;
	}
	else if(op == "addiu"){
		unsigned int C2 = C;
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rt] = CPURegister::reg[rs] + C2;
	}
	else if(op == "lw"){
		NumberOverflowDetect(CPURegister::reg[rs]+C, CPURegister::reg[rs], C);
		AddressOverflowDetect(CPURegister::reg[rs]+C, 3);
		DataMisaligned(CPURegister::reg[rs]+C, 4);
		if(Terminal::error_type[0] || Terminal::error_type[2] || Terminal::error_type[3]) return;
		int Word = 0;
		for(int i=0; i<4; i++)
			Word = (Word << 8) | (unsigned char)DataMemory::Memory[CPURegister::reg[rs]+C+i];
		CPURegister::reg[rt] = Word;
	}
	else if(op == "lh"){ 
		NumberOverflowDetect(CPURegister::reg[rs]+C, CPURegister::reg[rs], C);
		AddressOverflowDetect(CPURegister::reg[rs]+C, 1);
		DataMisaligned(CPURegister::reg[rs]+C, 2);
		if(Terminal::error_type[0] || Terminal::error_type[2] || Terminal::error_type[3]) return;
		int Word = DataMemory::Memory[CPURegister::reg[rs]+C];
		for(int i=1; i<2; i++)
			Word = (Word << 8) | (unsigned char)DataMemory::Memory[CPURegister::reg[rs]+C+i];
		CPURegister::reg[rt] = Word;
	} 
	else if(op == "lhu"){ 
		NumberOverflowDetect(CPURegister::reg[rs]+C, CPURegister::reg[rs], C);
		AddressOverflowDetect(CPURegister::reg[rs]+C, 1);
		DataMisaligned(CPURegister::reg[rs]+C, 2);
		if(Terminal::error_type[0] || Terminal::error_type[2] || Terminal::error_type[3]) return;
		int Word = 0;
		for(int i=0; i<2; i++)
			Word = (Word << 8) | (unsigned char)DataMemory::Memory[CPURegister::reg[rs]+C+i];
		CPURegister::reg[rt] = Word;
	}
	else if(op == "lb"){ 
		NumberOverflowDetect(CPURegister::reg[rs]+C, CPURegister::reg[rs], C);
		AddressOverflowDetect(CPURegister::reg[rs]+C, 0);
		if(Terminal::error_type[0] || Terminal::error_type[2]) return;
		CPURegister::reg[rt] = DataMemory::Memory[CPURegister::reg[rs]+C];
	}
	else if(op == "lbu"){
		NumberOverflowDetect(CPURegister::reg[rs]+C, CPURegister::reg[rs], C);
		AddressOverflowDetect(CPURegister::reg[rs]+C, 0);
		if(Terminal::error_type[0] || Terminal::error_type[2]) return;
		CPURegister::reg[rt] = (unsigned char)DataMemory::Memory[CPURegister::reg[rs]+C];
	}
	else if(op == "sw"){
		NumberOverflowDetect(CPURegister::reg[rs]+C, CPURegister::reg[rs], C);
		AddressOverflowDetect(CPURegister::reg[rs]+C, 3);
		DataMisaligned(CPURegister::reg[rs]+C, 4);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		for(int i=0; i<4; i++)
			DataMemory::Memory[CPURegister::reg[rs]+C+i] = (char)(CPURegister::reg[rt] >> (8*(3-i)));
	}	
	else if(op == "sh"){
		NumberOverflowDetect(CPURegister::reg[rs]+C, CPURegister::reg[rs], C);
		AddressOverflowDetect(CPURegister::reg[rs]+C, 1);
		DataMisaligned(CPURegister::reg[rs]+C, 2);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		for(int i=0; i<2; i++)
			DataMemory::Memory[CPURegister::reg[rs]+C+i] = (char)(CPURegister::reg[rt] >> (8*(1-i)));
	}
	else if(op == "sb"){
		NumberOverflowDetect(CPURegister::reg[rs]+C, CPURegister::reg[rs], C);
		AddressOverflowDetect(CPURegister::reg[rs]+C, 0);
		if(Terminal::error_type[2]) return;
		DataMemory::Memory[CPURegister::reg[rs]+C] = (char)CPURegister::reg[rt];
	}
	else if(op == "andi"){
		unsigned int C2 = C;
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rt] = CPURegister::reg[rs] & C2;
	}
	else if(op == "ori"){
		unsigned int C2 = C;
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rt] = CPURegister::reg[rs] | C2;
	}
	else if(op == "nori"){
		unsigned int C2 = C;
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rt] = ~(CPURegister::reg[rs] | C2);
	}
	else if(op == "slti"){
		if(Terminal::error_type[0]) return;
		CPURegister::reg[rt] = (CPURegister::reg[rs] < C);
	}
	else if(op == "beq"){ 
		if(CPURegister::reg[rs]==CPURegister::reg[rt]){
			NumberOverflowDetect(CPURegister::PC+4, CPURegister::PC, 4);
			NumberOverflowDetect(CPURegister::PC+4*C+4, CPURegister::PC+4, 4*C);
			CPURegister::PC = CPURegister::PC + 4 + 4*C;
			return;
		}
	}
	else if(op == "bne"){ 
		if(CPURegister::reg[rs]!=CPURegister::reg[rt]){
			NumberOverflowDetect(CPURegister::PC+4, CPURegister::PC, 4);
			NumberOverflowDetect(CPURegister::PC+4*C+4, CPURegister::PC+4, 4*C);
			CPURegister::PC = CPURegister::PC + 4 + 4*C;
			return;
		}
	}
	CPURegister::PC += 4;
}

void I_format2(string op, int rt, int C){
	if(rt==0){
		Terminal::error_type[0] = true;
		CPURegister::PC += 4;
		return;
	}
	CPURegister::reg[rt] = C << 16;
	CPURegister::PC += 4;
}

void I_format3(string op, int rs, int C){
	NumberOverflowDetect(CPURegister::PC+4, CPURegister::PC, 4);
	NumberOverflowDetect(CPURegister::PC+4*C+4, CPURegister::PC+4, 4*C);
	if(CPURegister::reg[rs] > 0) CPURegister::PC = CPURegister::PC + 4 + 4*C;
	else CPURegister::PC += 4;
}

void J_format(string op, unsigned int C){
	if(op == "jal") CPURegister::reg[31] = CPURegister::PC + 4;	 
	int Word = ((CPURegister::PC+4) >> 28) << 28;
	Word += (C << 2);;
	CPURegister::PC = Word;
}