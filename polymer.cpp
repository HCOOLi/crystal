#include "polymer.h"
#include<stack>
#include<tuple>
#include<fstream>
#include"ConnectedComponent.h"


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

void Room::input_one_ECC(vec init, int length, int direction,int type,int moveable)
{
	try {
		Polymer p;
		p.chain.resize(length);
		p.length = length;
		p.type = type;
		int chain_num = polymer_list.size();
		for (int j = 0; j < length; j++) {
			vec point(init);
			point[direction] += j;
			p[j] = set_point(point, chain_num, j, type,moveable);
		}
		p.construct();
		polymer_list.emplace_back(move(p));
	}
	catch (...) {
		cout << __FUNCTION__ << endl;
		throw;
	}
	
	
}

void Room::py_input_one_ECC(int x,int y,int z, int length, int direction,int type, int moveable)
{
	input_one_ECC(vec{ x,y,z }, length, direction,type, moveable);
}

void Room::input_one_circle(vec init, int length, int direction, int moveable) {

}
void Room::construct_by_pylist(py::list chain_list) {
	
	polymer_list.clear();
	lattice = Grid(shape[0], shape[1], shape[2]);
	for (int i = 0; i < py::len(chain_list); i++) {
		py::dict chain_dict = py::extract<py::dict>(chain_list[i]);
		py::list chain = py::extract<py::list>(chain_dict["chain"]);
		int type = py::extract<int>(chain_dict["type"]);
		Polymer p;
		p.chain.resize(py::len(chain));
		p.length = py::len(chain);
		p.type = type;
		int chain_num = polymer_list.size();
		for (int j = 0; j < py::len(chain); j++) {
			py::dict point_in_list = py::extract<py::dict>(chain[j]);
			py::list position= py::extract<py::list>(point_in_list["position"]);
			//{}
			vec point;
			for (int k = 0; k < py::len(position); k++) {
				int x = py::extract<int>(position[k]);
				point[k] = x;
			}
			int type = py::extract<int>(point_in_list["type"]);
			p[j] = set_point(point, chain_num, j,type, 0);


		}
		p.construct();
		polymer_list.emplace_back(move(p));
	}

}

void Room::inputECC(int num, int length)
{
	
	vec start_point = shape / 2;
	start_point[1] -= length / 2;
	
	int sqrt_num = ceil(sqrt(num));
	start_point[0] -= sqrt_num / 2;
	start_point[2] -= sqrt_num / 2;
	for (int i = 0; i < sqrt_num; i++) {
		for (int j = 0; j < sqrt_num; j++) {
			int type = rand() % 2 + 1;
			if (i*sqrt_num + j < num) {
				vec init{ start_point[0] + i,start_point[1] + j,start_point[2]  };
				input_one_ECC( init, length,2,type,0);
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
				input_one_ECC(init, shape[2], 2, 1,1);
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
		input_one_ECC(init, shape[2], 2, 1,1);
	}

}


inline shared_ptr< Point> Room::set_point(vec location,int chain_num, int pos_in_chain, int type,int moveable)
{
	try {
		if (lattice[location]) {
			cout << location;
			throw string("error");
		}
		shared_ptr< Point> temp (new Point(location, chain_num, pos_in_chain, type, moveable));
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
		return get_side_num(p1, p4)!=-1 || get_side_num(p2, p5) != -1 || get_side_num(p3, p6) != -1;
	}
	else if (direction*direction == 2) {
		if (point1[0] == point2[0]) {
			vec p1{ point1[0], point2[1], point1[2] };
			vec p2{ point2[0], point1[1], point2[2] };
			return get_side_num(p1, p2) != -1;
		}
		else if (point1[1] == point2[1]) {
			vec p1{ point2[0], point1[1], point1[2] };
			vec p2{ point1[0], point2[1], point2[2] };
			return get_side_num(p1, p2) != -1;
		}
		else if (point1[2] == point2[2]) {
			vec p1{ point2[0], point1[1], point1[2] };
			vec p2{ point1[0], point2[1], point2[2] };
			return get_side_num(p1, p2) != -1;
		}
	}
	return false;

}

int Room::get_side_num(vec & p1, vec & p2) const 
{
	try {
		shared_ptr< Point> a = lattice[p1];
		if (a == nullptr) return -1;
		shared_ptr< Point> b = lattice[p2];
		if (b == nullptr) return -1;
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
		if (p->moveable == 1) {
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
	if (polymer_list[i][0]->moveable == 1) return;
	int start_point = rand() % length;
	shared_ptr<Point> pol_iter= polymer_list[i][start_point];
	vec p1, p2;

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
	double Ep = cal_Ep();
	double Eb = cal_Eb();
	double E = Ec + Ep+ Eb;
	/*double a = cal_Ec() + cal_Ep();*/
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < polymer_list.size(); j++) {
			stack<vec> path;
			this->localSnakeMove(j, path);
			double dEc = cal_dEc_nearby(path)*Ec0;
			double dEp = cal_dEp_nearby(path);
			double dEb = cal_dEb_nearby(path);
			double dE = dEc +dEp+ dEb;
			if (dE >= 0) {
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

			E_list.append(E);
			Ep_list.append(Ep);
			Ec_list.append(Ec);
			Eb_list.append(Eb);
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
	while (iter != path.end()) {
		v2 = (*iter);
		num += count_parallel(v1, v2, path, 1);
		v1 = v2;
		iter++;
	} 
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
		v1 = v2;
		v2 = v3;
		iter++;
	}
	return -num;
}

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
	shared_ptr< Point> p1 = lattice[v1];
	shared_ptr< Point> p2 = lattice[v2];
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

	vec first_point_new = new_path.back();
	
	
	vec	last_point_old = old_path.front();

	double Eb1 = cal_Eb_point(first_point_new, 2)+ cal_Eb_point(last_point_old,2);
	stack<vec> re = repair(do_path);
	double Eb2 = cal_Eb_point(last_point_old, 2)+cal_Eb_point(first_point_new, 2);
	repair(re);
	return Eb1 - Eb2;
	
}

double Room::cal_Ep()const
{
	double num = 0;
	deque<vec> a;

	for (int i = 0; i < polymer_list.size(); i++) {
		int length = polymer_list[i].length;
		for (int j = 1; j < length; j++) {

				num += count_parallel(polymer_list[i][j - 1]->location, polymer_list[i][j]->location, a, 1);
			
		}
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

double Room::count_parallel_nearby_all(vec &point1, vec &point2,
	const deque<vec> & que, int cal_type)const {

	double num_self = 0, num_others = 0;
	int chain_num;
	if (lattice[point1] == nullptr)
		throw "NULL";
	chain_num = lattice[point1]->chain_num;
	int type1 = lattice[point1]->type;
	vec p1, p2;
	vec direction = cal_direction(point1, point2);
	
	
	//cout << direction<<endl;
	for (auto &direc : moves) {
		if ((direc == direction) || if_opposite(direc,direction)) {
			//cout << vec{ x,y,z };
			continue;
		}
		p1 = (point1 + direc) % shape;
		p2 = (point2 + direc) % shape;
		int result = get_side_num(p1, p2);
		if (result == -1) { continue; }
		else {
			int type2 = lattice[p1]->type;
			/*cout << type1 << type2;*/

			double Ep_cross = Ep_matrix[type1][type2];
			if (result == chain_num) {
				
				if (find_in_que(que, p1) && find_in_que(que, p2)) {
					num_self += 0.5*Ep_cross;
				}
				else {
					num_self += 1*Ep_cross;
				}
			}
			else {
				num_others += 1*Ep_cross;
			}
		}
	}
				
	if (cal_type == 0) {
		return num_others + num_self / 2.0;
		//cout << num_others << ',' << num_self << endl;
	}
	else {
		return num_others + num_self;
	}
}

double Room::count_parallel(vec &point1, vec &point2, deque<vec> & que,int cal_type)const
{
	double num = 0;
	try {
			return count_parallel_nearby_all(point1, point2, que, cal_type);		
	}
	catch (...) {
		cout << __FUNCTION__ << endl;
		throw;
	}
	return num;
}

//double Room::count_parallel_B(vec &point1, vec &point2, deque<vec> & que, int cal_type)const
//{
//	return count_parallel_nearby_allB(point1, point2, que, cal_type);
//}


double Room::cal_average_thick() const
{
	return 0.0;
}

//double Room::count_parallel_nearby_allB(vec &point1, vec &point2,
//	const deque<vec> & que, int cal_type)const {
//
//	double num_self = 0, num_others = 0;
//	int chain_num;
//	if (lattice[point1] == nullptr)
//		throw "NULL";
//	chain_num = lattice[point1]->chain_num;
//	vec p1, p2;
//	vec direction = cal_direction(point1, point2);
//	//cout << direction<<endl;
//	for (auto &direc : moves) {
//		if ((direc == direction) || ((direc + direction) == vec{ 0,0,0 })) {
//			//cout << vec{ x,y,z };
//			continue;
//		}
//		for (int i = 0; i < 5; i++) {
//			p1 = (point1 + i*direc) % shape;
//			p2 = (point2 + i*direc) % shape;
//			int result = get_side_num(p1, p2);
//			if (result == -1) { break; }
//			else {
//				if (result == chain_num) {
//					if (find_in_que(que, p1) && find_in_que(que, p2)) {
//						num_self += 0.5*pow(this->b2a,i);
//					}
//					else {
//						num_self += 1 * pow(this->b2a, i);
//					}
//				}
//				else {
//					num_others += 1 * pow(this->b2a, i);
//				}
//			}
//		}
//	}
//
//	if (cal_type == 0) {
//		return num_others + num_self / 2.0;
//		//cout << num_others << ',' << num_self << endl;
//	}
//	else {
//		if (num_self != 0) {
//			//cout << num_others << ',' << num_self << endl; 
//		}
//		return num_others + num_self;
//	}
//}


py::list Room::cal_thick_by_point()const//计算厚度
{
	py::list thicka, thickb, thickc;
	py::list *thickness = new py::list;
	//cout << "calthickness" << endl;
	vec point1{ 0,0,0 }, point2{ 0,0,1 };
	for (int k = 0; k < shape[2] - 1; k++) {
		for (int i = 0; i < shape[0]; i++) {
			int th_a = 0;
			for (int j = 0; j < shape[1]; j++) {

				point1[0] = i; point2[0] = i;
				point1[1] = j; point2[1] = j;
				point1[2] = k; point2[2] = k + 1;
				if(this->get_side_num(point1, point2) != -1)
					th_a++;
				else {
					if (th_a != 0) {
						thicka.append(th_a);
						th_a = 0;
					}

				}

			}
			if (th_a != 0) {
				if (th_a == 1) {
					th_a = 0;
					continue;
				}
				thicka.append(th_a);
				//cout << th_a;
				th_a = 0;
			}
		}
	}
	for (int k = 0; k < shape[2] - 1; k++) {
		for (int j = 0; j < shape[1]; j++) {
			int th_b = 0;
			for (int i = 0; i < shape[0]; i++) {
				point1[0] = i; point2[0] = i;
				point1[1] = j; point2[1] = j;
				point1[2] = k; point2[2] = k + 1;
				if (this->get_side_num(point1, point2) != -1)
					th_b++;
				else {
					if (th_b == 1) {
						th_b = 0;
						continue;
					}
					if (th_b != 0) {
						thickb.append(th_b);
						//cout << th_b;
						th_b = 0;
					}
				}
			}
			if (th_b != 0) {
				thickb.append(th_b);
				th_b = 0;
			}
		}
	}
	for (int j = 0; j < shape[1]; j++) {
		for (int i = 0; i < shape[0]; i++) {
			int th_c = 0;
			for (int k = 0; k < shape[2] - 1; k++) {
			point1[0] = i; point2[0] = i;
			point1[1] = j; point2[1] = j;
			point1[2] = k; point2[2] = k + 1;
			if (this->get_side_num(point1, point2) != -1)
				th_c++;
			else {
				if (th_c == 1) {
					th_c = 0;
					continue;
				}
				if (th_c != 0) {
					//cout << th_c;
					thickc.append(th_c);
					//cout << th_c;
					th_c = 0;
				}
			}
		}
		if (th_c != 0) {
			thickc.append(th_c);
			th_c = 0;
		}
	}
}

	thickness->append(thicka);
	thickness->append(thickb);
	thickness->append(thickc);


	return *thickness;
}

py::list Room::cal_thickness()const//计算厚度
{

	matrix::Matrix3 temp_lattice(shape[0],shape[1],shape[2]-1);
	

	vec point1, point2;
	
	for (int i = 0; i < shape[0]; i++) {
		for (int j = 0; j < shape[1]; j++) {
			for (int k = 0; k < shape[2] - 1; k++) {
			//for (int k = shape[2]/4+15; k < shape[2] - 1; k++) {
				point1[0] = i; point2[0] = i;
				point1[1] = j; point2[1] = j;
				point1[2] = k; point2[2] = k + 1;
				auto p1 = lattice[point1], p2 = lattice[point2];
				if (p1 == nullptr || p2 == nullptr) {
					continue;
				}
				else if (p1->chain_num== p2->chain_num) {
					if (p1->pos_in_chain - p2->pos_in_chain == -1) {
						temp_lattice[i][j][k] = 1;

					}
					else if (p1->pos_in_chain - p2->pos_in_chain == 1) {
						//cout << i << j << k<<endl;
						temp_lattice[i][j][k] = -1;
					}

				}
			}
		}
	}

	matrix::ConnectedComponentLabeling(temp_lattice);
	return matrix::ConnectedComponentLabeling(temp_lattice);
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
	i = lattice[p] == nullptr ? 0 : lattice[p]->type;
	double sum = 0;
	for (auto &direc : moves) {
		point = (p + direc) % shape;
		j = lattice[point] == nullptr ? 0 : lattice[point]->type;
		//cout << i << ',' << j << endl;
		sum += this->Eb_matrix[i][j];
		
	}
	//cout << "---dEp=" << sum << "---" << endl;
	return sum;

}
double Room::cal_Eb_point(vec & p) const

{
	vec point;//some bugs
	int i, j;
	i = lattice[p] == nullptr ? 0 : lattice[p]->type;
	double sum = 0;
	for (auto &direc : moves) {
		point = (p + direc) % shape;
		j = lattice[point] == nullptr ? 0 : lattice[point]->type;
		if((i*j)!=0)
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


