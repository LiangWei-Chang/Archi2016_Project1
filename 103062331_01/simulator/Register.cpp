/***************************************************

	File: Register.cpp

	Author: PinYo

***************************************************/

#include "Register.h"
#include <bitset>

string Register::Hex(){
	string Bin2Hex = "";
	bitset<32> bs;
	int power, sum;
	bs = value;
	//Binary2Hex
	for(int i=0; i<8; i++){
		power = 1;
		sum = 0;
		for(int j=i*4; j<(i+1)*4; j++){
			sum += bs[j] * power;
			power *= 2;
		}
		if(sum >= 10){
			switch(sum){
				case 10: 
					Bin2Hex = 'A' + Bin2Hex;
					break;
				case 11: 
					Bin2Hex = 'B' + Bin2Hex;
					break;
				case 12: 
					Bin2Hex = 'C' + Bin2Hex;
					break;
				case 13: 
					Bin2Hex = 'D' + Bin2Hex;
					break;
				case 14: 
					Bin2Hex = 'E' + Bin2Hex;
					break;
				case 15: 
					Bin2Hex = 'F' + Bin2Hex;
					break;
			}
		}
		else Bin2Hex = (char)(sum+'0') + Bin2Hex;
	}
	Bin2Hex = "0x" + Bin2Hex;
	return Bin2Hex;
}