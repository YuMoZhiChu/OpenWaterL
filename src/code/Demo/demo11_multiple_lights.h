#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <memory>

#include "../RenderHelp/shader_s.h"

void Demo11_Init();

void Demo11_Render();

void Demo11_Release();

// 搞成作用域，这样就可以统一写了
namespace Demo11_Multiple_Lights_NSP
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
		// 纹理
		unsigned int diffuseMap, specularMap;
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