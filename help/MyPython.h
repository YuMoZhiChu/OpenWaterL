#pragma once
#include "EngineConfig.h"
#include "Printf.h"
#include <Python.h>

// 定义一个空的 Python 接口
#define PY_NOT_IMPL(s) {s, [](PyObject*, PyObject*) -> PyObject* { Py_RETURN_NONE; }, METH_VARARGS, ""}


namespace Python
{
	int Init();
	void End();

	int RunFunction(const char *modname, const char *funcname,
		const char *resfmt, void *cresult, const char *argfmt, ...);
	int GetGlobal(const char *modname, const char *varname,
		const char *resfmt, void *cresult);
	int SetGlobal(const char *modname, const char *varname,
		const char *valfmt, ...);

	int RunMethod(PyObject *pobject, const char *method,
		const char *resfmt, void *cresult, const char *argfmt, ...);
	int RunMethod(PyObject *pmeth,
		const char *resfmt, void *cresult, const char *argfmt, ...);
	int GetMember(const char* modname, const char *clsname, const char *attrname,
		const char *resfmt, void* cresult);
	int GetMember(PyObject *pobject, const char *attrname,
		const char *resfmt, void* cresult);
	int SetMember(PyObject *pobject, const char *attrname,
		const char *argfmt, ...);

	inline void DebugPrintStack()
	{
		Print("############# Debug Print Stack!");
		PyRun_SimpleString("import traceback\ntraceback.print_stack()\n");
	}

	void PythonError();

	// 以下是做一层封装, 目的是为了和以后的版本做兼容 原引擎是为了和 Python 2.x 做兼容

	// 字符串相关 PyObject <-> CObject
	inline const char* PyUnicode_AsUTF8_Wrapper(PyObject* object)
	{
		return PyUnicode_AsUTF8(object);
	}

	inline PyObject* PyUnicode_FromString_Wrapper(const char* s)
	{
		return PyUnicode_FromString(s);
	}

	// module 相关 注意 PyImport_AppendInittab 来添加函数
	inline PyObject* PyModule_Create_Wrapper(const char* module_name)
	{
		return PyImport_ImportModule(module_name);
	}

	inline int PyUnicode_Check_Wrapper(PyObject* object)
	{
		// Return true if the object o is a Unicode object or an instance of a Unicode subtype.
		return PyUnicode_Check(object);
	}

	// Int相关 PyObject <-> Int
	inline int PyLong_Check_Wrapper(PyObject* object)
	{
		// Return true if its argument is a PyLongObject or a subtype of PyLongObject.
		return PyLong_Check(object);
	}

	inline int PyLong_AsLong_Wrapper(PyObject* object)
	{
		return PyLong_AsLong(object);
	}
}

//// 好像就这个测试样例跑得动，上面的python相关的系统应该是没弄好, 2018-7-18 今天先搞 fbx
//// 下面是个人测试
//// https://www.tutorialspoint.com/python/python_further_extensions.htm
static PyObject *module_func(PyObject* self, PyObject* args)
{
	Print("C_Hello module, method func, is called!");
	PyRun_SimpleString("import traceback;traceback.print_stack()");
	Py_RETURN_NONE;
}

static PyMethodDef module_methods[] = {
	{ "func", (PyCFunction)module_func, METH_NOARGS, NULL },
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC
PyInit_Hello(void)
{
	static struct PyModuleDef module_def = {
		PyModuleDef_HEAD_INIT,
		"C_Hello", // name of module
		NULL,     /* module documentation, may be NULL */
		-1,       /* size of per-interpreter state of the module,
				  or -1 if the module keeps state in global variables. */
		module_methods
	};
	return PyModule_Create(&module_def);
}

void InitPyHelloTest()
{
	// Py_SetPath(L""); lib 和 dll 所在, 可以通过配置环境变量来实现
	// Py_SetPythonHome(L""); python.exe 所在
	Py_SetPath(L"D:\\Python-3.6.5\\Lib"); // 要这一堆东西 2018-9-8, 这是一份从官网下载的python源码的lib文件夹
	// Py_SetPythonHome(L"D:\\Python-3.6.5\\PCbuild\\amd64");

	// 套路: 先注册 再 initialize 再 import

	PyImport_AppendInittab("C_Hello", PyInit_Hello);

	Py_Initialize();

	PyObject* module = PyImport_ImportModule("C_Hello");
	if (module == nullptr)
	{
		Print("not C_Hello init");
		return;
	}

	PyModule_AddIntConstant(module, "C_Hello", 123);
}

