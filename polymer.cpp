#include "polymer.h"
#include<stack>
#include<tuple>
#include<fstream>



inline bool find_in_que(const deque<vec> &que, vec p) {
	if (que.empty()) return false;
	auto iter = std::find(que.begin(), que.end(), p);
	return iter != que.end()?true:false;
}



void Room::initmoves()
{
	if (dimension == 3) {
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				for (int z = -1; z <= 1; z++) {
					if (!(x == 0 && y == 0 && z == 0)) {
						moves.emplace_back(vec{ x,y,z });
					}
				}
			}
		}
	}
	for (int i = 0; i < 26; i++) {
		move_set.insert(i);
	}
}

inline vec Room::cal_direction(const vec & point1, const vec & point2)const
{
	vec temp;
	for (int i = 0; i < 3; i++) {
		temp[i] = abs(point2[i]- point1[i])<=1? point2[i] - point1[i]:(point2[i] + 1) % shape[i] - (point1[i] + 1) % shape[i];
	}
	return temp;

}

void Room::input_one_ECC(vec init, int length, int direction,int movable)
{
	try {
		Polymer p;
		p.chain.resize(length);
		p.length = length;
		int chain_num = polymer_list.size();
		for (int j = 0; j < length; j++) {
			vec point(init);
			point[direction] += j;
			p[j] = set_point(point, chain_num, j, movable);
		}
		p.construct();
		polymer_list.emplace_back(move(p));
	}
	catch (...) {
		cout << __FUNCTION__ << endl;
		throw;
	}
	
	
}

void Room::py_input_one_ECC(int x,int y,int z, int length, int direction, int movable)
{
	input_one_ECC(vec{ x,y,z }, length, direction, movable);
}

void Room::input_one_circle(vec init, int length, int direction, int movable) {

}


void Room::inputECC(int num, int length)
{
	/*try {
		vec start_point(shape);
		start_point[0] = 3*start_point[0] / 4;
		start_point[1] = shape[1] / 4;
		start_point[2] = 0;
		int sqrt_num = ceil(sqrt(num / 2.0));
		for (int i = 0; i < sqrt_num; i++) {
			for (int j = 0; j < sqrt_num; j++) {
				if (i*sqrt_num + j < num) {
					vec init{ start_point[0] + i,start_point[1] + j,start_point[2] };
					input_one_ECC(init, length, 2, 0);
				}
				else {
					return;
				}
			}
		}
		start_point[0] = shape[0] / 4;
		start_point[1] = 3*shape[1] / 4;
		start_point[2] = 0;
		for (int i = 0; i < sqrt_num; i++) {
			for (int j = 0; j < sqrt_num; j++) {
				if (i*sqrt_num + j + sqrt_num * sqrt_num < num) {
					vec init{ start_point[0] + i,start_point[1] + j,start_point[2] };
					input_one_ECC(init, length, 2, 0);
				}
				else {
					return;
				}
			}
		}
	}
	catch (string x) {
		cout << __FUNCTION__ << endl;
		cout << x;
		throw;
	}
	catch (...) {
		cout << __FUNCTION__ << endl;
		throw;
	}*/
	vec start_point = shape / 2;
	start_point[1] -= length / 2;
	
	int sqrt_num = ceil(sqrt(num));
	start_point[0] -= sqrt_num / 2;
	start_point[2] -= sqrt_num / 2;
	for (int i = 0; i < sqrt_num; i++) {
		for (int j = 0; j < sqrt_num; j++) {
			if (i*sqrt_num + j < num) {
				vec init{ start_point[0] + i,start_point[1],start_point[2] + j };
				input_one_ECC( init, length,1,0);
			}
			else {
				return;
			}
		}
	}
}

void Room::inputcircle(int num, int length)
{
	

}

void Room::input_stop_chain()
{
	try {
		for (int i = 0; i < shape[0] - 1; i++) {
			for (int j = 0; j < 2; j++) {
				vec init = { i,i + j,0 };
				input_one_ECC(init, shape[2], 2, 1);
			}
		}
		
		
	}
	catch (...) {
		cout << __FUNCTION__ << endl;
		throw;
	}
}
void Room::input_stop_chain2() {
	for (int i = 0; i < shape[0]; i++) {
		vec init = { 0,i,0 };
		input_one_ECC(init, shape[2], 2, 1);
	}

}



inline shared_ptr< Point> Room::set_point(vec location,int chain_num, int pos_in_chain,int movable)
{
	try {
		if (lattice[location]) {
			cout << location;
			throw string("error");
			
		}
		shared_ptr< Point> temp (new Point(location, chain_num, pos_in_chain, movable));
		lattice[location] = temp;
		return temp;
	}
	catch (...) {
		cout << __FUNCTION__ << endl;
		throw;
	}
}

bool Room::intersect(vec & point1, vec & point2)const
{
	vec direction = point2 - point1;

	if (direction*direction == 3) {

		vec p1{ point2[0], point1[1], point1[2] };
		vec p2{ point2[0], point2[1], point1[2] };
		vec p3{ point1[0], point2[1], point1[2] };
		vec p4{ point1[0], point2[1], point2[2] };
		vec p5{ point1[0], point1[1], point2[2] };
		vec p6{ point2[0], point1[1], point2[2] };
		return hasSide(p1, p4) || hasSide(p2, p5) || hasSide(p3, p6);
	}
	else if (direction*direction == 2) {
		if (point1[0] == point2[0]) {
			vec p1{ point1[0], point2[1], point1[2] };
			vec p2{ point2[0], point1[1], point2[2] };
			return hasSide(p1, p2);
		}
		else if (point1[1] == point2[1]) {
			vec p1{ point2[0], point1[1], point1[2] };
			vec p2{ point1[0], point2[1], point2[2] };
			return hasSide(p1, p2);
		}
		else if (point1[2] == point2[2]) {
			vec p1{ point2[0], point1[1], point1[2] };
			vec p2{ point1[0], point2[1], point2[2] };
			return hasSide(p1, p2);
		}
	}
	return false;

}

int Room::hasSide(vec & p1, vec & p2) const 
{
	try {
		shared_ptr< Point> a = lattice[p1], b = lattice[p2];
		if (a == nullptr || b == nullptr) {
			return -1;
		}
		if (a->chain_num == b->chain_num) {
			if (abs(a->pos_in_chain - b->pos_in_chain) == 1)
				return a->chain_num;
		}
	}
	catch (...) {
		cout << __FUNCTION__ << endl;
		throw;
	}
	return -1;

}

bool Room::canMove(vec & point, vec & direction)const
{
	try {
		shared_ptr< Point>p = lattice[point];
		if (p->movable == 1) {
			return false;
		}
		vec p_next = (point + direction) % shape;
		if (lattice[p_next] == nullptr) {
			if (intersect(point, p_next) == false)
				return true;
		}
		return false;
	}
	catch (...) {
		cout << __FUNCTION__ << endl;
		throw;
	}
}

void Room::stepMove(vec & position, vec & next_position, stack<vec>& path)
{
	//cout << position << next_position << endl;
	path.push(position);
	path.push(next_position);
	shared_ptr< Point>temp = lattice[position];
	lattice[position] = nullptr;
	lattice[next_position] = temp;
	temp->location = next_position;
}

void Room::localSnakeMove(int i, stack<vec> &path)
{
	int length = polymer_list[i].length;
	if (length == 0)return;
	int start_point = rand() % length;
	polymer_iter pol_iter(polymer_list[i][start_point]);
	vec p1, p2;
	//set<int> can_move(move_set);

	/*while (!can_move.empty())*/
	
		//int m_rand = rand() % can_move.size();
		int m_rand = rand() % move_set.size();

		set<int>::iterator iter = move_set.begin();
		for (int rr = 0; rr < m_rand; rr++, iter++);
		vec direction(moves[*iter]);
		//can_move.erase(iter);
		vec p_next;
		if (canMove((*pol_iter).location, direction)) {
			p_next = ((*pol_iter).location + direction) % shape;
			if (start_point > 0 && start_point < length - 1) {
				if (distance_squre(p_next, polymer_list[i][start_point + 1]->location) > dimension &&
					distance_squre(p_next, polymer_list[i][start_point - 1]->location) > dimension) 
				{
					return;
				}
			}
			p1 = (*pol_iter).location;
			p2 = (*pol_iter).location;

			stepMove((*pol_iter).location, p_next, path);
			//break;
		}
		// 模式2
		else {
			return;
		}
	
	//if (can_move.empty())return;
	for (int j = start_point - 1; j > -1; j--) {
		if (distance_squre(polymer_list[i][j]->location, polymer_list[i][j+1]->location) > dimension) {
			try {
				if (lattice[p1] == nullptr) {
					vec t1 = polymer_list[i][j]->location;
					stepMove(t1, p1, path);
					p1 = t1;
				}
				else {
					cout << "被占用了1";
				}
			}
			catch (...) {
				cout << "P1 i:" << j << endl;
			}
		}
		else {
			break;
		}
	}
	for (int j = start_point + 1; j < length; j++) {
		if (distance_squre(polymer_list[i][j]->location, polymer_list[i][j-1]->location) > dimension) {
			try {
				if (lattice[p2] == nullptr) {
					vec t2 = polymer_list[i][j]->location;
					stepMove(t2, p2, path);
					p2 = t2;
				}
				else {
					cout << "被占用了2"<<endl;
				}
			}
			catch (...) {
				cout << "p2 i:" << j;
			}
		}
		else {
			break;
		}

	}
}

void Room::movie(int m, int n, double T)
{
	results = new py::list();
	py::list Ec_list;
	py::list Ep_list;
	py::list Eb_list;
	py::list E_list;
	double Ec = cal_Ec()*Ec0;
	double Ep = cal_Ep()*Ep0;
	double Eb = cal_Eb();
	double E = Ec + Ep+ Eb;
	/*double a = cal_Ec() + cal_Ep();*/
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < polymer_list.size(); j++) {
			stack<vec> path;
			//double E0 = cal_one_Ep(j);
			this->localSnakeMove(j, path);
			//double E1 = cal_one_Ep(j);
			double dEc = cal_dEc_nearby(path)*Ec0;
			double dEp = cal_dEp_nearby(path)*Ep0;
			double dEb = cal_dEb_nearby(path);
			double dE = dEc +dEp+ dEb;
			//cout << dEp<<endl;
			if (dE >= 0) {
				//E = Ec + Ep;
				E += dE;
				Ec += dEc;
				Ep += dEp;
				Eb += dEb;
			}
			else {
				auto a = randfloat();
				//cout << a;
				if ( a< pow(2.71828, dE/T)) {
					//cout <<a;
					E += dE;
					Ec += dEc;
					Ep += dEp;
					Eb += dEb;
					//E = Ec + Ep;
				}
				else {
					//cout << "not move"<<endl;
					repair(path);
				}
			}
		}
		if (i%n == 0) {
			//cout << i << ',' << n << endl;
			/*cout << "*****Ec=" << Ec << endl;
			cout << "*****Ep=" << Ep << endl;
			cout << "*****E=" << E << endl;*/
			E_list.append(E);
			Ep_list.append(Ep);
			Ec_list.append(Ec);
			Eb_list.append(Eb);
		}
		else {
			//cout << i << ',' << n << endl;
		}
		
	}
	cout << "*****Ec=" << Ec << endl;
	cout << "*****Ep=" << Ep << endl;
	cout << "*****Eb=" << Eb << endl;
	cout << "*****E=" << E << endl;
	results->append(E_list);
	results->append(Ec_list);
	results->append(Eb_list);
	results->append(Ep_list);
	//cout << "list";


}

void Room::save()
{
	ofstream file("polymer.txt");
	for (int i = 0; i < polymer_list.size(); i++) {
		file << polymer_list[i] << endl;
	}
	file.close();
}

void Room::load()
{
	/*ifstream file("polymer.txt");
	for (int i = 0; i < polymer_list.size(); i++) {
		file >> polymer_list[i];
	}*/
}

double Room::cal_Ec()const
{
	double num = 0;
	for (int i = 0; i < polymer_list.size(); i++) {
		int length = polymer_list[i].length;
		for (int j = 2; j < length; j++) {
			num += cal_ifline(polymer_list[i][j - 2]->location,
				polymer_list[i][j - 1]->location, polymer_list[i][j]->location);
		}
	}

	return -num;

}

double Room::cal_dEp(deque<vec > &path)const
{
	
	
	double num = 0;
	vec v1, v2;
	deque<vec >::iterator iter = path.begin();
	if (iter != path.end()) v1 = (*iter); else return num;
	
	iter++;
	/*cout << "at dEp" << endl;
	cout << v1;*/
	while (iter != path.end()) {
		v2 = (*iter);
		//cout << v2;
		//cout<<"count"<<count_parallel(v1, v2, path, 1)<<endl;
		//results.emplace_back(pool.enqueue(bind(&Room::count_parallel, this, v1, v2, path, 1)));
		//num += count_parallel(v1, v2, path,1);
		if(fabs(this->b2a) <1e-1) {
			num += count_parallel(v1, v2, path, 1);
		}
		else {
			{
				num += count_parallel_B(v1, v2, path, 1);
			}
		}
		v1 = v2;
		iter++;
	} 
	/*for (auto && result:results) {
		num += result.get();
	}*/
	//cout << "end dEp" << endl;
	return num;
}

double Room::cal_dEc(deque<vec > &path)const
{

	double num = 0;
	vec v1, v2, v3;
	deque<vec >::iterator iter = path.begin();
	if (iter != path.end()) v1 = (*iter); else return num;
	iter++;
	if (iter != path.end()) v2 = (*iter); else return num;
	iter++;
	while (iter != path.end()){
		v3 = (*iter);
		num += cal_ifline(v1, v2, v3);
		//cout << v1 << v2 << v3<<num<<endl;
		v1 = v2;
		v2 = v3;
		iter++;
	}
	return -num;
}

//double Room::cal_dEb(deque<vec>& path) const
//{
//	double num = 0;
//	for (auto &point : path) {
//		num += cal_Eb_point(point);
//	}
//	return num;
//}

double Room::cal_one_Ec(int i)const
{//has some bugs
	double num = 0;
	for (int i = 0; i < polymer_list.size(); i++) {
		int length = polymer_list[i].length;
		for (int j = 2; j < length; j++) {
			num += cal_ifline(polymer_list[i].chain[j - 2]->location,
				polymer_list[i].chain[j - 1]->location, polymer_list[i].chain[j]->location);
		}
	}
	return -num;
}

double Room::cal_dEc_nearby(stack<vec> path)const
{
	if (path.empty()) return 0.0;
	deque<vec> new_path, old_path;
	
	while (!path.empty()) {
		vec v1 = path.top();
		path.pop();
		vec v2 = path.top();
		path.pop();
		new_path.emplace_back(v1);
		old_path.emplace_back(v2);
	}
	vec v1 = new_path[0];
	vec v2 = new_path.back();
	shared_ptr< Point>p1 = lattice[v1];
	shared_ptr< Point>p2 = lattice[v2];
	int chain_num = p1->chain_num;
	int length = polymer_list[chain_num].length;
	if ((p1->pos_in_chain) > (p2->pos_in_chain)) {
		for (int i = p1->pos_in_chain + 1, j = 0; i < length && j < 2; i++, j++) {
			new_path.push_front(polymer_list[chain_num][i]->location);
			old_path.push_front(polymer_list[chain_num][i]->location);
		}
		for (int i = p2->pos_in_chain-1, j = 0; i >= 0 && j < 2; i--, j++) {
			new_path.push_back(polymer_list[chain_num][i]->location);
			old_path.push_back(polymer_list[chain_num][i]->location);
		}
		

	}
	else if(p1->pos_in_chain == p2->pos_in_chain){
		for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 2; i--, j++) {
			new_path.push_front(polymer_list[chain_num][i]->location);
			old_path.push_front(polymer_list[chain_num][i]->location);
		}
		for (int i = p2->pos_in_chain + 1, j = 0;  i < length&& j < 2; i++, j++) {
			new_path.push_back(polymer_list[chain_num][i]->location);
			old_path.push_back(polymer_list[chain_num][i]->location);
		}
	}
	else {
		for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 2; i--, j++) {
			new_path.push_front(polymer_list[chain_num][i]->location);
			old_path.push_front(polymer_list[chain_num][i]->location);
		}
		for (int i = p2->pos_in_chain + 1, j = 0;  i < length && j < 2; i++, j++) {
			new_path.push_back(polymer_list[chain_num][i]->location);
			old_path.push_back(polymer_list[chain_num][i]->location);
		}

	}
	
	double dEc = cal_dEc(new_path) - cal_dEc(old_path);
	return  dEc;


}


double Room::cal_dEp_nearby(stack<vec> path)
{
	if (path.empty()) return 0.0;
	deque<vec> new_path, old_path;
	
	stack<vec> do_path(path);
	
	while (!path.empty()) {
		vec v1 = path.top();
		path.pop();
		vec v2 = path.top();
		path.pop();
		new_path.emplace_back(v1);
		old_path.emplace_back(v2);
	}
	vec v1 = new_path[0];
	vec v2 = new_path.back();
	shared_ptr< Point>p1 = lattice[v1];
	shared_ptr< Point>p2 = lattice[v2];
	int chain_num = p1->chain_num;
	int length = polymer_list[chain_num].length;
	if (p1->pos_in_chain > p2->pos_in_chain) {
		for (int i = p1->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
			new_path.push_front(polymer_list[chain_num][i]->location);
			old_path.push_front(polymer_list[chain_num][i]->location);
		}
		for (int i = p2->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
			new_path.push_back(polymer_list[chain_num][i]->location);
			old_path.push_back(polymer_list[chain_num][i]->location);
		}


	}
	else if (p1->pos_in_chain == p2->pos_in_chain) {
		for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
			new_path.push_front(polymer_list[chain_num][i]->location);
			old_path.push_front(polymer_list[chain_num][i]->location);
		}
		for (int i = p1->pos_in_chain + 1, j = 0; i < length&& j < 1; i++, j++) {
			new_path.push_back(polymer_list[chain_num][i]->location);
			old_path.push_back(polymer_list[chain_num][i]->location);
		}
	}
	else {
		for (int i = p1->pos_in_chain - 1, j = 0; i >= 0 && j < 1; i--, j++) {
			new_path.push_front(polymer_list[chain_num][i]->location);
			old_path.push_front(polymer_list[chain_num][i]->location);
		}
		for (int i = p2->pos_in_chain + 1, j = 0; i < length && j < 1; i++, j++) {
			new_path.push_back(polymer_list[chain_num][i]->location);
			old_path.push_back(polymer_list[chain_num][i]->location);
		}

	}

	double dEp1;
	try {
		dEp1 = cal_dEp(new_path);
	}
	catch (...) {
		cout << "new_path"<<endl;
		cout << __FUNCTION__<<endl;
		throw;
	}
	stack<vec> re = repair(do_path);
	double dEp2;
	try {
		dEp2 = cal_dEp(old_path);
	}
	catch (...) {
		cout << "old_path";
	}
	repair(re);
	
	return dEp1 - dEp2;


}

double Room::cal_dEb_nearby(stack<vec> path)
{
	if (path.empty()) return 0.0;
	deque<vec> new_path, old_path;

	stack<vec> do_path(path);

	while (!path.empty()) {
		vec v1 = path.top();
		path.pop();
		vec v2 = path.top();
		path.pop();
		//cout << v1 << ',' << v2 << endl;
		new_path.emplace_back(v1);
		old_path.emplace_back(v2);
	}
	//vec first_point_old = old_path.front();
	vec first_point_new = new_path.back();
	//cout << "first_point_new" << first_point_new<<endl;
	
	
	vec	last_point_old = old_path.front();
	//cout << " last_point_old" << last_point_old << endl;
	double Eb1 = cal_Eb_point(first_point_new, 2)+ cal_Eb_point(last_point_old,2);
	stack<vec> re = repair(do_path);
	double Eb2 = cal_Eb_point(last_point_old, 2)+cal_Eb_point(first_point_new, 2);
	repair(re);
	return Eb1 - Eb2;
	//vec	last_point_new = new_path.back();
	/*if (first_point_old != last_point_old) {
		double Eb1= cal_Eb_point(first_point_new, last_point_new) + cal_Eb_point(last_point_new, first_point_new)+
			cal_Eb_point(first_point_old, last_point_old) + cal_Eb_point(last_point_old, first_point_old);
		stack<vec> re = repair(do_path);
		double Eb2 = cal_Eb_point(first_point_new, last_point_new) + cal_Eb_point(last_point_new, first_point_new)+ 
			cal_Eb_point(first_point_old,last_point_old)+ cal_Eb_point(last_point_old, first_point_old);
		repair(re);
		return Eb1 - Eb2;
	}
	else {
		double Eb1 = cal_Eb_point(first_point_new) +cal_Eb_point(first_point_old);
		stack<vec> re = repair(do_path);
		double Eb2 = cal_Eb_point(first_point_new) +cal_Eb_point(first_point_old);
		repair(re);
		return Eb1 - Eb2;
	}*/

	/*if(path.empty()) return 0.0;
	deque<vec> new_path, old_path;

	stack<vec> do_path(path);

	while (!path.empty()) {
		vec v1 = path.top();
		path.pop();
		vec v2 = path.top();
		path.pop();
		new_path.emplace_back(v1);
		old_path.emplace_back(v2);
	}
	double dEb1;
	try {
		dEb1 = cal_dEb(new_path);
	}
	catch (...) {
		cout << "new_path" << endl;
		cout << __FUNCTION__ << endl;
		throw;
	}
	stack<vec> re = repair(do_path);
	double dEb2;
	try {
		dEb2 = cal_dEb(old_path);
	}
	catch (...) {
		cout << "old_path";
	}
	repair(re);
	return  dEb1- dEb2;*/
}

double Room::cal_Ep()const
{
	double num = 0;
	deque<vec> a;

	for (int i = 0; i < polymer_list.size(); i++) {
		int length = polymer_list[i].length;
		for (int j = 1; j < length; j++) {
			if (fabs(this->b2a) < 1e-14) {
				num += count_parallel(polymer_list[i][j - 1]->location, polymer_list[i][j]->location, a, 1);
			}
			else {
				num += count_parallel_B(polymer_list[i][j - 1]->location, polymer_list[i][j]->location, a, 1);
			}
		}
		/*num2 += cal_one_Ep(i);
		cout << num << endl;
		cout << num2<<endl;*/
	}
	
	return num/2.0;
}

double Room::cal_Eb() const
{
	double sum = 0;
	for (auto &p : polymer_list) {
		for (auto &point : p.chain) {
			sum += cal_Eb_point(point->location);
		}
	}
	return sum;
}

double Room::cal_one_Ep(int i)const
{
	//分子内的要除二，而分子外的不用除二；
	deque<vec> a;
	double num = 0;
		int length = polymer_list[i].length;
		for (int j = 1; j < length; j++) {
			num += count_parallel(polymer_list[i][j - 1]->location, polymer_list[i][j]->location,a,0);
		}
		return num;
}

double Room::cal_one_Eb(int) const
{
	return 0.0;
}

double Room::count_parallel_nearby(vec &point1, vec &point2,
	int i,int j, deque<vec> & que,int cal_type)const {

	double num_self = 0, num_others=0;
	int chain_num;
	if( lattice[point1] == nullptr)
		throw "NULL";
	chain_num = lattice[point1]->chain_num;
	vec p1(point1), p2(point2),p3(point1), p4(point2);
	p1[i] = (p1[i] + 1) % shape[i];
	p2[i] = (p2[i] + 1) % shape[i];
	int result;
	
	result = hasSide(p1, p2);
	if (result == -1) {;}
	else {
		if (result == chain_num) {
			if (find_in_que(que, p1) && find_in_que(que, p2)) {
				num_self += 0.5;
			}
			else {
				num_self += 1;
			}
		}
		else {
			num_others += 1;
		}
	}
	p1[i] = (p1[i] + shape[i] - 2) % shape[i];
	p2[i] = (p2[i] + shape[i] - 2) % shape[i];
	 result = hasSide(p1, p2);
	if (result == -1) { ; }
	else {
		if (result == chain_num) {
			if (find_in_que(que, p1) && find_in_que(que, p2)) {
				num_self += 0.5;
			}
			else {
				num_self += 1;
			}
		}
		else {
			num_others += 1;
		}
	}
	p3[j] = (p3[j] + 1) % shape[j];
	p4[j] = (p4[j] + 1) % shape[j];
	 result = hasSide(p3, p4);
	if (result == -1) { ; }
	else {
		if (result == chain_num) {
			if (find_in_que(que, p3) && find_in_que(que, p4)) {
				num_self += 0.5;
			}
			else {
				num_self += 1;
			}
		}
		else {
			num_others += 1;
		}
	}
	p3[j] = (p3[j] + shape[j] - 2) % shape[j];
	p4[j] = (p4[j] + shape[j] - 2) % shape[j];
	  result = hasSide(p3, p4);
	if (result == -1) { ; }
	else {
		if (result == chain_num) {
			if (find_in_que(que, p3) && find_in_que(que, p4)) {
				num_self += 0.5;
			}
			else {
				num_self += 1;
			}
		}
		else {
			num_others += 1;
		}
	}
	if (cal_type == 0) {
		return num_others + num_self / 2;
		//cout << num_others << ',' << num_self << endl;
	}
	else {
		if (num_self != 0) { ; }
		//cout << num_others << ',' << num_self << endl;
		return num_others + num_self;
	}
}
double Room::count_parallel_nearby8(vec &point1, vec &point2, 
	int i, int j, deque<vec> & que, int cal_type)const {

	double num_self = 0, num_others = 0;
	int chain_num;
	if (lattice[point1] == nullptr)
		throw "NULL";
	chain_num = lattice[point1]->chain_num;
	vec p1(point1), p2(point2);
	int k = 3 - i - j;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x == 0 && y == 0) {
				continue;
			}
			p1[i] = (point1[i] + shape[i] + x) % shape[i];
			p2[i] = (point2[i] + shape[i] + x) % shape[i];
			p1[j] = (point1[j] + shape[j] + y) % shape[j];
			p2[j] = (point2[j] + shape[j] + y) % shape[j];
			int result;
			result = hasSide(p1, p2);
			if (result == -1) { ; }
			else {
				if (result == chain_num) {
					if (find_in_que(que, p1) && find_in_que(que, p2)) {
						num_self += 0.5;
					}
					else {
						num_self += 1;
					}
				}
				else {
					num_others += 1;
				}
			}

		}

	}
	if (cal_type == 0) {
		return num_others + num_self / 2;
		//cout << num_others << ',' << num_self << endl;
	}
	else {

		if (num_self != 0) { ; }
		//cout << num_others << ',' << num_self << endl;
		return num_others + num_self;
	}
}
double Room::count_parallel_nearby24(vec &point1, vec &point2,
	int i, int j,const deque<vec> & que, int cal_type)const {

	double num_self = 0, num_others = 0;
	int chain_num;
	if (lattice[point1] == nullptr)
		throw "NULL";
	chain_num = lattice[point1]->chain_num;
	vec p1(point1), p2(point2);
	int k = 3 - i - j;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				if (x == 0 && y == 0) {
					continue;
				}
				p1[i] = (point1[i] + shape[i] + x) % shape[i];
				p2[i] = (point2[i] + shape[i] + x) % shape[i];
				p1[j] = (point1[j] + shape[j] + y) % shape[j];
				p2[j] = (point2[j] + shape[j] + y) % shape[j];
				p1[k] = (point1[k] + shape[k] + z) % shape[k];
				p2[k] = (point2[k] + shape[k] + z) % shape[k];
				int result= hasSide(p1, p2);
				if (result == -1) { continue; }
				else {
					if (result == chain_num) {
						if (find_in_que(que, p1) && find_in_que(que, p2)) {
							num_self += 0.5;
						}
						else {
							num_self += 1;
						}
					}
					else {
						num_others += 1;
					}
				}
			}
	}

	}
	if (cal_type == 0) {
		return num_others + num_self / 2.0;
		//cout << num_others << ',' << num_self << endl;
	}
	else {
		if (num_self != 0) { 
			//cout << num_others << ',' << num_self << endl; }
		}
		return num_others + num_self;
	}
}
double Room::count_parallel_nearby_all(vec &point1, vec &point2,
	const deque<vec> & que, int cal_type)const {

	double num_self = 0, num_others = 0;
	int chain_num;
	if (lattice[point1] == nullptr)
		throw "NULL";
	chain_num = lattice[point1]->chain_num;
	vec p1, p2;
	vec direction = cal_direction(point1, point2);
	//cout << direction<<endl;
	for (auto &direc : moves) {
		if ((direc == direction) || ((direc +direction) == vec{ 0,0,0 })) {
			//cout << vec{ x,y,z };
			continue;
		}
		p1 = (point1 + direc) % shape;
		p2 = (point2 + direc) % shape;
		int result = hasSide(p1, p2);
		if (result == -1) { continue; }
		else {
			if (result == chain_num) {
				if (find_in_que(que, p1) && find_in_que(que, p2)) {
					num_self += 0.5;
				}
				else {
					num_self += 1;
				}
			}
			else {
				num_others += 1;
			}
		}
	}
				
	if (cal_type == 0) {
		return num_others + num_self / 2.0;
		//cout << num_others << ',' << num_self << endl;
	}
	else {
		if (num_self != 0) {
			//cout << num_others << ',' << num_self << endl; 
		}
		return num_others + num_self;
	}
}

double Room::count_parallel(vec &point1, vec &point2, deque<vec> & que,int cal_type)const
{
	double num = 0;
	try {
			return count_parallel_nearby_all(point1, point2, que, cal_type);
		//vec direction =cal_direction( point2, point1);
		//
		//if (direction*direction == 1) {
		//	/*if(point1[0] == point2[0] && point1[2] == point2[2]) {
		//		return count_parallel_nearby8(point1, point2, 0, 2, que, cal_type);
		//	}
		//	else if (point1[1] == point2[1] && point1[2] == point2[2]) {
		//	return count_parallel_nearby8(point1, point2, 1, 2, que, type);
		//	}
		//	else if (point1[0] == point2[0] && point1[1] == point2[1]) {
		//	return count_parallel_nearby8(point1, point2, 0, 1, que, type);
		//	}*/

		//	if (point1[0] == point2[0] && point1[1] == point2[1]) {
		//		return count_parallel_nearby24(point1, point2, 0, 1, que, type);
		//	}
		//	else if (point1[0] == point2[0] && point1[2] == point2[2]) {
		//		return count_parallel_nearby24(point1, point2, 0, 2, que, type);
		//	}
		//	else if (point1[1] == point2[1] && point1[2] == point2[2]) {
		//		return count_parallel_nearby24(point1, point2, 1, 2, que, type);
		//	}
		//	/*if (point1[0] == point2[0] && point1[2] == point2[2]) {
		//	return count_parallel_nearby(point1, point2, 0, 2, que,type);
		//}
		//else if (point1[1] == point2[1] && point1[2] == point2[2]) {
		//	return count_parallel_nearby(point1, point2, 1, 2, que, type);
		//}
		//else if (point1[0] == point2[0] && point1[1] == point2[1]) {
		//	return count_parallel_nearby(point1, point2, 0, 1, que, type);
		//}*/
		//}
		//else if (direction*direction == 2) {
		//	;
		//}
		//else if (direction*direction == 3) {
		//	;
		//}
		//else {
		//	cout << point1 << point2 << endl;
		//	cout << direction * direction<<endl;
		//	throw "long bond";
		//}
		
	}
	catch (...) {
		cout << __FUNCTION__ << endl;
		throw;
	}
	return num;
}

double Room::count_parallel_B(vec &point1, vec &point2, deque<vec> & que, int cal_type)const
{
	return count_parallel_nearby_allB(point1, point2, que, cal_type);
}


double Room::cal_average_thick() const
{
	return 0.0;
}

double Room::count_parallel_nearby_allB(vec &point1, vec &point2,
	const deque<vec> & que, int cal_type)const {

	double num_self = 0, num_others = 0;
	int chain_num;
	if (lattice[point1] == nullptr)
		throw "NULL";
	chain_num = lattice[point1]->chain_num;
	vec p1, p2;
	vec direction = cal_direction(point1, point2);
	//cout << direction<<endl;
	for (auto &direc : moves) {
		if ((direc == direction) || ((direc + direction) == vec{ 0,0,0 })) {
			//cout << vec{ x,y,z };
			continue;
		}
		for (int i = 0; i < 5; i++) {
			p1 = (point1 + i*direc) % shape;
			p2 = (point2 + i*direc) % shape;
			int result = hasSide(p1, p2);
			if (result == -1) { break; }
			else {
				if (result == chain_num) {
					if (find_in_que(que, p1) && find_in_que(que, p2)) {
						num_self += 0.5*pow(this->b2a,i);
					}
					else {
						num_self += 1 * pow(this->b2a, i);
					}
				}
				else {
					num_others += 1 * pow(this->b2a, i);
				}
			}
		}
	}

	if (cal_type == 0) {
		return num_others + num_self / 2.0;
		//cout << num_others << ',' << num_self << endl;
	}
	else {
		if (num_self != 0) {
			//cout << num_others << ',' << num_self << endl; 
		}
		return num_others + num_self;
	}
}


double Room::cal_thick(vec &p1, vec &p2)const//计算厚度
{
	/*
		 if self.dimension==2:
			thickx = 0
			thicky = 0
			widthx=0
			widthy=0
			if p1[0] == p2[0]:

				p3 = p1.copy()
				p4 = p2.copy()
				for i in range(1, self.shape[0]):
					p3[0] = (p1[0] + i) % self.shape[0]
					p4[0] = (p2[0] + i) % self.shape[0]
					if self.hasSide(p3, p4):
						pass
					else:
						thickx=i-1
						break
				p5 = p1.copy()
				p6 = p2.copy()
				p3[0] = (p1[0] + 1) % self.shape[0]
				p4[0] = (p2[0] + 1) % self.shape[0]
				if self.hasSide(p3, p4):
					for i in range(1, self.shape[0]):
						p3[1] = (p1[1] + i) % self.shape[1]
						p4[1] = (p2[1] + i) % self.shape[1]
						p5[1] = (p1[1] + i) % self.shape[1]
						p6[1] = (p2[1] + i) % self.shape[1]
						if self.hasSide(p3, p4) and self.hasSide(p5, p6):
							pass
						else:
							widthx=i-1
							break
			elif p1[1] == p2[1]:

				# mylog.debug("y")
				p3 = p1.copy()
				p4 = p2.copy()
				for i in range(1, self.shape[1]):
					p3[1] = (p1[1] + i) % self.shape[1]
					p4[1] = (p2[1] + i) % self.shape[1]
					if self.hasSide(p3, p4):
						pass
					else:
						thicky =i-1
						break
				p5 = p1.copy()
				p6 = p2.copy()
				p3[1] = (p1[1] + 1) % self.shape[1]
				p4[1] = (p2[1] + 1) % self.shape[1]
				if self.hasSide(p3, p4):
					for i in range(1, self.shape[1]):
						p3[0] = (p1[0] + i) % self.shape[0]
						p4[0] = (p2[0] + i) % self.shape[0]
						p5[0] = (p1[0] + i) % self.shape[0]
						p6[0] = (p2[0] + i) % self.shape[0]
						if self.hasSide(p3, p4) and self.hasSide(p5, p6):
							pass
						else:
							widthy=i-1
							break
			return max(thickx,thicky),max(widthx,widthy)
		else :
			return  0,0
		*/
	return 0.0;
}

double Room::cal_Rg()const// 均方旋转半径
{
	double num = 0;
	for (auto &p : polymer_list) {
		//vec center = p.get_center();

	}
	
	/*for p in polymer_list :
	center = np.mean(p.chain, axis = 0)
	centers = np.asarray([center] * p.length)
	rgb = p.chain - centers
	return np.mean(rgb*rgb)*dimension*/
	return 0.0;
}

double Room::cal_h2()const// 均方末端距
{
	double num = 0;
	for (auto &p : polymer_list) {
		num += distance_squre(p.chain[0]->location, p.chain.back()->location);
	}
	return num;
}

stack<vec> Room::repair(stack<vec> &path)
{
	stack<vec> no_use;
	while (!path.empty()) {
		vec v1 = path.top();
		path.pop();
		vec v2 = path.top();
		path.pop();
		stepMove(v1, v2, no_use);
	}
	return no_use;
}

inline double Room::cal_ifline(vec &p1, vec &p2, vec &p3)const
{
	if (cal_direction(p1, p2) == cal_direction(p2, p3)) 
		return 0;
	return 1;
}

double Room::cal_Eb_point(vec & p, int type) const

{
	vec point;//some bugs
	int i, j;
	i = lattice[p] == nullptr ? 0 : 1;
	double sum = 0;
	for (auto &direc : moves) {
		point = (p + direc) % shape;
		j = lattice[point] == nullptr ? 0 : 1;
		//cout << i << ',' << j << endl;
		sum += this->Eb_matrix[i][j];
		
	}
	//cout << "---dEp=" << sum << "---" << endl;
	return sum;

	//vec point;//some bugs
	//int i, j;
	//i = lattice[p] == nullptr ? 0 : 1;
	//double sum = 0;
	//for (auto &direc : moves) {
	//	point = (p + direc) % shape;
	//	j = lattice[point] == nullptr ? 0 : 1;
	//	sum += this->Eb_matrix[i][j];
	//}
	//return sum;
}
double Room::cal_Eb_point(vec & p) const

{
	vec point;//some bugs
	int i, j;
	i = lattice[p] == nullptr ? 0 : 1;
	double sum = 0;
	for (auto &direc : moves) {
		point = (p + direc) % shape;
		j = lattice[point] == nullptr ? 0 : 1;
		if(i==1&&j==1)
			sum += this->Eb_matrix[i][j]/2;
		else {
			sum += this->Eb_matrix[i][j];
		}
	}
	return sum;
	
}

double Room::cal_PSM() const
{
	for (auto &p : polymer_list) {
		for (auto &point : p.chain) {
			cal_PSM_point(point->location);
		}
	}
	return 0.0;
}

double Room::cal_PSM_point(vec &p) const
{
	return 0.0;
}

void Grid::thread_yz(int i, int y, int z)
{
	lattice[i].resize(y);
	for (int j = 0; j < y; j++) {
		lattice[i][j].resize(z);
		for (int k = 0; k < z; k++) {
			lattice[i][j][k] = nullptr;
		}
	}
}


ostream & operator<<(ostream & o, Point & p)
{
	o << p.location;
	return o;
}

ostream & operator<<(ostream & o, Polymer & p)
{
	for (int i = 0; i < p.length; i++) {
		o << *(p.chain[i])<<',';
	}
	return o;
}

void Polymer::construct()
{//connect points in a chain
	for (int i = 0; i < length; i++) {
		if (i != 0) {
			//cout << i<<endl;
			chain[i]->pre = chain[i - 1];
		}
		if (i !=length-1) {
			chain[i]->next = chain[i + 1];
		}
	}
	
}

polymer_iter polymer_iter::operator+(int n)
{
	polymer_iter temp(*this);
	for (int i = 0; i < n; i++, temp++);
	return temp;
}

polymer_iter polymer_iter::operator-(int n)
{
	polymer_iter temp(*this);
	for (int i = 0; i < n; i++, temp--);
	return temp;
}