// 简单的材质
#include "demo10_material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../RenderHelp/camera.h"
#include "../RenderHelp/misc.h"

#include "../engineL/openwater.h"

// 在 openwater.h 中定义
// 窗口大小
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

void Demo10_Init()
{
	Demo10_Material_NSP::Demo_Instance().Init();
}

void Demo10_Render()
{
	Demo10_Material_NSP::Demo_Instance().Render();
}

void Demo10_Release()
{
	Demo10_Material_NSP::Demo_Instance().Release();
}

namespace Demo10_Material_NSP
{
	// 渲染的三角形顶点，位置，法线
	const float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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
		lightingShaderPtr = std::make_unique<Shader>("shader_code/demo10_lighting_maps.vs", "shader_code/demo10_lighting_maps.fs");
		lightCubeShaderPtr = std::make_unique<Shader>("shader_code/demo10_light_cube.vs", "shader_code/demo10_light_cube.fs");

		// 渲染相关初始化
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(cubeVAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// lightCubeVAO
		glGenVertexArrays(1, &lightCubeVAO);
		glBindVertexArray(lightCubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// note that we update the lamp's position attribute's stride to reflect the updated buffer data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// 解绑VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// 解绑VAO
		glBindVertexArray(0);

		// load textures (we now use a utility function to keep the code more organized)
		// -----------------------------------------------------------------------------
		diffuseMap = loadTexture("textures/container2.png");
		specularMap = loadTexture("textures/container2_specular.png");

		// shader configuration
		// --------------------
		lightingShaderPtr->use();
		lightingShaderPtr->setInt("material.diffuse", 0);
		lightingShaderPtr->setInt("material.specular", 1);

		// configure global opengl state
		// 打开深度测试
		// -----------------------------
		glEnable(GL_DEPTH_TEST);
	}

	void Demo::Render()
	{
		// 渲染, 这是背景
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// 需要同时清除上一次的深度信息
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShaderPtr->use();
		lightingShaderPtr->setVec3("light.position", lightPos);
		lightingShaderPtr->setVec3("viewPos", GlobalCameraInstance().Ptr->Position);

		// light properties
		lightingShaderPtr->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShaderPtr->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShaderPtr->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		lightingShaderPtr->setFloat("material.shininess", 64.0f);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(GlobalCameraInstance().Ptr->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = GlobalCameraInstance().Ptr->GetViewMatrix();
		lightingShaderPtr->setMat4("projection", projection);
		lightingShaderPtr->setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShaderPtr->setMat4("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

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