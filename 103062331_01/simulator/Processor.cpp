/***************************************************

	File: Processor.cpp

	Author: PinYo

***************************************************/

#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <iomanip>
#include <map>
#include <vector>
#include "Binary2Assembly.h"
#include "Register.h"

using namespace std;

void InitialReg(Register *reg){
	for(int i=0; i<32; i++)
		reg[i].value = 0;
}

void cyclePrint(ofstream &fout, int &Cycle, Register *reg, Register PC){
	fout << "cycle " << Cycle++ << endl;
	for(int i=0; i<32; i++){
		fout << "$" << setw(2) << setfill('0') << i;
		fout << ": " << reg[i].Hex() << endl;
	}
	fout << "PC: " << PC.Hex() << endl << endl << endl;
}

int Bin2Dec(int *Word){ // **Consider negative num**
	int sum = 0, power = 1;
	for(int i=0; i<32; i++){
		sum += (Word[i] * power);
		power *= 2;
	}
	return sum;
}

// reg[33] represent PC;

int main(){
	char ch;
	int Word[32], bytes = 4, Cycle = 0, idx = -2, next_addr;
	Register reg[32], PC;
	bool Halt = false;
	bitset<8> bs;
	map< int, vector<int> > Address;
	map< int, vector<int> > Memory;
	// Initialize register;
	InitialReg(reg);
	
	ofstream fout("../testcase/snapshot.rpt", ios::out);
	
	// Read iimage.bin
	ifstream fin("../testcase/iimage.bin", ios::in | ios::binary);
	if(!fin) cout << "Error to load 'iimage.bin'!\n";
	while(!fin.eof()){
		fin.get(ch);
		bs = ch;
		for(int i=0; i<8; i++)
			Word[bytes*8-1-i] = bs[7-i];
		bytes--;
		if(bytes==0){
			bytes = 4;
			if(idx==-2){
				PC.value = Bin2Dec(Word);
				idx++;
				continue;
			}
			else if(idx==-1){
				idx++;
				continue;
			}
			for(int i=0; i<32; i++)	Address[PC.value+idx*4].push_back(Word[i]);
			idx++;
		}
	}
	fin.close();
	
	// Read dimage.bin
	fin.open("../testcase/dimage.bin", ios::in | ios::binary);
	// Read $sp
	for(int i=4; i>0; i--){
		fin.get(ch);
		bs = ch;
		for(int j=0; j<8; j++)
			Word[i*8-1-j] = bs[7-j];
	}
	reg[29].value = Bin2Dec(Word);
	// Numbers of words
	for(int i=4; i>0; i--){
		fin.get(ch);
		bs = ch;
		for(int j=0; j<8; j++)
			Word[i*8-1-j] = bs[7-j];
	}
	int NumbersOfWords = Bin2Dec(Word);
	for(int i=0; i<NumbersOfWords*4; i++){
		fin.get(ch);
		bs = ch;
		for(int j=0; j<8; j++)
			Memory[i].push_back(bs[j]);
	}
	cyclePrint(fout, Cycle, reg, PC);
	while(!Halt){
		Binary2Assembly(Address[PC.value], reg, PC);
		if(PC.value==0xFFFF){
			Halt = true;
			return 0;
		}
		cyclePrint(fout, Cycle, reg, PC);
	}
	return 0;
}