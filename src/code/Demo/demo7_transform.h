#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <memory>

#include "../RenderHelp/shader_s.h"

void Demo7_Init();

void Demo7_Render();

void Demo7_Release();

// 搞成作用域，这样就可以统一写了
namespace Demo7_Transform_NSP
{
	// 用一个类来包括
	// 一般来说，都做成一个单例
	class Demo
	{
		// Shader 的指针
		std::unique_ptr<Shader> shaderProgramPtr;
		// VAO, VBO, EBO
		unsigned int VAO, VBO, EBO;
		// 纹理 1，2
		unsigned int texture1, texture2;
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