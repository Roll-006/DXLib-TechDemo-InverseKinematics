#include "animation_ik.hpp"

AnimationIK::AnimationIK()
{

}

AnimationIK::~AnimationIK()
{

}

void AnimationIK::Test(const int model_handle, const VECTOR& world_destination)
{
	CalcOneBoneIK(model_handle, world_destination, MV1SearchFrame(model_handle, BonePath.LEFT_HAND));
}

void AnimationIK::LegIK(const int model_handle)
{

}

void AnimationIK::CalcOneBoneIK(const int model_handle, const VECTOR& world_destination, const int frame_index)
{
	auto	   child_local_m	= MV1GetFrameLocalMatrix(model_handle, frame_index);
	const auto child_local_pos	= MGetTranslateElem(child_local_m);
	auto	   child_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, frame_index);
	const auto child_world_pos	= MGetTranslateElem(child_world_m);

	// 変換後のXYZ軸を取得
	// MixamoのモデルはY軸がボーンの進行方向であるため、それを前提に作成する
	// ヨー軸回転 : Z, ピッチ軸回転 : X, ロール軸回転 : Y
	const auto current_axis = math::ConvertRotMatrixToAxis(child_world_m);
    Axis target_axis;
    target_axis.y_axis		= v3d::GetNormalizedV(child_world_pos - child_world_pos);
	target_axis.x_axis		= math::GetNormalVector(current_axis.z_axis, target_axis.y_axis);		// 一時的なピッチ軸回転を計算
    target_axis.z_axis		= math::GetNormalVector(target_axis.y_axis,  target_axis.x_axis);
    target_axis.x_axis		= math::GetNormalVector(target_axis.z_axis,  target_axis.y_axis);		// ピッチ軸回転を再計算
    target_axis.x_axis		= target_axis.x_axis * -1.0f;											// 軸の向き不一致を解消するために反転

	axis::Draw(target_axis, child_world_pos, 50);

	// 子のローカル回転行列を取得
	const auto parent_world_m			= MV1GetFrameLocalWorldMatrix(model_handle, MV1GetFrameParent(model_handle, frame_index));
	const auto parent_world_rot_m		= matrix::GetRotMatrix(parent_world_m);
	const auto parent_world_rot_inv_m	= MInverse(parent_world_rot_m);
	auto	   target_world_rot_m		= math::ConvertAxisToRotMatrix(target_axis);
	child_local_m = target_world_rot_m * parent_world_rot_inv_m;

	// 座標を戻し回転結果をフレームに適用
	matrix::SetPos(child_local_m, child_local_pos);
	MV1SetFrameUserLocalMatrix(model_handle, frame_index, child_local_m);
}
