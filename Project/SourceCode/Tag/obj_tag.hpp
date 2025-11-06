#pragma once
#include <string>

static struct ObjTag
{
	#pragma region カメラ
	const std::string CAMERA			= "camera";
	const std::string VIRTUAL_CAMERA	= "virtual_camera";
	#pragma endregion

	#pragma region キャラクター
	const std::string PLAYER			= "player";
	#pragma endregion

	#pragma region オブジェクト
	const std::string BUILDING			= "building";
	const std::string STAGE				= "stage";
	const std::string SKYDOME			= "skydome";
	#pragma endregion

}ObjTag;
