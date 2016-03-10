/***************************************************

	File: GlobalVar.h

	Author: PinYo

***************************************************/
#ifndef GlobalVar_h
#define GlobalVar_h

#include <map>
#include <vector>
#include "Register.h"

using namespace std;

class DataMemory{
public: 
	static map< int,vector<int> > Memory;
};

class CPURegister{
public:
	static Register reg[32], PC;
};
#endif