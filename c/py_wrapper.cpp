// Example.cpp

//wrap.cpp

//1.C代码

#include <Python.h>
/*int add(int arg1, int arg2)
{
	return arg1 + arg2;
}
2.add的包装函数：

static PyObject* Windy_dict(PyObject *self, PyObject *args)
{
	创建列表
	PyObject *newlist = PyList_New(0);
	PyList_Append(newlist, PyUnicode_FromString("first"));
	PyList_Append(newlist, PyUnicode_FromString("second"));
	PyList_Append(newlist, PyUnicode_FromString("third"));
	返回给解释器
	return newlist;
}

static PyObject* wrap_add(PyObject *self, PyObject *args)
{
	把输入的Python对象转换为C/C++能识别的数据

	int arg1, arg2;
	if (!PyArg_ParseTuple(args, "ii", &arg1, &arg2))
		return NULL;
	调用C/C++函数，得到结果

	int result = add(arg1, arg2);
	把得到的结果包装成Python对象，并返回

	return (PyObject*)Py_BuildValue("i", result);
}
3.为模块添加PyMethodDef方法数组

static PyMethodDef wrap_methods[] = {
	{ "add", wrap_add, METH_VARARGS },
{ NULL, NULL }
};
4.增加模块初始化函数InitModule

PyMODINIT_FUNC initwrap(void)
{
	Py_InitModule("wrap ", wrap_methods);
}
*/



class Numbers
{
public:
	Numbers(int first, double second)
		: m_first(first), m_second(second) {}
	double NumMemberMult(void) { return m_first * m_second; }
private:
	int m_first;
	double m_second;
};

static void PyDelNumbers(PyObject *ptr)
{
	void*  newptr=PyCapsule_GetPointer(ptr, NULL);
	Numbers * oldnum = static_cast<Numbers *>(newptr);
	delete oldnum;
	return;
}

;
PyObject *Example_new_Numbers(PyObject *, PyObject* args)
{
	int arg1;
	double arg2;
	int ok = PyArg_ParseTuple(args, "id", &arg1, &arg2);
	if (!ok) return NULL;
	//动态创建一个新对象

	Numbers *newnum = new Numbers(arg1, arg2);
	
	//把指针newnum包装成PyCObject对象并返回给解释器

	return PyCapsule_New(newnum,NULL, PyDelNumbers);
}




PyObject * Example_Numbers_MemberMult(PyObject *self, PyObject* args)
{
	
	/*PyObject *pynum = 0;
	int ok = PyArg_ParseTuple(args, "O", &pynum);
	PyInstanceMethod_New(pynum);
	if (!ok) return NULL;*/
	//把PyCObject转换为void指针

	void * temp = PyCapsule_GetPointer(self,NULL);
	//把void指针转换为一个Numbers对象指针

	Numbers * thisnum = static_cast<Numbers *>(temp);
	//调用函数

	double result = thisnum->NumMemberMult();
	//返回结果 

	return Py_BuildValue("d", result);
}

static PyMethodDef Example_methods[] = {
	{ "Numbers", Example_new_Numbers, METH_VARARGS,"Numbers" },
	{ "NumMemberMult", Example_Numbers_MemberMult, METH_VARARGS },
	{ NULL, NULL }
};
static struct PyModuleDef ExtestModule = {
	PyModuleDef_HEAD_INIT,
	"Extest",
	NULL,
	-1,
	Example_methods
};


PyMODINIT_FUNC PyInit_c_wrapper(void)
{
	return PyModule_Create(&ExtestModule);
}
//
//#include <Python.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#define BUFSIZE 10
//
//int fac(int n) {
//	if (n < 2)
//		return 1;
//	return n * fac(n - 1);
//}
//
//static PyObject * Extest_fac(PyObject *self, PyObject *args) {
//	int res;//计算结果值
//	int num;//参数
//	PyObject* retval;//返回值
//
//					 //i表示需要传递进来的参数类型为整型，如果是，就赋值给num，如果不是，返回NULL；
//	res = PyArg_ParseTuple(args, "i", &num);
//	if (!res) {
//		//包装函数返回NULL，就会在Python调用中产生一个TypeError的异常
//		return NULL;
//	}
//	res = fac(num);
//	//需要把c中计算的结果转成python对象，i代表整数对象类型。
//	retval = (PyObject *)Py_BuildValue("i", res);
//	return retval;
//}
//
//char *reverse(char *s) {
//	register char t;
//	char *p = s;
//	char *q = (s + (strlen(s) - 1));
//	while (p < q) {
//		t = *p;
//		*p++ = *q;
//		*q-- = t;
//	}
//	return s;
//}
//
//static PyObject *
//Extest_reverse(PyObject *self, PyObject *args) {
//	char *orignal;
//	if (!(PyArg_ParseTuple(args, "s", &orignal))) {
//		return NULL;
//	}
//	return (PyObject *)Py_BuildValue("s", reverse(orignal));
//}
//
//static PyObject *
//Extest_doppel(PyObject *self, PyObject *args) {
//	char *orignal;
//	char *reversed;
//	PyObject * retval;
//	if (!(PyArg_ParseTuple(args, "s", &orignal))) {
//		return NULL;
//	}
//	retval = (PyObject *)Py_BuildValue("ss", orignal, reversed = reverse(strdup(orignal)));
//	free(reversed);
//	return retval;
//}
//
//static PyMethodDef
//ExtestMethods[] = {
//	{ "fac", Extest_fac, METH_VARARGS },
//{ "doppel", Extest_doppel, METH_VARARGS },
//{ "reverse", Extest_reverse, METH_VARARGS },
//{ NULL, NULL },
//};
//
//static struct PyModuleDef ExtestModule = {
//	PyModuleDef_HEAD_INIT,
//	"Extest",
//	NULL,
//	-1,
//	ExtestMethods
//};
//
//PyMODINIT_FUNC PyInit_Extest(void)
//{
//	return PyModule_Create(&ExtestModule);
//}