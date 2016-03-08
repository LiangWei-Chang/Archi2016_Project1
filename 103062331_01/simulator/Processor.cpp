/***************************************************

	File: Processor.cpp

	Author: PinYo

***************************************************/

#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <iomanip>
#include "Binary2Assembly.h"
#include "Register.h"

using namespace std;

void InitialReg(Register *reg){
	for(int i=0; i<33; i++)
		reg[i].value = 0;
}

int main(){
	char ch;
	int Word[32], bytes = 4, Cycle = 0;
	Register reg[33];
	bool Halt = false;
	bitset<8> bs;
	ifstream fin("../testcase/iimage.bin", ios::in | ios::binary);
	ofstream fout("../testcase/snapshot.rpt", ios::out);
	if(!fin) cout << "Error to load 'iimage.bin'!\n";
	// Initialize register;
	InitialReg(reg);
	while(!fin.eof()){
		fin.get(ch);
		bs = ch;
		for(int i=0; i<8; i++)
			Word[bytes*8-1-i] = bs[i];
		bytes--;
		if(bytes==0){
			Halt = Binary2Assembly(Word, reg);
			if(Halt) return 0;
			bytes = 4;
			fout << "cycle " << Cycle++ << endl;
			for(int i=0; i<32; i++){
				fout << "$" << setw(2) << setfill('0') << i;
				fout << ": " << reg[i].Hex() << endl;
			}
			fout << "PC: " << reg[33].Hex() << endl << endl << endl;
		}
	}
	return 0;
}