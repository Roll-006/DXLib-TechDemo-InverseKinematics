#include "character_collider_creator.hpp"

#pragma region コライダーの生成
void CharacterColliderCreator::CreateCapsuleCollider(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float capsule_radius)
{
	const auto transform		= physical_obj->GetTransform();
	const auto begin_pos		= transform->GetPos(CoordinateKind::kWorld) + VGet(0.0f, capsule_radius, 0.0f);
	const auto capsule_collider = std::make_shared<Capsule>(begin_pos, transform->GetUp(CoordinateKind::kWorld), 0.0f, capsule_radius);

	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, capsule_collider, physical_obj));

	CalcCapsuleColliderDirAndLength(modeler, physical_obj->GetColliderAll(), transform);
}

void CharacterColliderCreator::CreateLandingRay		(PhysicalObjBase* physical_obj, const float lenght)
{
	// TODO : カプセルのサイズの比率によってずらし量を自動で設定させるように変更
	const auto capsule		= std::static_pointer_cast<Capsule>(physical_obj->GetCollider(ColliderKind::kCollider)->GetShape());
	const auto pos			= capsule->GetSegment().GetBeginPos() - VGet(0.0f, 5.0f, 0.0f);

	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLandingRay, std::make_shared<Segment>(pos, -axis::GetWorldYAxis(), lenght), physical_obj));
}

void CharacterColliderCreator::CreateProjectRay		(PhysicalObjBase* physical_obj, const float length)
{
	const auto capsule		= std::static_pointer_cast<Capsule>(physical_obj->GetCollider(ColliderKind::kCollider)->GetShape());
	const auto pos			= capsule->GetSegment().GetBeginPos();

	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kProjectRay, std::make_shared<Segment>(pos, -axis::GetWorldYAxis(), length), physical_obj));
}

void CharacterColliderCreator::CreateVisionTrigger	(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float lenfth, const float fov)
{
	auto head_m = MV1GetFrameLocalWorldMatrix(modeler->GetModelHandle(), MV1SearchFrame(modeler->GetModelHandle(), BonePath.HEAD));
	const auto head_pos  = MGetTranslateElem(head_m);
	const auto head_axis = math::ConvertRotMatrixToAxis(head_m);

	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kMiddleVisionTrigger, std::make_shared<Cone>(head_pos, -head_axis.z_axis, lenfth, fov), physical_obj));

	CalcVisionTriggerPos(modeler, physical_obj->GetColliderAll());
}

void CharacterColliderCreator::CreateVisibleTrigger	(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler)
{
	auto head_m = MV1GetFrameLocalWorldMatrix(modeler->GetModelHandle(), MV1SearchFrame(modeler->GetModelHandle(), BonePath.HEAD));
	const auto head_pos = MGetTranslateElem(head_m);

	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kVisibleTrigger, std::make_shared<Point>(head_pos), physical_obj));

	CalcVisibleTriggerPos(modeler, physical_obj->GetColliderAll());
}

void CharacterColliderCreator::CreateHeadTrigger	(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float sphere_radius)
{
	auto head_m		= MV1GetFrameLocalWorldMatrix(modeler->GetModelHandle(), MV1SearchFrame(modeler->GetModelHandle(), BonePath.HEAD));
	const auto head_pos = MGetTranslateElem(head_m);

	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kHeadTrigger, std::make_shared<Sphere>(head_pos, sphere_radius), physical_obj));

	CalcHeadTriggerPos(modeler, physical_obj->GetColliderAll());
}

void CharacterColliderCreator::CreateBodyTrigger	(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float up_body_capsule_radius, const float down_body_capsule_radius)
{
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kUpBodyTrigger,	 std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), up_body_capsule_radius),   physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kDownBodyTrigger, std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), down_body_capsule_radius), physical_obj));

	CalcBodyTriggerPos(modeler, physical_obj->GetColliderAll());
}

void CharacterColliderCreator::CreateArmTrigger		(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float upper_arm_capsule_radius, const float forearm_capsule_radius, const float hand_capsule_radius)
{
	const auto left_upper_arm_trigger	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), upper_arm_capsule_radius);
	const auto left_forearm_trigger		= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), forearm_capsule_radius);
	const auto left_hand_trigger		= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), hand_capsule_radius);
	const auto right_upper_arm_trigger	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), upper_arm_capsule_radius);
	const auto right_forearm_trigger	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), forearm_capsule_radius);
	const auto right_hand_trigger		= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), hand_capsule_radius);

	// 登録
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLeftUpperArmTrigger,  left_upper_arm_trigger,  physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLeftForearmTrigger,   left_forearm_trigger,	   physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLeftHandTrigger,	  left_hand_trigger,	   physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightUpperArmTrigger, right_upper_arm_trigger, physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightForearmTrigger,  right_forearm_trigger,   physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightHandTrigger,	  right_hand_trigger,	   physical_obj));

	CalcArmTriggerPos(modeler, physical_obj->GetColliderAll());
}

void CharacterColliderCreator::CreateLegTrigger		(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float up_leg_capsule_radius, const float down_leg_capsule_radius)
{
	const auto left_up_leg_trigger		= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), up_leg_capsule_radius);
	const auto left_down_leg_trigger	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), down_leg_capsule_radius);
	const auto right_up_leg_trigger		= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), up_leg_capsule_radius);
	const auto right_down_leg_trigger	= std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), down_leg_capsule_radius);

	// 登録
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLeftUpLegTrigger,	 left_up_leg_trigger,    physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kLeftDownLegTrigger,  left_down_leg_trigger,  physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightUpLegTrigger,	 right_up_leg_trigger,   physical_obj));
	physical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kRightDownLegTrigger, right_down_leg_trigger, physical_obj));

	CalcLegTriggerPos(modeler, physical_obj->GetColliderAll());
}

void CharacterColliderCreator::CreateMeshTrigger	(PhysicalObjBase* phsyical_obj, std::shared_ptr<Modeler>& modeler)
{
	phsyical_obj->AddCollider(std::make_shared<Collider>(ColliderKind::kMeshTrigger, modeler->GetModelHandle(), phsyical_obj));
}
#pragma endregion


#pragma region 位置計算
void CharacterColliderCreator::CalcCapsuleColliderPos(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider)
{
	if (!collider.count(ColliderKind::kCollider)) { return; }

	modeler->ApplyMatrix();
	const auto model_handle = modeler->GetModelHandle();
	const auto transform	= modeler->GetTransform();

	// 位置を取得
	auto	   hips_m		= MV1GetFrameLocalWorldMatrix(modeler->GetModelHandle(), MV1SearchFrame(modeler->GetModelHandle(), BonePath.HIPS));
	auto	   head_top_m	= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.HEAD_TOP_END));
	const auto hips_pos		= MGetTranslateElem(hips_m);
	const auto pos			= transform->GetPos(CoordinateKind::kWorld);

	const auto capsule			= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kCollider)->GetShape());
	const auto begin_pos		= VECTOR(hips_pos.x, pos.y + capsule->GetRadius(), hips_pos.z);
	const auto capsule_length	= VSize(pos - MGetTranslateElem(head_top_m)) - capsule->GetRadius() * 2.0f;
	capsule->SetSegmentBeginPos(begin_pos, true);
	capsule->SetSegmentEndPos  (begin_pos + transform->GetUp(CoordinateKind::kWorld) * capsule_length, true);
}

void CharacterColliderCreator::CalcLandingRayPos(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider)
{
	if (!collider.count(ColliderKind::kCollider)) { return; }

	modeler->ApplyMatrix();
	const auto model_handle = modeler->GetModelHandle();

	const auto segment		= std::static_pointer_cast<Segment>(collider.at(ColliderKind::kLandingRay)->GetShape());
	const auto capsule		= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kCollider)  ->GetShape());
	const auto pos			= capsule->GetSegment().GetBeginPos() - VGet(0.0f, 5.0f, 0.0f);

	segment->SetBeginPos(pos, false);
}

void CharacterColliderCreator::CalcProjectRayPos	(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider)
{
	if (!collider.count(ColliderKind::kCollider)) { return; }

	modeler->ApplyMatrix();
	const auto model_handle = modeler->GetModelHandle();

	const auto segment		= std::static_pointer_cast<Segment>(collider.at(ColliderKind::kProjectRay)->GetShape());
	const auto capsule		= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kCollider)  ->GetShape());
	const auto pos			= capsule->GetSegment().GetBeginPos();

	segment->SetBeginPos(pos, false);
}

void CharacterColliderCreator::CalcVisionTriggerPos	(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider)
{
	modeler->ApplyMatrix();
	const auto model_handle = modeler->GetModelHandle();

	// 位置を取得
	auto head_m = MV1GetFrameLocalWorldMatrix(modeler->GetModelHandle(), MV1SearchFrame(modeler->GetModelHandle(), BonePath.HEAD));
	const auto head_pos	 = MGetTranslateElem(head_m);
	const auto head_axis = math::ConvertRotMatrixToAxis(head_m);

	const auto cone = std::static_pointer_cast<Cone>(collider.at(ColliderKind::kMiddleVisionTrigger)->GetShape());
	cone->SetDir(-head_axis.z_axis);
	cone->SetVertex(head_pos);
}

void CharacterColliderCreator::CalcVisibleTriggerPos(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider)
{
	modeler->ApplyMatrix();
	const auto model_handle = modeler->GetModelHandle();

	// 位置を取得
	auto spine2_m = MV1GetFrameLocalWorldMatrix(modeler->GetModelHandle(), MV1SearchFrame(modeler->GetModelHandle(), BonePath.SPINE_2));
	const auto spine2_pos = MGetTranslateElem(spine2_m);

	const auto cone = std::static_pointer_cast<Point>(collider.at(ColliderKind::kVisibleTrigger)->GetShape());
	cone->SetPos(spine2_pos);
}

void CharacterColliderCreator::CalcHeadTriggerPos	(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider)
{
	modeler->ApplyMatrix();
	const auto model_handle = modeler->GetModelHandle();

	// 位置を取得
	auto head_m				= MV1GetFrameLocalWorldMatrix(modeler->GetModelHandle(), MV1SearchFrame(modeler->GetModelHandle(), BonePath.HEAD));
	const auto head_pos		= MGetTranslateElem(head_m);

	std::static_pointer_cast<Sphere>(collider.at(ColliderKind::kHeadTrigger)->GetShape())->SetPos(head_pos);
}

void CharacterColliderCreator::CalcBodyTriggerPos	(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider)
{
	modeler->ApplyMatrix();
	const auto model_handle = modeler->GetModelHandle();

	// 行列情報を取得
	auto left_shoulder_m	= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_SHOULDER));
	auto right_shoulder_m	= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_SHOULDER));
	auto spine2_m			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.SPINE_2));
	auto hips_m				= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.HIPS));

	// 位置を取得
	const auto left_shoulder_pos	= MGetTranslateElem(left_shoulder_m);
	const auto right_shoulder_pos	= MGetTranslateElem(right_shoulder_m);
	const auto spine2_pos			= MGetTranslateElem(spine2_m);
	const auto hips_pos				= MGetTranslateElem(hips_m);

	// 位置を適用
	const auto up_body_capsule	 = std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kUpBodyTrigger)  ->GetShape());
	const auto down_body_capsule = std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kDownBodyTrigger)->GetShape());

	up_body_capsule  ->SetSegmentBeginPos(left_shoulder_pos,	true);
	up_body_capsule  ->SetSegmentEndPos  (right_shoulder_pos,	true);
	down_body_capsule->SetSegmentBeginPos(spine2_pos,			true);
	down_body_capsule->SetSegmentEndPos  (hips_pos,				true);
}

void CharacterColliderCreator::CalcArmTriggerPos	(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider)
{
	modeler->ApplyMatrix();
	const auto model_handle = modeler->GetModelHandle();

	// フレームの行列情報を取得
	auto left_arm_m						= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_ARM));
	auto left_forearm_m					= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_FORE_ARM));
	auto left_hand_m					= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_HAND));
	auto left_hand_middle1_m			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_HAND_MIDDLE_3));
	auto right_arm_m					= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_ARM));
	auto right_forearm_m				= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_FORE_ARM));
	auto right_hand_m					= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_HAND));
	auto right_hand_middle1_m			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_HAND_MIDDLE_3));

	// 位置を取得
	const auto left_arm_pos				= MGetTranslateElem(left_arm_m);
	const auto left_forearm_pos			= MGetTranslateElem(left_forearm_m);
	const auto left_hand_pos			= MGetTranslateElem(left_hand_m);
	const auto left_hand_middle1_pos	= MGetTranslateElem(left_hand_middle1_m);
	const auto right_arm_pos			= MGetTranslateElem(right_arm_m);
	const auto right_forearm_pos		= MGetTranslateElem(right_forearm_m);
	const auto right_hand_pos			= MGetTranslateElem(right_hand_m);
	const auto right_hand_middle1_pos	= MGetTranslateElem(right_hand_middle1_m);

	// 位置を適用
	const auto left_upper_arm_capsule	= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kLeftUpperArmTrigger)	->GetShape());
	const auto left_forearm_capsule		= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kLeftForearmTrigger)	->GetShape());
	const auto left_hand_capsule		= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kLeftHandTrigger)		->GetShape());
	const auto right_upper_arm_capsule	= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kRightUpperArmTrigger)->GetShape());
	const auto right_forearm_capsule	= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kRightForearmTrigger)	->GetShape());
	const auto right_hand_capsule		= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kRightHandTrigger)	->GetShape());

	left_upper_arm_capsule	->SetSegmentBeginPos(left_forearm_pos,		 true);
	left_upper_arm_capsule	->SetSegmentEndPos	(left_arm_pos,			 true);
	left_forearm_capsule	->SetSegmentBeginPos(left_hand_pos,			 true);
	left_forearm_capsule	->SetSegmentEndPos	(left_forearm_pos,		 true);
	left_hand_capsule		->SetSegmentBeginPos(left_hand_middle1_pos,  true);
	left_hand_capsule		->SetSegmentEndPos	(left_hand_pos,			 true);
	right_upper_arm_capsule	->SetSegmentBeginPos(right_forearm_pos,		 true);
	right_upper_arm_capsule	->SetSegmentEndPos	(right_arm_pos,			 true);
	right_forearm_capsule	->SetSegmentBeginPos(right_hand_pos,		 true);
	right_forearm_capsule	->SetSegmentEndPos	(right_forearm_pos,		 true);
	right_hand_capsule		->SetSegmentBeginPos(right_hand_middle1_pos, true);
	right_hand_capsule		->SetSegmentEndPos	(right_hand_pos,		 true);
}

void CharacterColliderCreator::CalcLegTriggerPos	(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider)
{
	modeler->ApplyMatrix();
	const auto model_handle		= modeler->GetModelHandle();

	// フレームの行列情報を取得
	auto left_up_leg_m			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_UP_LEG));
	auto left_leg_m				= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_LEG));
	auto left_foot_m			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.LEFT_FOOT));
	auto right_up_leg_m			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_UP_LEG));
	auto right_leg_m			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_LEG));
	auto right_foot_m			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_FOOT));

	// 位置を取得
	const auto left_up_leg_pos	= MGetTranslateElem(left_up_leg_m);
	const auto left_leg_pos		= MGetTranslateElem(left_leg_m);
	const auto left_foot_pos	= MGetTranslateElem(left_foot_m);
	const auto right_up_leg_pos = MGetTranslateElem(right_up_leg_m);
	const auto right_leg_pos	= MGetTranslateElem(right_leg_m);
	const auto right_foot_pos	= MGetTranslateElem(right_foot_m);

	// 位置を適用
	const auto left_up_leg_capsule		= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kLeftUpLegTrigger)->GetShape());
	const auto left_down_leg_capsule	= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kLeftDownLegTrigger)->GetShape());
	const auto right_up_leg_capsule		= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kRightUpLegTrigger)->GetShape());
	const auto right_down_leg_capsule	= std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kRightDownLegTrigger)->GetShape());

	left_up_leg_capsule   ->SetSegmentBeginPos	(left_leg_pos,		true);
	left_up_leg_capsule   ->SetSegmentEndPos	(left_up_leg_pos,	true);
	left_down_leg_capsule ->SetSegmentBeginPos	(left_foot_pos,		true);
	left_down_leg_capsule ->SetSegmentEndPos	(left_leg_pos,		true);
	right_up_leg_capsule  ->SetSegmentBeginPos	(right_leg_pos,		true);
	right_up_leg_capsule  ->SetSegmentEndPos	(right_up_leg_pos,	true);
	right_down_leg_capsule->SetSegmentBeginPos	(right_foot_pos,	true);
	right_down_leg_capsule->SetSegmentEndPos	(right_leg_pos,		true);
}
#pragma endregion


#pragma region その他計算
void CharacterColliderCreator::CalcCapsuleColliderDirAndLength(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider, const std::shared_ptr<Transform>& transform)
{
	if (!collider.count(ColliderKind::kCollider)) { return; }

	modeler->ApplyMatrix();

	// 行列情報を取得
	const auto model_handle		= modeler->GetModelHandle();
	auto	   head_top_m		= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.HEAD_TOP_END));

	// 始点から頭部までの長さを取得
	const auto capsule_length = VSize(transform->GetPos(CoordinateKind::kWorld) - MGetTranslateElem(head_top_m));
	const auto capsule = std::static_pointer_cast<Capsule>(collider.at(ColliderKind::kCollider)->GetShape());
	capsule->SetCapsuleLength(capsule_length);
	capsule->SetDir(transform->GetUp(CoordinateKind::kWorld));
}
#pragma endregion
