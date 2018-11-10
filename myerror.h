#pragma once
#include<iostream>
#include"vec.h"
#include<string>
// error types //To be done
class WrongPoint{ 
public: 
	WrongPoint(string a,vec b) {
		std::cout << a;
		std::cout << b;
	}
};