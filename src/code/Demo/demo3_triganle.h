#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void Demo3_Init();

void Demo3_Render();

void Demo3_Release();

// 搞成作用域，这样就可以统一写了
namespace Demo3_Triangle_NSP
{
	// 用一个类来包括
	// 一般来说，都做成一个单例
	class Demo
	{
		// 链接 shaderProgram 的ID
		unsigned int shaderProgram;
		// VAO, VBO
		unsigned int VAO, VBO;
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