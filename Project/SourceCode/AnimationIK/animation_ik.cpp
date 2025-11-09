#include "animation_ik.hpp"

AnimationIK::AnimationIK()
{

}

AnimationIK::~AnimationIK()
{

}

void AnimationIK::LeftLegIK(const int model_handle, const VECTOR& world_destination)
{
	//const auto end_frame			= BonePath.LEFT_LEG;
	//const auto begin_frame			= BonePath.LEFT_UP_LEG;
	//const auto end_frame_index		= MV1SearchFrame(model_handle, end_frame);
	//auto	   current_frame_index	= MV1GetFrameParent(model_handle, end_frame_index);
	//auto	   loop_count			= 0;

	FrameIK(model_handle, world_destination, MV1SearchFrame(model_handle, BonePath.LEFT_UP_LEG), MV1SearchFrame(model_handle, BonePath.LEFT_FOOT));

	//while (true)
	//{
	//	FrameIK(model_handle, world_destination, current_frame_index, end_frame_index);
	//	if (loop_count >= 2)
	//	{
	//		break;
	//	}

	//	current_frame_index = MV1GetFrameParent(model_handle, current_frame_index);

	//	const auto frame_name = MV1GetFrameName(model_handle, current_frame_index);
	//	if (_tcscmp(frame_name, begin_frame) == 0)
	//	{
	//		current_frame_index = MV1GetFrameParent(model_handle, end_frame_index);
	//		++loop_count;
	//	}
	//}
}

Axis AnimationIK::CreateAxis(const Axis& origin_axis, const VECTOR& target_forward)
{
	Axis axis;
	axis.y_axis = target_forward;
	axis.x_axis = math::GetNormalVector(origin_axis.z_axis, axis.y_axis);		// 一時的なピッチ軸回転を計算
	axis.z_axis = math::GetNormalVector(axis.y_axis, axis.x_axis);
	axis.x_axis = math::GetNormalVector(axis.z_axis, axis.y_axis);				// ピッチ軸回転を再計算
	axis.x_axis = axis.x_axis * -1.0f;											// 軸の向き不一致を解消するために反転
	return axis;
}

void AnimationIK::OneFrameIK(const int model_handle, const VECTOR& world_destination, const int frame_index)
{
	auto	   child_local_m	= MV1GetFrameLocalMatrix(model_handle, frame_index);
	const auto child_local_pos	= MGetTranslateElem(child_local_m);
	auto	   child_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, frame_index);
	const auto child_world_pos	= MGetTranslateElem(child_world_m);

	// 変換後のXYZ軸を取得
	// MixamoのモデルはY軸がボーンの進行方向であるため、それを前提に作成する
	// ヨー軸回転 : Z, ピッチ軸回転 : X, ロール軸回転 : Y
	const auto current_axis = math::ConvertRotMatrixToAxis(child_world_m);
	const auto forward		= v3d::GetNormalizedV(world_destination - child_world_pos);
	const auto target_axis	= CreateAxis(current_axis, forward);

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

void AnimationIK::FrameIK(const int model_handle, const VECTOR& world_destination, const int begin_frame_index, const int end_frame_index)
{
	auto	   begin_local_m	= MV1GetFrameLocalMatrix(model_handle, begin_frame_index);
	const auto begin_local_pos	= MGetTranslateElem(begin_local_m);
	auto	   begin_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, begin_frame_index);
	const auto begin_world_pos	= MGetTranslateElem(begin_world_m);

	auto	   end_local_m		= MV1GetFrameLocalMatrix(model_handle, end_frame_index);
	const auto end_local_pos	= MGetTranslateElem(end_local_m);
	auto	   end_world_m		= MV1GetFrameLocalWorldMatrix(model_handle, end_frame_index);
	const auto end_world_pos	= MGetTranslateElem(end_world_m);

	// 終点フレームに回転させた軸から補助行列を取得する
	const auto current_axis		= math::ConvertRotMatrixToAxis(begin_world_m);
	const auto forward			= v3d::GetNormalizedV(end_world_pos - begin_world_pos);
	const auto aid_axis			= CreateAxis(current_axis, forward);
	const auto aid_rot_m		= math::ConvertAxisToRotMatrix(aid_axis);
}
