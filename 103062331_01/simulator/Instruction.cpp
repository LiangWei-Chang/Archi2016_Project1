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
		CPURegister::reg[rd].value = (unsigned int)CPURegister::reg[rt].value >> C;
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
		NumberOverflowDetect(CPURegister::reg[rs].value+C+3, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C+3);
		DataMisaligned(CPURegister::reg[rs].value+C, 4);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		int Word = 0;
		for(int i=0; i<4; i++)
			Word = (Word << 8) | (unsigned char)DataMemory::Memory[CPURegister::reg[rs].value+C+i];
		CPURegister::reg[rt].value = Word;
	}
	else if(op == "lh"){ 
		NumberOverflowDetect(CPURegister::reg[rs].value+C+1, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C+1);
		DataMisaligned(CPURegister::reg[rs].value+C, 2);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		int Word = DataMemory::Memory[CPURegister::reg[rs].value+C];
		for(int i=1; i<2; i++)
			Word = (Word << 8) | (unsigned char)DataMemory::Memory[CPURegister::reg[rs].value+C+i];
		CPURegister::reg[rt].value = Word;
	} 
	else if(op == "lhu"){ 
		NumberOverflowDetect(CPURegister::reg[rs].value+C+1, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C+1);
		DataMisaligned(CPURegister::reg[rs].value+C, 2);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		int Word = 0;
		for(int i=0; i<2; i++)
			Word = (Word << 8) | (unsigned char)DataMemory::Memory[CPURegister::reg[rs].value+C+i];
		CPURegister::reg[rt].value = Word;
	}
	else if(op == "lb"){ 
		NumberOverflowDetect(CPURegister::reg[rs].value+C, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C);
		if(Terminal::error_type[2]) return;
		CPURegister::reg[rt].value = DataMemory::Memory[CPURegister::reg[rs].value+C];
	}
	else if(op == "lbu"){
		NumberOverflowDetect(CPURegister::reg[rs].value+C, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C);
		if(Terminal::error_type[2]) return;
		CPURegister::reg[rt].value = (unsigned char)DataMemory::Memory[CPURegister::reg[rs].value+C];
	}
	else if(op == "sw"){
		NumberOverflowDetect(CPURegister::reg[rs].value+C+3, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C+3);
		DataMisaligned(CPURegister::reg[rs].value+C, 4);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		for(int i=0; i<4; i++)
			DataMemory::Memory[CPURegister::reg[rs].value+C+i] = (char)(CPURegister::reg[rt].value >> (8*(3-i)));
	}	
	else if(op == "sh"){
		NumberOverflowDetect(CPURegister::reg[rs].value+C+1, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C+1);
		DataMisaligned(CPURegister::reg[rs].value+C, 2);
		if(Terminal::error_type[2] || Terminal::error_type[3]) return;
		for(int i=0; i<2; i++)
			DataMemory::Memory[CPURegister::reg[rs].value+C+i] = (char)(CPURegister::reg[rt].value >> (8*(1-i)));
	}
	else if(op == "sb"){
		NumberOverflowDetect(CPURegister::reg[rs].value+C, CPURegister::reg[rs].value, C);
		AddressOverflowDetect(CPURegister::reg[rs].value+C);
		if(Terminal::error_type[2]) return;
		DataMemory::Memory[CPURegister::reg[rs].value+C] = (char)CPURegister::reg[rt].value;
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
	int Word = ((CPURegister::PC.value+4) >> 28) << 28;
	Word += (C << 2);;
	CPURegister::PC.value = Word;
}