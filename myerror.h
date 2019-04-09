#pragma once
#include<iostream>
#include"vec.h"
#include<string>
// error types //To be done
class WrongPoint:exception{ 
public: 
	WrongPoint(string a,vec b) {
		std::cerr << this->what();
		std::cerr << a;
		std::cerr << b;
	}
};

