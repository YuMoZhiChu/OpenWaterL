#include "demo.h"

void DemoInit(DemoTitle demoTitle)
{
	switch (demoTitle)
	{
	case DemoTitle::Demo0_Zero:
		return;
	case DemoTitle::Demo1_Pure_Green:
		Demo1_Init();
		return;
	case DemoTitle::Demo2_Pure_Red:
		Demo2_Init();
		return;
	case DemoTitle::Demo3_Triangle:
		Demo3_Init();
		return;
	case DemoTitle::Demo4_Two_Triangle_IBO:
		Demo4_Init();
		return;
	default:
		break;
	}
}

void DemoRender(DemoTitle demoTitle)
{
	switch (demoTitle)
	{
	case DemoTitle::Demo0_Zero:
		return;
	case DemoTitle::Demo1_Pure_Green:
		Demo1_Render();
		return;
	case DemoTitle::Demo2_Pure_Red:
		Demo2_Render();
		return;
	case DemoTitle::Demo3_Triangle:
		Demo3_Render();
		return;
	case DemoTitle::Demo4_Two_Triangle_IBO:
		Demo4_Render();
		return;
	default:
		break;
	}
}

void DemoRelease(DemoTitle demoTitle)
{
	switch (demoTitle)
	{
	case DemoTitle::Demo0_Zero:
		return;
	case DemoTitle::Demo1_Pure_Green:
		Demo1_Release();
		return;
	case DemoTitle::Demo2_Pure_Red:
		Demo2_Release();
		return;
	case DemoTitle::Demo3_Triangle:
		Demo3_Release();
		return;
	case DemoTitle::Demo4_Two_Triangle_IBO:
		Demo4_Release();
		return;
	default:
		break;
	}
}
