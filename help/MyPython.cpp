#include "stdafx.h"
#include <sstream>

#include "MyPython.h"

// include 各个模块
#include "PySys.h"

static bool s_PythonInit = false;

// 无用的函数
// PyMODINIT_FUNC init_struct(void);
// PyMODINIT_FUNC initcStringIO(void);

namespace Python
{
	// PyObject* DNetInit(); // 网络相关？

	// void RegisterIMESink(); // windows tsf ime 相关  是和输入法有关系的一套流程 现在先不管 18_5_26

	int LoadInit()
	{
		PyObject* pInit = NULL;
		if (!pInit)
		{
			PythonError();
			return -1;
		}
		Py_DECREF(pInit);
		return 0;
	}

	std::string GetAbsolutePath()
	{
		std::string absolute_path;
		// absolute_path = CFile::GetDataPath(); // 涉及到CFile
		return absolute_path;
	}

	void ModuleRegister()
	{
		// py3 套路 1.注册 2.Py_Initialize 3.import

		// XXXXX ModuelRegister

		// 比如说我们要建立一个叫 C_sys 的模块
		// 这里是第一步
		SysModuleRegister();

		// 这一步很蹊跷，而且涉及到了更深层的2个类 PyScriptObject 和 PyBaseObject 这两个类是做什么的呢，不知道呀
		// SPyScriptObject::ObjectModuleRegister();
	}

	void ModuleInit()
	{
		// 这里是第三步骤 import 
		SysInit();

		// SPyScriptObject::RegisterAll();
	}

	bool CheckFlsExist(const std::string& fls_path)
	{
		// 关于使用 fls 打包方面，先不管 18_5_26
		return false;
	}

	// 初始化函数 对外就是 Python::Init 表示初始化了整个 Python 系统
	int Init()
	{
		if (s_PythonInit)
			return 0;

		s_PythonInit = true;

		// AndroidStdeerToLogcat(); 安卓相关 把标准错误输出到 logcat  18-5-26 先不管
		
		// 模块注册
		ModuleRegister();

		// python set path 相关 , 现在先不管

		// 排除系统的path 对 编辑器/游戏的影响
		// ... ...
		// 之后再看如何准确的定位我们的 pythonhome 和 pythonlib 18-5-26
		// 这里之所以要把 register 和 initialize 分开，是因为中间要设置 python 的路径，不然它回去寻找本地的pythonhome 和 pythonpath
		// 这里我们先不管这些路径的设置 18_5_26

		// 最终目的
		// Py_SetPath(正确的python lib 路径)

		Py_Initialize();

		if (!Py_IsInitialized())
		{
			Print("Initialize Python failed!");
			return -1;
		}

		ModuleInit();

		// 这里是创建2个路径放进原有 python 模块 sys 的 我们自定义创建的属性 .path 中
		// 当然，我这里的路径直接写随机字符串就可以了, 学习他的写法 18-5-26
		// 这里的目的是，将相应的脚本路径加入到 sys.path 中
		PyObject *newPath = PyList_New(2);

		std::string lib_path = "this is lib_path";
		std::string script_path = "this is script_path";
		PyList_SetItem(newPath, 0, Py_BuildValue("s", lib_path.c_str()));
		PyList_SetItem(newPath, 1, Py_BuildValue("s", script_path.c_str()));

		SetGlobal("sys", "path", "O", newPath);
		// 记得 减少引用
		Py_DECREF(newPath);

		// 屏蔽 gc
		PyRun_SimpleString(
			"import gc\n"
			"gc.disable()\n"
		);

		if (!LoadInit()) {
			Print("LoadInit Failed");
			PythonError();
			return -1;
		}

		// RegisterIMESink() windows下输入法相关 不管

		return 0;
	}

	void End()
	{
		// 结束的时候把 python 关闭 

		if (!s_PythonInit)
			return;

		s_PythonInit = false;
		Print("Py_Finalize begin");

		// Undo all initializations made by Py_Initialize() and subsequent use of Python/C API functions, 
		// and destroy all sub-interpreters (see Py_NewInterpreter() below) that were created 
		// and not yet destroyed since the last call to Py_Initialize(). ....
		// 谷歌查看细节

		Py_Finalize();
		Print("Py_Finalize end");
	}

	static int ConvertResult(PyObject *presult, const char *resFormat, void *resTarget)
	{
		// 按照 resFormat转换 presult?

		if (presult == NULL)
		{
			PythonError();
			return -1;
		}
		if (resTarget == NULL)
		{
			Py_DECREF(presult);
			return 0;
		}
		if (!PyArg_Parse(presult, resFormat, resTarget))
		{
			PythonError();
			Py_DECREF(presult);
			return -1;
		}
		if (strcmp(resFormat, "O") != 0)
		{
			Py_DECREF(presult);
		}
		return 0;
	}

	int RunFunction(const char *modname, const char *funcname,
		const char *resfmt, void *cresult, const char *argfmt, ...)
	{
		PyObject *pmod, *pfunc, *args, *presult;
		pmod = PyImport_ImportModule(modname);
		if (pmod == NULL)
		{
			Print("Can't load %s\n", modname);
			PythonError();
			return -1;
		}

		pfunc = PyObject_GetAttrString(pmod, funcname);
		Py_DECREF(pmod);
		if (pfunc == NULL)
		{
			Print("Can't get %s.%s", modname, funcname);
			return -1;
		}

		va_list argslist;
		va_start(argslist, argfmt);
		args = Py_VaBuildValue(argfmt, argslist);
		va_end(argslist);

		if (args == NULL)
		{
			Print("Py_VaBuildValue error");
			Py_DECREF(pfunc);
			return 1;
		}

		presult = PyObject_CallObject(pfunc, args);

		Py_XDECREF(pfunc);
		Py_XDECREF(args);
		return ConvertResult(presult, resfmt, cresult);
	}

	int GetGlobal(const char *modname, const char *varname, const char *resfmt, void *cresult)
	{
		PyObject *pmod, *pvar;

		pmod = PyImport_ImportModule(modname);
		if (pmod == NULL)
		{
			Print("Can't load %s\n", modname);
			return -1;
		}

		pvar = PyObject_GetAttrString(pmod, varname);
		Py_DECREF(pmod);
		if (pvar == NULL)
		{
			Print("Can't get %s.%s", modname, varname);
			return -1;
		}
		return ConvertResult(pvar, resfmt, cresult);
	}

	int SetGlobal(const char *modname, const char *varname, const char *valfmt, ...)
	{
		int result;
		PyObject *pmod, *val;
		pmod = PyImport_ImportModule(modname);
		if (pmod == NULL)
		{
			Print("Can't load %s\n", modname);
			return -1;
		}

		va_list cvals;
		va_start(cvals, valfmt);
		val = Py_VaBuildValue(valfmt, cvals);
		va_end(cvals);
		if (val == NULL)
			return -1;
		result = PyObject_SetAttrString(pmod, varname, val);
		Py_DECREF(val);
		return result;
	}

	inline int RunMethod(PyObject *pmeth, const char *resfmt, void *cresult, const char *argfmt, va_list argslist)
	{
		PyObject *pargs, *presult;

		pargs = Py_VaBuildValue(argfmt, argslist); /* args: c -> python */
		if (pargs == NULL)
		{
			return -1;
		}
		presult = PyEval_CallObject(pmeth, pargs); /* run interpreter */

		Py_DECREF(pargs);
		return ConvertResult(presult, resfmt, cresult);
	}

	
	// python 报错
	void PythonError(void)
	{
		PyObject *pType, *pValue, *pTraceback, *pStr1, *pStr2;

		PyErr_Fetch(&pType, &pValue, &pTraceback);

		if (pType == NULL || pValue == NULL)
			return;

		// 报错的堆栈全部在下面这个函数里面进行
		// log 输出
		if (pTraceback)
			LogTraceTree(pTraceback);

		pStr1 = PyObject_Str(pType);
		pStr2 = PyObject_Str(pValue);

		// 这里记录的是最后一句的报错信息
		Print("%s: %s", PyUnicode_AsUTF8_Wrapper(pStr1), PyUnicode_AsUTF8_Wrapper(pStr2));

		// 通知脚本有错误发生-方便脚本在错误时做一些逻辑
		// 避免脚本抛出异常造成死循环
		static bool is_handing = false;
		if (!is_handing)
		{
			is_handing = true;
			if (pTraceback)
			{
				std::string traceString = ParseTraceTree2String(pTraceback);
				// 调用脚本的 OnPythonError 这个函数，从来没见过。。
				Python::RunFunction("game", "OnPythonError", "", NULL, "(sss)", traceString.data(),
					PyUnicode_AsUTF8_Wrapper(pStr1), PyUnicode_AsUTF8_Wrapper(pStr2));
			}
			is_handing = false;
		}

		Py_DECREF(pType);
		Py_DECREF(pValue);
		if (pTraceback)
		{
			Py_DECREF(pTraceback);
		}
		Py_DECREF(pStr1);
		Py_DECREF(pStr2);
	}
}
