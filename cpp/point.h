#pragma once
#include<iostream>
#include"vec.h"

class Point //A point class
{

public:
	int chain_num;
	int pos_in_chain;
	int moveable;//0可以移动,1不可以移动
	int type;
	vec location;
	

public:
	Point() {}
	Point(vec loc, int c_num, int p_i_c,int ty=1, int moveable = 0) :
		location(loc), chain_num(c_num),type(ty), pos_in_chain(p_i_c), moveable(moveable) {}

};
ostream & operator<<(ostream & o, Point & p);