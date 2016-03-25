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
#include "GlobalVar.h"

using namespace std;

string Message[4] = {": Write $0 Error", ": Number Overflow", ": Address Overflow", ": Misalignment Error"};

void InitialReg(){
	for(int i=0; i<32; i++)
		CPURegister::reg[i].value = 0;
}

void cyclePrint(ofstream &fout, int &Cycle){
	fout << "cycle " << Cycle++ << endl;
	for(int i=0; i<32; i++){
		fout << "$" << setw(2) << setfill('0') << i;
		fout << ": " << CPURegister::reg[i].Hex() << endl;
	}
	fout << "PC: " << CPURegister::PC.Hex() << endl << endl << endl;
}

unsigned int Bin2Dec(int *Word){ // **Consider negative num**
	int sum = 0, power = 1, neg[31];
	if(Word[31]==1){
		for(int i=0; i<32; i++)
			neg[i] = !Word[i];
		int carry = 1, temp;
		for(int i=0; i<32; i++){
			temp = (neg[i]+carry) % 2;
			carry = (neg[i]+carry) / 2;
			neg[i] = temp;
			sum += (neg[i] * power);
			power *= 2;
		}
		sum *= (-1);
	}
	else{
		for(int i=0; i<32; i++){
			sum += (Word[i] * power);
			power *= 2;
		}
	}
	return sum;
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
				CPURegister::PC.value = Word;
				idx++;
				continue;
			}
			else if(idx==-1){
				idx++;
				continue;
			}
			Address[CPURegister::PC.value+idx*4] = Word;
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
	CPURegister::reg[29].value = Word;
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
		Binary2Assembly(Address[CPURegister::PC.value]);
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