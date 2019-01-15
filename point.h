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
	int moveable;//0可以移动,1不可以移动
	int type;
	vec location;
	shared_ptr< Point> pre = nullptr;
	shared_ptr< Point> next = nullptr;
	

public:
	Point() {}
	Point(vec loc, int c_num, int p_i_c,int ty=1, int moveable = 0) :
		location(loc), chain_num(c_num),type(ty), pos_in_chain(p_i_c), moveable(moveable) {}
	/*Point(Point & p);*/
	py::dict get_list()const {
		py::dict dic;
		
		py::list py_position;
		for (int i = 0; i < 3; i++) {
			py_position.append(location[i]);
		}
		dic["position"] = py_position;
		dic["type"] = type;
		dic["moveable"] = moveable;
		return dic;
	}
};
ostream & operator<<(ostream & o, Point & p);