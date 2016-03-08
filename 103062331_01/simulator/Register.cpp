/***************************************************

	File: Register.cpp

	Author: PinYo

***************************************************/

#include "Register.h"
#include <bitset>

string Register::Hex(){
	string Bin2Hex = "0x";
	bitset<32> bs;
	int power, sum;
	bs = value;
	//Binary2Hex
	for(int i=0; i<8; i++){
		power = 8;
		sum = 0;
		for(int j=i*4; j<(i+1)*4; j++){
			sum += bs[j] * power;
			power /= 2;
		}
		if(sum >= 10){
			switch(sum){
				case 10: 
					Bin2Hex += 'A';
					break;
				case 11: 
					Bin2Hex += 'B';
					break;
				case 12: 
					Bin2Hex += 'C';
					break;
				case 13: 
					Bin2Hex += 'D';
					break;
				case 14: 
					Bin2Hex += 'E';
					break;
				case 15: 
					Bin2Hex += 'F';
					break;
			}
		}
		else Bin2Hex += (char)(sum+'0');
	}
	return Bin2Hex;
}