// 多光源场景
#include "demo11_multiple_lights.h"

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

void Demo11_Init()
{
	Demo11_Multiple_Lights_NSP::Demo_Instance().Init();
}

void Demo11_Render()
{
	Demo11_Multiple_Lights_NSP::Demo_Instance().Render();
}

void Demo11_Release()
{
	Demo11_Multiple_Lights_NSP::Demo_Instance().Release();
}

namespace Demo11_Multiple_Lights_NSP
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

	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

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
		lightingShaderPtr = std::make_unique<Shader>("shader_code/demo11_multiple_lights.vs", "shader_code/demo11_multiple_lights.fs");
		lightCubeShaderPtr = std::make_unique<Shader>("shader_code/demo11_light_cube.vs", "shader_code/demo11_light_cube.fs");

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
		lightingShaderPtr->setVec3("viewPos", GlobalCameraInstance().Ptr->Position);
		lightingShaderPtr->setFloat("material.shininess", 32.0f);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		lightingShaderPtr->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShaderPtr->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShaderPtr->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShaderPtr->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		lightingShaderPtr->setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShaderPtr->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShaderPtr->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShaderPtr->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShaderPtr->setFloat("pointLights[0].constant", 1.0f);
		lightingShaderPtr->setFloat("pointLights[0].linear", 0.09f);
		lightingShaderPtr->setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		lightingShaderPtr->setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShaderPtr->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShaderPtr->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShaderPtr->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShaderPtr->setFloat("pointLights[1].constant", 1.0f);
		lightingShaderPtr->setFloat("pointLights[1].linear", 0.09f);
		lightingShaderPtr->setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		lightingShaderPtr->setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShaderPtr->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShaderPtr->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShaderPtr->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShaderPtr->setFloat("pointLights[2].constant", 1.0f);
		lightingShaderPtr->setFloat("pointLights[2].linear", 0.09f);
		lightingShaderPtr->setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		lightingShaderPtr->setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShaderPtr->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShaderPtr->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShaderPtr->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShaderPtr->setFloat("pointLights[3].constant", 1.0f);
		lightingShaderPtr->setFloat("pointLights[3].linear", 0.09f);
		lightingShaderPtr->setFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		lightingShaderPtr->setVec3("spotLight.position", GlobalCameraInstance().Ptr->Position);
		lightingShaderPtr->setVec3("spotLight.direction", GlobalCameraInstance().Ptr->Front);
		lightingShaderPtr->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShaderPtr->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShaderPtr->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShaderPtr->setFloat("spotLight.constant", 1.0f);
		lightingShaderPtr->setFloat("spotLight.linear", 0.09f);
		lightingShaderPtr->setFloat("spotLight.quadratic", 0.032f);
		lightingShaderPtr->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShaderPtr->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

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

		// render containers
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShaderPtr->setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// also draw the lamp object(s)
		lightCubeShaderPtr->use();
		lightCubeShaderPtr->setMat4("projection", projection);
		lightCubeShaderPtr->setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightCubeShaderPtr->setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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