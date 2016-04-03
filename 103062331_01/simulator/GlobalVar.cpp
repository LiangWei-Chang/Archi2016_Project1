/***************************************************

	File: GlobalVar.cpp

	Author: PinYo

***************************************************/

#include "GlobalVar.h"

map< int,char > DataMemory::Memory;
int CPURegister::reg[32];
int CPURegister::PC;
bool Terminal::Halt;
bool Terminal::error_type[4];