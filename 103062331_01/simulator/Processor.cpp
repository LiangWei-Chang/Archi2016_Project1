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
#include "GlobalVar.h"

using namespace std;

string Message[4] = {": Write $0 Error", ": Number Overflow", ": Address Overflow", ": Misalignment Error"};

void InitialReg(){
	for(int i=0; i<32; i++)
		CPURegister::reg[i] = 0;
}

void cyclePrint(ofstream &fout, int &Cycle){
	fout << "cycle " << dec << Cycle++ << endl;
	for(int i=0; i<32; i++){
		fout << "$" << setw(2) << setfill('0') << dec << i;
		fout << ": 0x" << setw(8) << setfill('0') << hex << uppercase << CPURegister::reg[i] << endl;
	}
	fout << "PC: 0x" << setw(8) << setfill('0') << hex << uppercase << CPURegister::PC << endl << endl << endl;
}

int main(){
	char ch;
	int Word = 0, bytes = 4, Cycle = 0, idx = -2;
	map< int,int > Address;
	// Initialize register;
	InitialReg();
	
	ofstream fout("snapshot.rpt", ios::out);
	ofstream Errorout("error_dump.rpt", ios::out);
	bitset<32> bs;
	// Read iimage.bin
	ifstream fin("iimage.bin", ios::in | ios::binary);
	if(!fin){
		cout << "Error to load 'iimage.bin'!\n";
		return 0;
	}
	while(!fin.eof()){
		fin.get(ch);
		Word = (Word << 8) | (unsigned char)ch;
		bytes--;
		if(bytes==0){
			bytes = 4;
			if(idx==-2){
				CPURegister::PC = Word;
				idx++;
				continue;
			}
			else if(idx==-1){
				idx++;
				continue;
			}
			Address[CPURegister::PC+idx*4] = Word;
			idx++;
		}
	}
	fin.close();
	
	// Read dimage.bin
	fin.open("dimage.bin", ios::in | ios::binary);
	// Read $sp
	for(int i=4; i>0; i--){
		fin.get(ch);
		Word = (Word << 8) + (unsigned char)ch;
	}
	CPURegister::reg[29] = Word;
	// Numbers of words
	for(int i=4; i>0; i--){
		fin.get(ch);
		Word = (Word << 8) + (unsigned char)ch;
	}
	int NumbersOfWords = Word;
	for(int i=0; i<NumbersOfWords*4; i++){
		fin.get(ch);
		DataMemory::Memory[i] = ch;
	}
	cyclePrint(fout, Cycle);
	Terminal::Halt = false;
	
	//Start Instructions
	while(!Terminal::Halt){
		for(int i=0; i<4; i++) Terminal::error_type[i] = false;
		Binary2Assembly(Address[CPURegister::PC]);
		for(int i=0; i<4; i++){
			if(Terminal::error_type[i]==true)
				Errorout << "In cycle " << Cycle << Message[i] << endl;
		}
		if(Terminal::Halt==true)
			break;
		cyclePrint(fout, Cycle);
	}
	fout.close();
	Errorout.close();
	return 0;
}