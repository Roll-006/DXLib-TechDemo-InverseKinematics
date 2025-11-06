#pragma once
#include <DxLib.h>

struct MaterialData
{
	COLOR_F diffuse_color	= { 1.0f, 1.0f, 1.0f, 1.0f };
	COLOR_F specular_color	= { 1.0f, 1.0f, 1.0f, 1.0f };
	COLOR_F emissive_color	= { 1.0f, 1.0f, 1.0f, 1.0f };
	COLOR_F ambient_color	= { 1.0f, 1.0f, 1.0f, 1.0f };
};
