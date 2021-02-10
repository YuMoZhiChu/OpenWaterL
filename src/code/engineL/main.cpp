//main.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Task/task.h"

#include "../Core/CoreInclude.h"
#include "../Console/Console.h"
#include "../Demo/demo.h"

#include "FBX/FBXTest.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	//FBXTest();

	//CConsoleInstance().Show(true);

	// glfw: initialize and configure
	// ------------------------------
	// 初始化 GLFW
	glfwInit();
	// OpenGL 的版本号3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 核心模式-只能使用OpenGL功能的一个子集
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac Os X系统 需要用这个

	// glfw window creation
	// --------------------
	// 创建 OpenGL 窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 当前 Context 对应的线程 对应该 window
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//// 测试，获取最大的 uniform 支持大小
	//int a, b, c;
	//glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &a);
	//glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &b);
	//glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &c);

	// init IO Thread
	CIOTaskManager::Initialize();

	// 选择demo，第一次默认是 0
	DemoTitle demoTitle = CConsoleInstance().ConsoleCommand();

	// render loop
	// -----------
	// 是否退出循环
	while (!glfwWindowShouldClose(window))
	{

		// input
		// -----
		// 处理控制台输入
		processInput(window);

		// render


		// 对应 Demo 的渲染
		DemoTitle temp = CConsoleInstance().ConsoleCommand();
		if (temp != Demo0_Zero)
		{
			if (temp != demoTitle)
			{
				// step 1 还原原来的 Demo
				DemoRelease(demoTitle);
				DemoInit(temp);
				demoTitle = temp;
			}
		}
		DemoRender(demoTitle);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		// 交换颜色缓冲
		glfwSwapBuffers(window);
		// 检测触发事件
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	// release io task
	CIOTaskManager::Release();

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// 设置窗口大小
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

