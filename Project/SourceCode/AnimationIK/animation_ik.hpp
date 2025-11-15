#pragma once
#include <MixamoHelper/mixamo_helper.hpp>
#include <JSON/json_loader.hpp>
#include "../Calculation/math.hpp"
#include "../Path/bone_path.hpp"

class AnimationIK final
{
public:
	enum class PartKind
	{
		kLeftArm,
		kRightArm,
		kLeftLeg,
		kRightLeg,
	};

public:
	AnimationIK();
	~AnimationIK();

	void LeftLegIK(const int model_handle, const VECTOR& world_destination);

private:
	/// @brief 起点とするXYZ軸をもとにして、新たなforwardを基準としたときのXYZ軸を生成する
	/// @param origin_axis 起点とする軸
	/// @param target_forward 基準とするforward
	/// @return 新たに生成したXYZ軸
	[[nodiscard]] Axis CreateAxis(const Axis& origin_axis, const VECTOR& target_forward);

	/// @brief フレーム1つのみに対してIK処理を適用する
	void OneFrameIK	(const int model_handle, const VECTOR& world_destination, const int frame_index);

	/// @brief IK処理を適用する
	/// @param model_handle IK処理が適用されるモデルのハンドル
	/// @param world_destination 目標地点
	/// @param begin_frame_index 起点フレームのインデックス
	/// @param end_frame_index 終点フレームのインデックス
	void FrameIK(const int model_handle, const VECTOR& world_destination, const int begin_frame_index, const int end_frame_index);
};
