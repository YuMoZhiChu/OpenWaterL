// 多光源场景
#include "demo12_model_loading.h"

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

void Demo12_Init()
{
	Demo12_Model_Loading_NSP::Demo_Instance().Init();
}

void Demo12_Render()
{
	Demo12_Model_Loading_NSP::Demo_Instance().Render();
}

void Demo12_Release()
{
	Demo12_Model_Loading_NSP::Demo_Instance().Release();
}

namespace Demo12_Model_Loading_NSP
{
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
		modelLoadingShaderPtr = std::make_unique<Shader>("shader_code/demo12_model_loading.vs", "shader_code/demo12_model_loading.fs");

		// load models
		// -----------
		// 读 model
		modelPtr = std::make_unique<Model>("model/nanosuit/nanosuit.obj");

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

		// don't forget to enable shader before setting uniforms
		modelLoadingShaderPtr->use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(GlobalCameraInstance().Ptr->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = GlobalCameraInstance().Ptr->GetViewMatrix();
		modelLoadingShaderPtr->setMat4("projection", projection);
		modelLoadingShaderPtr->setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		modelLoadingShaderPtr->setMat4("model", model);
		modelPtr->Draw(*modelLoadingShaderPtr);
	}

	void Demo::Release()
	{

		// 关闭深度测试
		glDisable(GL_DEPTH_TEST);

		modelLoadingShaderPtr->deleteProgram();
	}
}