/***************************************************

	File: Memory.h

	Author: PinYo

***************************************************/
#ifndef Memory_h
#define Memory_h

#include <map>
#include <vector>
using namespace std;

class DataMemory{
public: 
	static map< int,vector<int> > Memory;
};

#endif