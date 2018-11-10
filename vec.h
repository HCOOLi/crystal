#pragma once
#include<iostream>
#include<cmath>
#include<vector>
#include<array>
using namespace std;
typedef array<int, 3> vec;
// some useful functions and operators;
inline vec operator+(const vec &a, const vec &b) {
	vec v;
	for (int i = 0; i < 3; i++) {
		v[i] = a[i] + b[i];
	}
	return v;
}
inline vec operator-(const vec &a, const vec &b) {
	vec v;
	for (int i = 0; i < 3; i++) {
		v[i] = a[i] - b[i];
	}
	return v;
}
inline vec operator/(const vec & a, const  int b)
{
	return vec{ a[0] / b,a[1] / b,a[2] / b };
}
inline bool operator>=(const vec &a, const  vec &b) {
	for (int i = 0; i < 3; i++) {
		if (a[i] >= b[i]) {
			return true;
		}
	}
	return false;
}
inline bool operator<(const vec &a, const vec &b) {
	for (int i = 0; i < 3; i++) {
		if (a[i] < b[i]) {
			return true;
		}
	}
	return false;
}
inline  double operator*(const vec &a, const vec &b) {
	double sum = 0;
	for (int i = 0; i < 3; i++) {
		sum += a[i] * b[i];
	}
	return sum;
}

inline  vec operator*(int a, const vec &b) {
	vec result;
	for (int i = 0; i < 3; i++) {
		result[i]= a * b[i];
	}
	return result;
}

inline vector<double> mean(const vec &a, const vec &b) {
	return vector<double>();
}
inline double mean(const vec &a) {
	return 0;
}
inline ostream& operator<<(ostream&o, const vec &a) {
	o << '<' << a[0] << "," << a[1] << ',' << a[2] << '>';
	return o;
}
inline  vec operator%(const vec & a, const vec & b)
{
	vec v;
	for (int i = 0; i < 3; i++) {
		v[i] = (a[i] + b[i]) % b[i];
	}
	return v;
}
inline bool operator==(const vec& a, const vec& b)
{
	for (int i = 0; i < 3; i++) {
		if (a[i] != b[i])
		{
			return false;
		}
	}
	return true;
}

