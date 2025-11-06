#include "bone_pos_corrector.hpp"

void BonePosCorrector::CorrectAimPoseBonePos(const int model_handle, const VECTOR& aim_dir)
{
	// ボーンインデックス取得
	const auto spine_index	= MV1SearchFrame(model_handle, BonePath.SPINE);
	const auto spine1_index	= MV1SearchFrame(model_handle, BonePath.SPINE_1);
	const auto spine2_index	= MV1SearchFrame(model_handle, BonePath.SPINE_2);

	// 回転量を分割
	auto angle				= math::GetAngleBetweenTwoVector(aim_dir, axis::GetWorldYAxis()) - 90.0f * math::kDegToRad;
	angle					+= kAimOffsetAngle * math::kDegToRad;
	const auto spine_angle	= angle * kAimSpineAngleRate;
	const auto spine1_angle	= angle * kAimSpine1AngleRate;
	const auto spine2_angle	= angle * kAimSpine2AngleRate;

	const auto angle1 = angle			* math::kRadToDeg;
	const auto angle2 = spine_angle		* math::kRadToDeg;
	const auto angle3 = spine1_angle	* math::kRadToDeg;
	const auto angle4 = spine2_angle	* math::kRadToDeg;

	// ローカル行列を取得
	auto spine_local_m	= MV1GetFrameLocalMatrix(model_handle, spine_index);
	auto spine1_local_m	= MV1GetFrameLocalMatrix(model_handle, spine1_index);
	auto spine2_local_m	= MV1GetFrameLocalMatrix(model_handle, spine2_index);

	// 座標を保管
	const auto spine_pos	= MGetTranslateElem(spine_local_m);
	const auto spine1_pos	= MGetTranslateElem(spine1_local_m);
	const auto spine2_pos	= MGetTranslateElem(spine2_local_m);

	// 適用
	auto result_spine_local_m	= math::ConvertEulerAnglesToZXYRotMatrix(VGet(-spine_angle, kAimAngle * math::kDegToRad, -spine_angle));
	matrix::SetPos(result_spine_local_m, spine_pos);
	MV1SetFrameUserLocalMatrix(model_handle, spine_index,  result_spine_local_m);

	auto result_spine1_local_m	= math::ConvertEulerAnglesToZXYRotMatrix(VGet(-spine1_angle, 0.0f, -spine1_angle));
	matrix::SetPos(result_spine1_local_m, spine1_pos);
	MV1SetFrameUserLocalMatrix(model_handle, spine1_index, result_spine1_local_m);

	auto result_spine2_local_m	= math::ConvertEulerAnglesToZXYRotMatrix(VGet(-spine2_angle, 0.0f, -spine2_angle));
	matrix::SetPos(result_spine2_local_m, spine2_pos);
	MV1SetFrameUserLocalMatrix(model_handle, spine2_index, result_spine2_local_m);
}
