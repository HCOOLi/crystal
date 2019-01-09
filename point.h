#pragma once
#include<iostream>
#include"vec.h"
#include<boost/python.hpp>
namespace py = boost::python;
class Point //A point class
{

public:
	int chain_num;
	int pos_in_chain;
	int movable;//0可以移动,1不可以移动
	int type;
	vec location;
	shared_ptr< Point> pre = nullptr;
	shared_ptr< Point> next = nullptr;
	

public:
	Point() {}
	Point(vec loc, int c_num, int p_i_c,int ty=1, int movable = 0) :
		location(loc), chain_num(c_num),type(ty), pos_in_chain(p_i_c), movable(movable) {}
	/*Point(Point & p);*/
	py::list get_list()const {
		py::list a;
		for (int i = 0; i < 3; i++) {
			a.append(location[i]);
		}
		return a;
	}
};
ostream & operator<<(ostream & o, Point & p);