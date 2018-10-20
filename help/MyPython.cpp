//#include "stdafx.h"
//#include <sstream>
//
//#include "MyPython.h"
//
//// include 各个模块
//#include "PySys.h"
//
//static bool s_PythonInit = false;
//
//// 无用的函数
//// PyMODINIT_FUNC init_struct(void);
//// PyMODINIT_FUNC initcStringIO(void);
//
//namespace Python
//{
//	// PyObject* DNetInit(); // 网络相关？
//
//	// void RegisterIMESink(); // windows tsf ime 相关  是和输入法有关系的一套流程 现在先不管 18_5_26
//
//	int LoadInit()
//	{
//		PyObject* pInit = NULL;
//		if (!pInit)
//		{
//			PythonError();
//			return -1;
//		}
//		Py_DECREF(pInit);
//		return 0;
//	}
//
//	std::string GetAbsolutePath()
//	{
//		std::string absolute_path;
//		// absolute_path = CFile::GetDataPath(); // 涉及到CFile
//		return absolute_path;
//	}
//
//	void ModuleRegister()
//	{
//		// py3 套路 1.注册 2.Py_Initialize 3.import
//
//		// XXXXX ModuelRegister
//
//		// 比如说我们要建立一个叫 C_sys 的模块
//		// 这里是第一步
//		SysModuleRegister();
//	}
//
//	void ModuleInit()
//	{
//		// 这里是第三步骤 import 
//		SysInit();
//	}
//
//	bool CheckFlsExist(const std::string& fls_path)
//	{
//		// 关于使用 fls 打包方面，先不管 18_5_26
//		return false;
//	}
//
//	// 初始化函数 对外就是 Python::Init 表示初始化了整个 Python 系统
//	int Init()
//	{
//		if (s_PythonInit)
//			return 0;
//
//		s_PythonInit = true;
//
//		// AndroidStdeerToLogcat(); 安卓相关 把标准错误输出到 logcat  18-5-26 先不管
//		
//		// 模块注册
//		ModuleRegister();
//
//		// python set path 相关 , 现在先不管
//
//		// 排除系统的path 对 编辑器/游戏的影响
//		// ... ...
//		// 之后再看如何准确的定位我们的 pythonhome 和 pythonlib 18-5-26
//		// 这里之所以要把 register 和 initialize 分开，是因为中间要设置 python 的路径，不然它回去寻找本地的pythonhome 和 pythonpath
//		// 这里我们先不管这些路径的设置 18_5_26
//
//		Py_Initialize();
//
//		if (!Py_IsInitialized())
//		{
//			Print("Initialize Python failed!");
//			return -1;
//		}
//
//		ModuleInit();
//
//		// 这里是创建2个路径放进原有 python 模块 sys 的 我们自定义创建的属性 .path 中
//		// 当然，我这里的路径直接写随机字符串就可以了, 学习他的写法 18-5-26
//		PyObject *newPath = PyList_New(2);
//
//		return 0;
//	}
//}
