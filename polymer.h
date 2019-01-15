#pragma once
#include<iostream>
#include"vec.h"
#include"polymer.h"
#include"point.h"
#include<set>
#include<stack>
#include"myerror.h"
#include<boost/python.hpp>


namespace py = boost::python;

inline double randfloat() {//generate a random float
	return rand() / (double)(RAND_MAX);
}


class Polymer {//a polymer 
public:
	vector<shared_ptr< Point>> chain;
	int length;
	int type=1;

	Polymer() {};
	Polymer(const Polymer & p) {
		chain = p.chain;
		length = p.length;
		type = p.type;
	};
	Polymer & operator=(const Polymer & p) {
		chain = p.chain;
		length = p.length;
		type = p.type;
		return *this;
	}
	Polymer & operator=(Polymer && p) {
		chain = move(p.chain);
		length = p.length;
		type = p.type;
		p.chain.clear();
		p.length = 0;
		return *this;
	}

	Polymer(Polymer && p) {
		chain = move(p.chain);
		length = p.length;

		p.chain.clear();
		type = p.type;
		p.length = 0;
	};

	Polymer(int l) : length(l) {}
	int get_type() {
		return type;
	}


	shared_ptr< Point>&operator[](int i) { return chain[i]; }
	shared_ptr< Point> operator[](int i) const { return chain[i]; }
	void construct();
	py::dict get_list()const {
		py::dict json;
		py::list chain_list;
		for (int i = 0; i < length; i++) {
			chain_list.append(chain[i]->get_list());
		}
		json["chain"] = chain_list;
		json["type"] = this->type;
		return json;
	}
	Point &  get_polymer(int i) {
		return *chain[i];
	}
};
ostream & operator<<(ostream & o, Polymer& p);

class Grid {
public:
	vec shape;
	vector<vector< vector<shared_ptr< Point> > > > lattice;
	//array<array<array< > > > 

	Grid() {};

	shared_ptr< Point>  & operator[](const vec &P) {
		//const static vec b{ 0,0,0 };
		//if (P < b || P >= shape) {
		//	throw WrongPoint(__FUNCTION__, P);
		//}
		return lattice[P[0]][P[1]][P[2]];
	}
	shared_ptr< Point>   operator[](const vec &P)const {
		//const static vec b{ 0,0,0 };
		//if (P<b || P>shape) {
		//	throw WrongPoint(__FUNCTION__, P);
		//}
		return lattice[P[0]][P[1]][P[2]];
	}
	void thread_yz(int i, int y, int z);
	Grid(int x, int y, int z) :shape(vec{ x,y,z })
	{
		lattice.resize(x);
		for (int i = 0; i < x; i++) {
			thread_yz(i, y, z);
		}
	}


};

class Room {
public:

	const vec shape;
	const int dimension = 3;
	Grid lattice;
	vector< vec > moves;
	set<int> move_set;
	//parameters
	const double Ec0=1.0;
	vector<vector<double> > Eb_matrix;
	vector<vector<double> > Ep_matrix;
	//const double b2a;
	//const double b2b;
	//const double b2c;

	vector<Polymer> polymer_list;

	py::list *results;
	Polymer &  get_polymer(int i) {
		return polymer_list[i];
	}
	Room(int x, int y, int z, py::list Ep, py::list Eb ) : lattice(x, y, z), shape(vec{ x,y,z }) {
		cout << "constructing"<<endl;
		Ep_matrix.resize(py::len(Ep));
		//cout << py::len(Ep);
		for (int i = 0; i < py::len(Ep); i++) {
			Ep[i];
			cout << 'a';
			py::list Ep_array = py::extract<py::list>(Ep[i]);
			cout << py::len(Ep_array);
			Ep_matrix[i].resize(py::len(Ep_array));
			for (int j = 0; j < py::len(Ep_array); j++) {
				Ep_matrix[i][j] = py::extract<double>(Ep_array[j]);
			}
		}
		cout << "Ep_matrix"<<endl;
		Eb_matrix.resize(py::len(Eb));
		for (int i = 0; i < py::len(Eb); i++) {

			py::list Eb_array = py::extract<py::list>(Eb[i]);
			Eb_matrix[i].resize(py::len(Eb_array));
			for (int j = 0; j < py::len(Eb_array); j++) {
				Eb_matrix[i][j] = py::extract<double>(Eb_array[j]);
			}
		}
		//TODO
		cout << "Eb_matrix"<<endl;
		results = new py::list();
		initmoves();
		srand(1);
	}

	//initiate
	//Room(int x, int y, int z, double Ep = 1, double Eb=0) : lattice(x, y, z),shape(vec{ x,y,z }) {
	//	Eb_matrix.resize(2);
	//	Eb_matrix[0].resize(2);
	//	Eb_matrix[1].resize(2);
	//	Eb_matrix[0][0] = 0;
	//	Eb_matrix[0][1] = -Eb;
	//	Eb_matrix[1][0] = -Eb;
	//	Eb_matrix[1][1] = Eb;
	//	//TODO
	//	Ep_matrix.resize(3);
	//	Ep_matrix[0].resize(3);
	//	Ep_matrix[1].resize(3);
	//	Ep_matrix[2].resize(3);
	//	Ep_matrix[0][0] = 0;
	//	Ep_matrix[0][1] = 0;
	//	Ep_matrix[2][0] = 0;
	//	Ep_matrix[0][2] = 0;
	//	Ep_matrix[1][0] = 0;
	//	Ep_matrix[1][1] = 0.5;
	//	Ep_matrix[1][2] = 2;
	//	Ep_matrix[2][1] = 2;
	//	Ep_matrix[2][2] = 0.5;

	//	results = new py::list();
	//	initmoves();
	//	srand(1);
	//}
	void initmoves();

	shared_ptr<Point> set_point(vec location, int chain_num, int pos_in_chain, int type, int moveable);

	//some useful functions
	bool intersect(vec &point1, vec &point2)const;
	int get_side_num(vec & p1, vec & p2) const;
	vec cal_direction(const vec & point1, const vec & point2) const;

	void input_one_ECC(vec init, int length, int direction, int type, int moveable);

	void py_input_one_ECC(int x, int y, int z, int length, int direction, int type, int moveable);



	double distance_squre(vec &p1, vec &p2)const {
		vec direction = cal_direction(p2, p1);
		return direction * direction;
	}
	bool canMove(vec &point, vec &direction)const;
	//input chains
	void inputECC(int num, int length);
	void inputcircle(int num, int length);
	void input_stop_chain();
	void input_stop_chain2();
	void input_one_circle(vec init, int length, int direction, int moveable);
	void construct_by_pylist(py::list chain_list);

	//move
	void stepMove(vec &position, vec &next_position, stack<vec> & path);
	void localSnakeMove(int i, stack<vec > &path);
	void movie(int m, int n, double T);
	stack<vec> repair(stack<vec > &path);
	//calculate something
	void deletechain(int i) {
		for (auto &p : polymer_list[i].chain) {
			if (lattice[p->location] != p) {
				throw "somethin wrong";
			}
			else {
				lattice[p->location] = nullptr;
				p = nullptr;
			}
		}
		polymer_list[i].length = 0;
		polymer_list[i].chain.clear();

		//cout << polymer_list.size();
	}
	double cal_Ec()const;
	double cal_Ep()const;
	double cal_Eb()const;

	double cal_one_Ec(int)const;
	double cal_one_Ep(int)const;
	double cal_one_Eb(int)const;

	double cal_dEp(deque<vec> &path)const;
	double cal_dEc(deque<vec> &path)const;
	//double cal_dEb(deque<vec> &path)const;

	double cal_dEc_nearby(stack<vec> path)const;
	double cal_dEp_nearby(stack<vec> path);
	double cal_dEb_nearby(stack<vec> path);

	double cal_ifline(vec &p1, vec &p2, vec &p3)const;


	//double cal_Eb_point(vec & p, vec & p2) const;

	double cal_Eb_point(vec & p, int type) const;

	double cal_Eb_point(vec & p) const;

	double count_parallel_nearby(vec & point1, vec & point2, int i, int j, deque<vec>& que, int cal_type)const;
	double count_parallel_nearby24(vec & point1, vec & point2, int i, int j, const  deque<vec>& que, int cal_type)const;
	double count_parallel_nearby_all(vec & point1, vec & point2, const deque<vec>& que, int cal_type) const;
	double count_parallel_nearby8(vec & point1, vec & point2, int i, int j, deque<vec>& que, int cal_type)const;
	double count_parallel(vec  &point1, vec&  point2, deque<vec>& que, int cal_type)const;
	double count_parallel_B(vec & point1, vec & point2, deque<vec>& que, int cal_type) const;


	double cal_average_thick()const;

	double count_parallel_nearby_allB(vec & point1, vec & point2, const deque<vec>& que, int cal_type) const;

	py::list cal_thick_by_point()const;
	py::list cal_thickness()const;


	double cal_Rg()const;
	double cal_h2()const;
	double cal_PSM()const;
	double cal_PSM_point(vec &) const;

	//load&save
	void save();
	void load();
	//python 
	py::list get_list() const {
		py::list a;
		for (int i = 0; i < polymer_list.size(); i++)
			a.append(polymer_list[i].get_list());
		return a;
	}
	py::object get_result() const {

		return py::object(*results);
	}
	int num_of_polymers()const {
		return polymer_list.size();
	}
	~Room() {

		/*for (int i = 0; i < length; i++) {
			if (chain[i]) {
				delete chain[i];
			}
		}*/
	}
};