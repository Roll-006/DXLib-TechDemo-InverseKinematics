#include "camera_body.hpp"

CameraBody::CameraBody(const std::shared_ptr<Transform>& owner_transform) :
	m_owner_transform	(owner_transform),
	m_target_transform	(nullptr),
	m_destination_pos	(v3d::GetZeroV()),
	m_current_pos		(v3d::GetZeroV()),
	m_follow_offset		(v3d::GetZeroV()),
	m_damping			(v3d::GetZeroV()),
	//m_damping_yaw		(0.0f),
	//m_damping_pitch	(0.0f),
	m_is_track			(false)
{
	// 処理なし
}

CameraBody::~CameraBody()
{
	// 処理なし
}

void CameraBody::CalcPos()
{
	m_destination_pos = m_target_transform->GetPos(CoordinateKind::kWorld);

	// ターゲットの軸をもとに位置を決定
	const auto target_axis = m_target_transform->GetAxis(CoordinateKind::kWorld);
	m_destination_pos += target_axis.x_axis * m_follow_offset.x;
	m_destination_pos += target_axis.y_axis * m_follow_offset.y;
	m_destination_pos += target_axis.z_axis * m_follow_offset.z;

	m_owner_transform->SetPos(CoordinateKind::kWorld, m_destination_pos);
}

void CameraBody::CalcDampedPos()
{
	const auto time_manager = GameTimeManager::GetInstance();
	const auto owner_axis	= m_owner_transform->GetAxis(CoordinateKind::kWorld);

	m_current_pos = math::GetDampedValueOnAxis(
		m_current_pos, m_destination_pos, m_damping, 
		owner_axis, time_manager->GetDeltaTime(TimeScaleLayerKind::kCamera));

	m_owner_transform->SetPos(CoordinateKind::kWorld, m_current_pos);
}


#pragma region Attach / Detach
void CameraBody::AttachTarget(const std::shared_ptr<Transform>& target_transform)
{
	m_target_transform		= target_transform;
	m_is_track				= true;
}

void CameraBody::DetachTarget()
{
	m_target_transform		= nullptr;
	m_is_track				= false;
}
#pragma endregion


#pragma region Setter
void CameraBody::SetDamping(const VECTOR& damping)
{
	m_damping.x = std::clamp(damping.x, 0.0f, kMaxDampingNum);
	m_damping.y = std::clamp(damping.y, 0.0f, kMaxDampingNum);
	m_damping.z = std::clamp(damping.y, 0.0f, kMaxDampingNum);
}

//void CameraBody::SetDampingYaw(const float damping_yaw)
//{
//	m_damping_yaw = std::clamp(damping_yaw, 0.0f, kMaxDampingNum);
//}
//
//void CameraBody::SetDampingPitch(const float damping_pitch)
//{
//	m_damping_pitch = std::clamp(damping_pitch, 0.0f, kMaxDampingNum);
//}
#pragma endregion
