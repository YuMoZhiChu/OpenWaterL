#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <memory>

#include "../RenderHelp/shader_s.h"
#include "../RenderHelp/model.h"

void Demo12_Init();

void Demo12_Render();

void Demo12_Release();

// 搞成作用域，这样就可以统一写了
namespace Demo12_Model_Loading_NSP
{
	// 用一个类来包括
	// 一般来说，都做成一个单例
	class Demo
	{
		// Shader 的指针
		std::unique_ptr<Shader> modelLoadingShaderPtr;
		// Model 的指针
		std::unique_ptr<Model> modelPtr;
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