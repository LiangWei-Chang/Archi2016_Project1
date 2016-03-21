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
	int Word[32], bytes = 4, Cycle = 0, idx = -2, next_addr;
	bitset<8> bs;
	map< int,vector<int> > Address;
	// Initialize register;
	InitialReg();
	
	ofstream fout("../testcase/error2/snapshot.rpt", ios::out);
	ofstream Errorout("../testcase/error2/error_dump.rpt", ios::out);
	
	// Read iimage.bin
	ifstream fin("../testcase/error2/iimage.bin", ios::in | ios::binary);
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
				CPURegister::PC.value = Bin2Dec(Word);
				idx++;
				continue;
			}
			else if(idx==-1){
				idx++;
				continue;
			}
			for(int i=0; i<32; i++)	Address[CPURegister::PC.value+idx*4].push_back(Word[i]);
			idx++;
		}
	}
	fin.close();
	
	// Read dimage.bin
	fin.open("../testcase/error2/dimage.bin", ios::in | ios::binary);
	// Read $sp
	for(int i=4; i>0; i--){
		fin.get(ch);
		bs = ch;
		for(int j=0; j<8; j++)
			Word[i*8-1-j] = bs[7-j];
	}
	CPURegister::reg[29].value = Bin2Dec(Word);
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
			DataMemory::Memory[i].push_back(bs[j]);
	}
	cyclePrint(fout, Cycle);
	Terminal::Halt = false;
	Terminal::error_type = 0;
	
	//Start Instructions
	while(!Terminal::Halt){
		Binary2Assembly(Address[CPURegister::PC.value]);
		if(CPURegister::PC.value==0xFFFF){
			Terminal::Halt = true;
			return 0;
		}
		if(Terminal::error_type!=0){
			switch(Terminal::error_type){
				case 1:
					Errorout << "In cycle " << Cycle << ": Write $0 Error" << endl;
					Terminal::error_type = 0;
					break;
				case 2:
					Errorout << "In cycle " << Cycle << ": Number Overflow" << endl;
					Terminal::error_type = 0;
					break;
				case 3:
					Errorout << "In cycle " << Cycle << ": Address Overflow" << endl;
					Terminal::Halt = true;
					break;
				case 4:
					Errorout << "In cycle " << Cycle << ": Misalignment Error" << endl;
					Terminal::Halt = true;
					break;
				default: break;
			}
			if(Terminal::error_type==3 || Terminal::error_type==4) return 0;
		}
		cyclePrint(fout, Cycle);
	}
	fout.close();
	Errorout.close();
	return 0;
}