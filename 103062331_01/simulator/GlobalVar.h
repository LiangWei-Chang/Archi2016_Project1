/***************************************************

	File: GlobalVar.h

	Author: PinYo

***************************************************/
#ifndef GlobalVar_h
#define GlobalVar_h

#include <map>
#include <vector>

using namespace std;

class DataMemory{
public: 
	static map< int,char > Memory;
};

class CPURegister{
public:
	static int reg[32], PC;
};

class Terminal{
public:
	static bool Halt;
	static bool error_type[4];
};
#endif