#pragma once
#include<iostream>
#include"vec.h"
#include<boost/python.hpp>
class Point //A point class
{

public:
	int chain_num;
	int pos_in_chain;
	int movable;
	shared_ptr< Point>pre = nullptr;
	shared_ptr< Point>next = nullptr;
	vec location;

public:
	Point() {}
	Point(vec loc, int c_num, int p_i_c, int t = 0) :
		location(loc), chain_num(c_num), pos_in_chain(p_i_c), movable(t) {}

	py::list get_list()const {
		py::list a;
		for (int i = 0; i < 3; i++) {
			a.append(location[i]);
		}
		return a;
	}
};
ostream & operator<<(ostream & o, Point & p);