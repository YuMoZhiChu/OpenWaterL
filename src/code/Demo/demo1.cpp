// 画一张绿色的背景
#include "demo1.h"

void Demo1_Init()
{
}

void Demo1_Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Demo1_Release()
{
	// 还原成黑色
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
