/***************************************************

	File: Binary2Assembly.h

	Author: PinYo

***************************************************/
#ifndef Binary2Assembly_h
#define Binary2Assembly_h

#include "Register.h"
#include <vector>

void Binary2Assembly(std::vector<int> Word, Register *reg, Register &PC);

#endif