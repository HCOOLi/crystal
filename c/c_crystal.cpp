
#include"../cpp/room.h"
extern "C" {
#include"c_crystal.h"
}
//
//	py::class_<Room>("Room", py::init<int, int, int, py::list, py::list >())
//		
//		.def("input_one_ECC", &Room::py_input_one_ECC)
//		.def("input_one_FCC", &Room::py_input_one_FCC)
//		.def("input_stop_chain", &Room::input_stop_chain)
//		.def("input_stop_chain2", &Room::input_stop_chain2)
//		.def("cal_thick_by_point", &Room::cal_thick_by_point)
//		.def("construct_by_pylist", &Room::construct_by_pylist)
//		.def("cal_thickness", &Room::cal_thickness)
//		.def("movie", &Room::movie)
//		.def("delete_chain", &Room::lazy_delete_chain)
//		.def("get_list", &Room::get_list)
//		.def("cal_Ec", &Room::cal_Ec)
//		.def("cal_Ep", &Room::cal_Ep)
//		.def("ca_Eb", &Room::cal_Eb)
//		.def("get_result", &Room::get_result)
//		.def("get_polymer", &Room::get_polymer, py::return_internal_reference<>())
//		.def("get_num_of_polymers", &Room::num_of_polymers)
//		

struct C_Room :Room
{
	C_Room(int x,int y,int z,int type):Room(x,y,z,type) {}
	~C_Room(){}
};


C_Room * newRoom(int x, int y, int z, int type)
{
	auto p= new C_Room(x, y, z, type);
	return p;
}
void DeleteRoom(C_Room * room) {
	delete room;
}

void inputECC(C_Room * room, int num, int length)
{
	room->inputECC(num, length);
}

void input_one_ECC(C_Room * room, int x, int y, int z, int length, int direction, int* type, int moveable)
{
	room->input_one_ECC(vec{ x,y,z }, length, direction, vector<int>(type, type + length), moveable);
}

void movie(C_Room * room, int m, int n, double T)
{
	room->movie(m, n, T);
}

void input_one_FCC(C_Room * room, int x, int y, int z, int length, int direction, int fold_direction, int* type, int moveable)
{
	room->input_one_FCC(vec{x,y,z},length,direction,fold_direction,vector<int>(type,type+length),moveable);
}
