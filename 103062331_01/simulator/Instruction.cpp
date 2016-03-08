/***************************************************

	File: Instruction.cpp

	Author: PinYo

***************************************************/

#include "Instruction.h"

void R_format(string op, int rs, int rt, int rd, Register *reg){
	if(op == "add")	reg[rd].value = reg[rs].value + reg[rt].value;
	else if(op == "addu") reg[rd].value = reg[rs].value + reg[rt].value; // Need Modified
	else if(op == "sub") reg[rd].value = reg[rs].value - reg[rt].value;
	else if(op == "and") reg[rd].value = reg[rs].value & reg[rt].value;
	else if(op == "or")	reg[rd].value = reg[rs].value | reg[rt].value;
	else if(op == "xor") reg[rd].value = reg[rs].value ^ reg[rt].value;
	else if(op == "nor")	reg[rd].value = ~(reg[rs].value | reg[rt].value);
	else if(op == "nand") reg[rd].value = ~(reg[rs].value & reg[rt].value);
	else if(op == "slt") reg[rd].value = (reg[rs].value < reg[rt].value);
}

void R_format2(string op, int rt, int rd, int C, Register *reg){
	if(op == "sll")	reg[rd].value = reg[rt].value << C;
	else if(op == "srl") reg[rd].value = reg[rt].value >> C;
	else if(op == "sra") reg[rd].value = reg[rt].value >> C; // Need Modified
}

void R_format3(string op, int rs, Register *reg){
	reg[33].value = reg[rs].value;
}

void I_format(string op, int rs, int rt, int C, Register *reg){
	if(op == "addi") reg[rt].value = reg[rs].value + C;
	else if(op == "addiu") reg[rt].value = reg[rs].value + C; // Need Modified
	else if(op == "lw") reg[rt].value = (*(&reg[rs]+C)).value; // Need Modified
	else if(op == "lh") reg[rt].value = (*(&reg[rs]+C)).value; // Need Modified
	else if(op == "lhu") reg[rt].value = (*(&reg[rs]+C)).value; // Need Modified
	else if(op == "lb") reg[rt].value = (*(&reg[rs]+C)).value; // Need Modified
	else if(op == "lbu") reg[rt].value = (*(&reg[rs]+C)).value; // Need Modified
	else if(op == "sw") (*(&reg[rs]+C)).value = reg[rt].value; // Need Modified
	else if(op == "sh") (*(&reg[rs]+C)).value = reg[rt].value; // Need Modified
	else if(op == "sb") reg[rt].value = (*(&reg[rs]+C)).value & 0x000000FF; // Need Modified
	else if(op == "andi") reg[rt].value = reg[rs].value & C;
	else if(op == "ori") reg[rt].value = reg[rs].value | C;
	else if(op == "nori") reg[rt].value = reg[rs].value ^ C;
	else if(op == "slti") reg[rt].value = (reg[rs].value < C);
	else if(op == "beq"); // Need Modified
	else if(op == "bne"); // Need Modified
}

void I_format2(string op, int rt, int C, Register *reg){
	reg[rt].value = C << 16;
}

void I_format3(string op, int rs, int C, Register *reg){
	// Need Modified
}

void J_format(string op, int C, Register *reg){
	// Need Modified
}