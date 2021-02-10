#pragma once

#include "demo1.h"
#include "demo2.h"
// 对应每个 demo 的变量
enum DemoTitle
{
	Demo0_Zero,
	Demo1_Pure_Green,
	Demo2_Pure_Red,
};

void DemoInit(DemoTitle demoTitle);

void DemoRender(DemoTitle demoTitle);

void DemoRelease(DemoTitle demoTitle);
