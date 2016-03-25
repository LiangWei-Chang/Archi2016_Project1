#include <stdio.h>

int main(){
	short a = 0xFA05; // a = -1531;
	unsigned int b = 0xF032; // b = 61490;
	int i = 0, k;
	for(i = 0; i < 1000; i++){
		if(a < b)
			b = b + a;
	}
	k = i - 1;
	return 0;
}