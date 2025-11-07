#pragma once
#include <string>

static struct ObjName
{
	#pragma region カメラ
	const std::string MAIN_CAMERA		= "main_camera";
	const std::string FREEDOM_CAMERA	= "freedom_camera";
	const std::string ROT_STAND_CAMERA	= "rot_stand_camera";
	const std::string ROT_CROUCH_CAMERA	= "rot_crouch_camera";
	#pragma endregion


	#pragma region キャラクター
	const std::string PLAYER			= "player";
	#pragma endregion


	#pragma region オブジェクト
	const std::string STAGE				= "stage";
	#pragma endregion


	#pragma region スカイドーム
	const std::string SKYDOME			= "skydome";
	#pragma endregion

}ObjName;
