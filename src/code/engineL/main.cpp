﻿//main.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"

#include "openwater.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 第一次使用前 要加一个 define，就会有定义
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Task/task.h"

#include "../Core/CoreInclude.h"
#include "../Console/Console.h"
#include "../Demo/demo.h"
#include "../RenderHelp/shader_s.h"
#include "../RenderHelp/camera.h"

#include "FBX/FBXTest.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// 窗口大小
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

	// 接入照相机
	// 初始化
	GlobalCamera::Instance(std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f)),
		std::make_shared<CameraData>(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f, true),
		0.0f, 0.0f);
	// 对应 glfw 的回调函数 接入
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// init IO Thread
	CIOTaskManager::Initialize();

	// 选择demo，第一次默认是 0
	DemoTitle demoTitle = CConsoleInstance().ConsoleCommand();

	//// configure global opengl state
	//// -----------------------------
	//glEnable(GL_DEPTH_TEST);

	//// build and compile our shader zprogram
	//// ------------------------------------
	//Shader lightingShader("shader_code/demo9_basic_lighting.vs", "shader_code/demo9_basic_lighting.fs");
	//Shader lightCubeShader("shader_code/demo9_light_cube.vs", "shader_code/demo9_light_cube.fs");


	//// set up vertex data (and buffer(s)) and configure vertex attributes
	//// ------------------------------------------------------------------
	//float vertices[] = {
	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	//	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	//	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	//};
	//// first, configure the cube's VAO (and VBO)
	//unsigned int VBO, cubeVAO;
	//glGenVertexArrays(1, &cubeVAO);
	//glGenBuffers(1, &VBO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindVertexArray(cubeVAO);

	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// normal attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);


	//// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	//unsigned int lightCubeVAO;
	//glGenVertexArrays(1, &lightCubeVAO);
	//glBindVertexArray(lightCubeVAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	// render loop
	// -----------
	// 是否退出循环
	while (!glfwWindowShouldClose(window))
	{

		// per-frame time logic
		// --------------------
		float currentFrame = float(glfwGetTime());
		GlobalCameraInstance().Delta_Time = currentFrame - GlobalCameraInstance().Last_Frame;
		GlobalCameraInstance().Last_Frame = currentFrame;

		// input
		// -----
		// 处理控制台输入
		processInput(window);

		//// render
		//// ------
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// be sure to activate shader when setting uniforms/drawing objects
		//lightingShader.use();
		//lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		//lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		//lightingShader.setVec3("lightPos", lightPos);
		//lightingShader.setVec3("viewPos", GlobalCameraInstance().Ptr->Position);

		//// view/projection transformations
		//glm::mat4 projection = glm::perspective(glm::radians(GlobalCameraInstance().Ptr->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 view = GlobalCameraInstance().Ptr->GetViewMatrix();
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);

		//// world transformation
		//glm::mat4 model = glm::mat4(1.0f);
		//lightingShader.setMat4("model", model);

		//// render the cube
		//glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		//// also draw the lamp object
		//lightCubeShader.use();
		//lightCubeShader.setMat4("projection", projection);
		//lightCubeShader.setMat4("view", view);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		//lightCubeShader.setMat4("model", model);

		//glBindVertexArray(lightCubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);


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

	// 如果有需要释放的，在这里做一次释放
	DemoRelease(demoTitle);

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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		GlobalCameraInstance().Ptr->ProcessKeyboard(FORWARD, GlobalCameraInstance().Delta_Time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		GlobalCameraInstance().Ptr->ProcessKeyboard(BACKWARD, GlobalCameraInstance().Delta_Time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		GlobalCameraInstance().Ptr->ProcessKeyboard(LEFT, GlobalCameraInstance().Delta_Time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		GlobalCameraInstance().Ptr->ProcessKeyboard(RIGHT, GlobalCameraInstance().Delta_Time);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (GlobalCameraInstance().DataPtr->firstMouse)
	{
		GlobalCameraInstance().DataPtr->lastX = float(xpos);
		GlobalCameraInstance().DataPtr->lastY = float(ypos);
		GlobalCameraInstance().DataPtr->firstMouse = false;
	}

	float xoffset = float(xpos) - GlobalCameraInstance().DataPtr->lastX;
	float yoffset = GlobalCameraInstance().DataPtr->lastY - float(ypos); // reversed since y-coordinates go from bottom to top

	GlobalCameraInstance().DataPtr->lastX = float(xpos);
	GlobalCameraInstance().DataPtr->lastY = float(ypos);

	GlobalCameraInstance().Ptr->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	GlobalCameraInstance().Ptr->ProcessMouseScroll(float(yoffset));
}

