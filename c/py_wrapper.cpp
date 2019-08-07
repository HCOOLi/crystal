// Example.cpp

//wrap.cpp

//1.C����

#include <Python.h>
/*int add(int arg1, int arg2)
{
	return arg1 + arg2;
}
2.add�İ�װ������

static PyObject* Windy_dict(PyObject *self, PyObject *args)
{
	�����б�
	PyObject *newlist = PyList_New(0);
	PyList_Append(newlist, PyUnicode_FromString("first"));
	PyList_Append(newlist, PyUnicode_FromString("second"));
	PyList_Append(newlist, PyUnicode_FromString("third"));
	���ظ�������
	return newlist;
}

static PyObject* wrap_add(PyObject *self, PyObject *args)
{
	�������Python����ת��ΪC/C++��ʶ�������

	int arg1, arg2;
	if (!PyArg_ParseTuple(args, "ii", &arg1, &arg2))
		return NULL;
	����C/C++�������õ����

	int result = add(arg1, arg2);
	�ѵõ��Ľ����װ��Python���󣬲�����

	return (PyObject*)Py_BuildValue("i", result);
}
3.Ϊģ�����PyMethodDef��������

static PyMethodDef wrap_methods[] = {
	{ "add", wrap_add, METH_VARARGS },
{ NULL, NULL }
};
4.����ģ���ʼ������InitModule

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
	//��̬����һ���¶���

	Numbers *newnum = new Numbers(arg1, arg2);
	
	//��ָ��newnum��װ��PyCObject���󲢷��ظ�������

	return PyCapsule_New(newnum,NULL, PyDelNumbers);
}




PyObject * Example_Numbers_MemberMult(PyObject *self, PyObject* args)
{
	
	/*PyObject *pynum = 0;
	int ok = PyArg_ParseTuple(args, "O", &pynum);
	PyInstanceMethod_New(pynum);
	if (!ok) return NULL;*/
	//��PyCObjectת��Ϊvoidָ��

	void * temp = PyCapsule_GetPointer(self,NULL);
	//��voidָ��ת��Ϊһ��Numbers����ָ��

	Numbers * thisnum = static_cast<Numbers *>(temp);
	//���ú���

	double result = thisnum->NumMemberMult();
	//���ؽ�� 

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
//	int res;//������ֵ
//	int num;//����
//	PyObject* retval;//����ֵ
//
//					 //i��ʾ��Ҫ���ݽ����Ĳ�������Ϊ���ͣ�����ǣ��͸�ֵ��num��������ǣ�����NULL��
//	res = PyArg_ParseTuple(args, "i", &num);
//	if (!res) {
//		//��װ��������NULL���ͻ���Python�����в���һ��TypeError���쳣
//		return NULL;
//	}
//	res = fac(num);
//	//��Ҫ��c�м���Ľ��ת��python����i���������������͡�
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