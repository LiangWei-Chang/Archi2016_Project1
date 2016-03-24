/***************************************************

	File: GlobalVar.cpp

	Author: PinYo

***************************************************/

#include "GlobalVar.h"

map< int,char > DataMemory::Memory;
Register CPURegister::reg[32];
Register CPURegister::PC;
bool Terminal::Halt;
bool Terminal::error_type[4];