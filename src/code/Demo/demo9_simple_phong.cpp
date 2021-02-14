// 简单的冯氏光照模型
#include "demo9_simple_phong.h"

// 如果是 extern, 就直接 #include 即可
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../RenderHelp/camera.h"

#include "../engineL/openwater.h"

// 在 openwater.h 中定义
// 窗口大小
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

void Demo9_Init()
{
	Demo9_Simple_Phong_NSP::Demo_Instance().Init();
}

void Demo9_Render()
{
	Demo9_Simple_Phong_NSP::Demo_Instance().Render();
}

void Demo9_Release()
{
	Demo9_Simple_Phong_NSP::Demo_Instance().Release();
}

namespace Demo9_Simple_Phong_NSP
{
	// 渲染的三角形顶点，位置，法线
	const float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};


	// lighting
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	Demo& Demo::Instance()
	{
		static Demo _instance;
		return _instance;
	}

	void Demo::Init()
	{
		// build and compile our shader program
		// ------------------------------------
		// 建立 Shader Program
		lightingShaderPtr = std::make_unique<Shader>("shader_code/demo9_basic_lighting.vs", "shader_code/demo9_basic_lighting.fs");
		lightCubeShaderPtr = std::make_unique<Shader>("shader_code/demo9_light_cube.vs", "shader_code/demo9_light_cube.fs");

		// 渲染相关初始化
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		// 绑定 VAO（顶点数组数据），
		glBindVertexArray(cubeVAO);
		// VBO 解析顶点数据
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// lightCubeVAO
		glGenVertexArrays(1, &lightCubeVAO);
		glBindVertexArray(lightCubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// note that we update the lamp's position attribute's stride to reflect the updated buffer data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// 解绑VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// 解绑VAO
		glBindVertexArray(0);

		// configure global opengl state
		// 打开深度测试
		// -----------------------------
		glEnable(GL_DEPTH_TEST);
	}

	void Demo::Render()
	{
		// 渲染, 这是背景
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// 需要同时清除上一次的深度信息
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShaderPtr->use();
		lightingShaderPtr->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShaderPtr->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShaderPtr->setVec3("lightPos", lightPos);
		lightingShaderPtr->setVec3("viewPos", GlobalCameraInstance().Ptr->Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(GlobalCameraInstance().Ptr->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = GlobalCameraInstance().Ptr->GetViewMatrix();
		lightingShaderPtr->setMat4("projection", projection);
		lightingShaderPtr->setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShaderPtr->setMat4("model", model);

		// render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// also draw the lamp object
		lightCubeShaderPtr->use();
		lightCubeShaderPtr->setMat4("projection", projection);
		lightCubeShaderPtr->setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lightCubeShaderPtr->setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void Demo::Release()
	{
		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteVertexArrays(1, &lightCubeVAO);
		glDeleteBuffers(1, &VBO);

		// 关闭深度测试
		glDisable(GL_DEPTH_TEST);

		lightingShaderPtr->deleteProgram();
		lightCubeShaderPtr->deleteProgram();
	}
}