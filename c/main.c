#include"c_crystal.h"
#include<stdio.h>
int main() {
	C_Room * r=newRoom(32,32, 32,4);
	input_one_ECC(r, 2, 10);
	DeleteRoom(r);
	return 0;
}