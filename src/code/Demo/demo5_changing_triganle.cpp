// 画一个变色的三角形
#include "demo5_changing_triganle.h"

void Demo5_Init()
{
	Demo5_Changing_Triangle_NSP::Demo_Instance().Init();
}

void Demo5_Render()
{
	Demo5_Changing_Triangle_NSP::Demo_Instance().Render();
}

void Demo5_Release()
{
	Demo5_Changing_Triangle_NSP::Demo_Instance().Release();
}

namespace Demo5_Changing_Triangle_NSP
{
	// 渲染的三角形顶点和颜色数据
	const float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	Demo& Demo::Instance()
	{
		static Demo _instance;
		return _instance;
	}

	void Demo::Init()
	{
		// 建立 Shader Program
		shaderProgramPtr = std::make_unique<Shader>("shader_code/demo5_changing_triangle.vs", "shader_code/demo5_changing_triangle.fs");

		// 渲染相关初始化
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// 解绑VBO
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		// 解绑VAO
		glBindVertexArray(0);
	}

	void Demo::Render()
	{
		// 渲染, 这是背景
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgramPtr->use();
		// 更新时间数据, 映射到 [0,1]
		shaderProgramPtr->setFloat("color_scale", sin(float(glfwGetTime())) / 2.0f + 0.5f);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // no need to unbind it every time 
	}

	void Demo::Release()
	{
		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		shaderProgramPtr->deleteProgram();
	}
}