//#include "PySys.h"
//#include "MyPython.h"
//
//namespace Python
//{
//	static PyObject *SayHello(PyObject* self, PyObject* args)
//	{
//		Print("C_sys Call!");
//		Print("hello");
//		Py_RETURN_NONE;
//	}
//
//	static PyMethodDef s_SysMethods[] = {
//		{ "func", (PyCFunction)SayHello, METH_NOARGS, NULL },
//
//		{ NULL, NULL, 0, NULL } // 结尾判断
//	};
//
//	PyMODINIT_FUNC
//	PyInit_Sys(void)
//	{
//		static struct PyModuleDef module_def = {
//			PyModuleDef_HEAD_INIT,
//			"C_Hello", // name of module
//			NULL,     /* module documentation, may be NULL */
//			-1,       /* size of per-interpreter state of the module,
//					  or -1 if the module keeps state in global variables. */
//			s_SysMethods
//		};
//		return PyModule_Create(&module_def);
//	}
//
//	void SysMoudleRegister()
//	{
//		PyImport_AppendInittab("C_sys", PyInit_Sys);
//	}
//
//	void SysInit()
//	{
//		PyObject* module = PyImport_ImportModule("C_sys");
//
//		if (module == nullptr)
//		{
//			Print("can not initialize C_sys");
//			return;
//		}
//
//		// 加入常量的做法
//		PyModule_AddIntConstant(module, "HelloNumber", 5436);
//	}
//}
