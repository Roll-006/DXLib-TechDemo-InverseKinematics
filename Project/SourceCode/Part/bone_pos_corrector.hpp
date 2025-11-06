#pragma once
#include <string>
#include <unordered_map>
#include <DxLib.h>

#include "../Kind/time_kind.hpp"
#include "../Path/bone_path.hpp"
#include "../Calculation/math.hpp"

class BonePosCorrector
{
public:
	/// @brief 銃を構えた際のボーンの位置を修正
	/// @param model_handle モデルハンドル
	/// @param aim_dir 狙っている方向
	void CorrectAimPoseBonePos(const int model_handle, const VECTOR& aim_dir);

private:
	static constexpr float kAimAngle			= 20.0f;
	static constexpr float kAimOffsetAngle		= 20.0f;
	static constexpr float kAimSpineAngleRate	= 0.2f;
	static constexpr float kAimSpine1AngleRate	= 0.4f;
	static constexpr float kAimSpine2AngleRate	= 0.4f;
};
