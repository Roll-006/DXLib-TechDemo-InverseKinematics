#include "animation_ik.hpp"

AnimationIK::AnimationIK()
{

}

AnimationIK::~AnimationIK()
{

}

void AnimationIK::Test(const int model_handle, const VECTOR& world_destination)
{
	const auto frame_index		= MV1SearchFrame(model_handle, BonePath.LEFT_HAND);
	auto	   frame_m			= MV1GetFrameLocalMatrix(model_handle, frame_index);

	CalcOneBoneIK(world_destination, frame_m);
	MV1SetFrameUserLocalMatrix(model_handle, frame_index, frame_m);
}

void AnimationIK::LegIK(const int model_handle)
{

}

void AnimationIK::CalcOneBoneIK(const VECTOR& world_destination, MATRIX& frame_local_matrix)
{
	//const auto bone_pos = MGetTranslateElem(bone_matrix);
	//const auto local_destination = destination - bone_pos;
	//const auto squared_x = std::pow(local_destination.x, 2);
	//const auto squared_y = std::pow(local_destination.y, 2);
	//const auto squared_z = std::pow(local_destination.z, 2);

	//// ヨー軸回転を取得
	//const auto cos_y = local_destination.x / std::sqrt(squared_x + squared_z);
	//const auto sin_y = local_destination.z / std::sqrt(squared_x + squared_z);
	//const auto yaw_m = matrix::CreateYMatrix(cos_y, sin_y);

	//// ピッチ軸回転値を取得
	//const auto cos_x = std::sqrt(squared_x + squared_z) / std::sqrt(squared_x + squared_y + squared_z);
	//const auto sin_x = local_destination.y / std::sqrt(squared_x + squared_y + squared_z);
	//const auto pitch_m = matrix::CreateXMatrix(cos_x, sin_x);

	//// 回転を適用
	//bone_matrix = yaw_m * pitch_m * bone_matrix;

	// MixamoのモデルはY軸がボーンの進行方向であるため、それを前提に作成する
	// ヨー軸回転 : Z, ピッチ軸回転 : X, ロール軸回転 : Y
	const auto frame_local_pos		= MGetTranslateElem(frame_local_matrix);
	const auto frame_axis			= math::ConvertRotMatrixToAxis(frame_local_matrix);
	const auto local_destination	= world_destination - frame_local_pos;
	const auto dir					= v3d::GetNormalizedV(local_destination - frame_local_pos);

	// ヨー軸回転値を取得
	const auto dir_z0		= VGet(dir.x, dir.y, 0.0f);
	const auto z_axis_z0	= VGet(frame_axis.z_axis.x, frame_axis.z_axis.y, 0.0f);
	const auto yaw			= math::GetYaw(dir_z0 - z_axis_z0);
	const auto yaw_m		= MGetRotZ(yaw);

	// ピッチ軸回転値を取得
	//const auto pitch		= math::GetPitch(dir - frame_axis.x_axis);
	//const auto pitch_m		= MGetRotZ(pitch);

	// 回転を適用
	frame_local_matrix = yaw_m * /*pitch_m * */MGetRotElem(frame_local_matrix);
	matrix::SetPos(frame_local_matrix, frame_local_pos);
}
