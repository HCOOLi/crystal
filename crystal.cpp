#include<string>
#include <boost/python.hpp>
#include"../data_struct/polymer.h"

namespace py=boost::python;

BOOST_PYTHON_MODULE(crystal)
{
	py::class_<Room>("Room", py::init<int, int, int, py::optional<double,double,double,double,double,double> >())
		.def("inputECC", &Room::inputECC)
		.def("input_one_ECC", &Room::py_input_one_ECC)
		.def("input_stop_chain",&Room::input_stop_chain)
		.def("input_stop_chain2", &Room::input_stop_chain2)
		.def("cal_thick_by_point",&Room::cal_thick_by_point)
		.def("construct_by_pylist",&Room::construct_by_pylist)
		.def("movie", &Room::movie)
		.def("delete_chain",&Room::deletechain)
		.def("get_list",&Room::get_list)
		.def("cal_Ec",&Room::cal_Ec)
		.def("cal_Ep",&Room::cal_Ep)
		.def("get_result",&Room::get_result)
		;
}