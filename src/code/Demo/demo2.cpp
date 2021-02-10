// 画一张红色的背景
#include "demo1.h"

void Demo2_Init()
{
}

void Demo2_Render()
{
	glClearColor(0.8f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Demo2_Release()
{
	// 还原成黑色
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
