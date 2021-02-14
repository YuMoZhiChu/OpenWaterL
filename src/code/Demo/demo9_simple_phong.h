#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <memory>

#include "../RenderHelp/shader_s.h"

void Demo9_Init();

void Demo9_Render();

void Demo9_Release();

// 搞成作用域，这样就可以统一写了
namespace Demo9_Simple_Phong_NSP
{
	// 用一个类来包括
	// 一般来说，都做成一个单例
	class Demo
	{
		// Shader 的指针
		std::unique_ptr<Shader> lightingShaderPtr;
		std::unique_ptr<Shader> lightCubeShaderPtr;
		// VAO, VBO
		unsigned int cubeVAO, lightCubeVAO, VBO;
	public:
		static Demo& Instance(void);
		void Init();
		void Render();
		void Release();
	};

	// 单例模式
	// 返回一个右值
	inline Demo& Demo_Instance()
	{
		return Demo::Instance();
	}
}