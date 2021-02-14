#pragma once

#include "../engineL/openwater.h"

#include "demo1.h"
#include "demo2.h"
#include "demo3_triganle.h"
#include "demo4_two_triganle_IBO.h"
#include "demo5_changing_triganle.h"
#include "demo6_texture.h"
#include "demo7_transform.h"
#include "demo8_coordinate_systems.h"
#include "demo9_simple_phong.h"
// 对应每个 demo 的变量
enum DemoTitle
{
	Demo0_Zero,
	Demo1_Pure_Green,
	Demo2_Pure_Red,
	Demo3_Triangle,
	Demo4_Two_Triangle_IBO,
	Demo5_Changing_Triangle,
	Demo6_Texture,
	Demo7_Transform,
	Demo8_Coordinate_Systems,
	Demo9_Simple_Phong,
};

void DemoInit(DemoTitle demoTitle);

void DemoRender(DemoTitle demoTitle);

void DemoRelease(DemoTitle demoTitle);
