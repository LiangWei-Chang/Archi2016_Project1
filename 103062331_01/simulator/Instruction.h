/***************************************************

	File: Instruction.h

	Author: PinYo

***************************************************/
#ifndef Instruction_h
#define Instruction_h

#include <string>
using namespace std;

void R_format(string op, int rs, int rt, int rd);
void R_format2(string op, int rt, int rd, int C);
void R_format3(string op, int rs);
void I_format(string op, int rs, int rt, int C);
void I_format2(string op, int rt, int C);
void I_format3(string op, int rs, int C);
void J_format(string op, unsigned int C);

#endif