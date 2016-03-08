/***************************************************

	File: Instruction.h

	Author: PinYo

***************************************************/
#ifndef Instruction_h
#define Instruction_h

#include "Register.h"
#include <string>
using namespace std;

void R_format(string op, int rs, int rt, int rd, Register *reg, Register &PC);
void R_format2(string op, int rt, int rd, int C, Register *reg, Register &PC);
void R_format3(string op, int rs, Register *reg, Register &PC);
void I_format(string op, int rs, int rt, int C, Register *reg, Register &PC);
void I_format2(string op, int rt, int C, Register *reg, Register &PC);
void I_format3(string op, int rs, int C, Register *reg, Register &PC);
void J_format(string op, int C, Register *reg, Register &PC);

#endif