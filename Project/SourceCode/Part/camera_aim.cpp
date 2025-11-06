#include "camera_aim.hpp"

CameraAim::CameraAim(const std::shared_ptr<Transform>& owner_transform) :
	m_owner_transform		(owner_transform),
	m_target_transform		(nullptr),
	m_rot_matrix			(MGetIdent()),
	m_destination_aim_pos	(v3d::GetZeroV()),
	m_current_aim_pos		(v3d::GetZeroV()),
	m_tracked_obj_offset	(v3d::GetZeroV()),
	m_horizontal_damping	(0.0f),
	m_vertical_damping		(0.0f),
	m_screen				(0.5f, 0.5f),
	m_dead_zone				(v2d::GetZeroV<Vector2D<float>>()),
	m_soft_zone				(v2d::GetZeroV<Vector2D<float>>()),
	m_bias					(v2d::GetZeroV<Vector2D<float>>()),
	m_is_track				(false)
{

}

CameraAim::~CameraAim()
{

}

void CameraAim::CalcAimPos()
{
	m_destination_aim_pos = m_target_transform->GetPos(CoordinateKind::kWorld);

	// カメラの軸をもとに位置を修正
	const auto target_axes = m_target_transform->GetAxes(CoordinateKind::kWorld);
	m_destination_aim_pos += target_axes.x_axis * m_tracked_obj_offset.x;
	m_destination_aim_pos += target_axes.y_axis * m_tracked_obj_offset.y;
	m_destination_aim_pos += target_axes.z_axis * m_tracked_obj_offset.z;

	//DrawSphere3D(m_destination_aim_pos, 3, 8, 0xffffff, 0xffffff, TRUE);
}

void CameraAim::CalcDampedAimPos()
{
	const auto time_manager = GameTimeManager::GetInstance();
	const auto   owner_axes	= m_owner_transform->GetAxes(CoordinateKind::kWorld);
	const VECTOR damping	= { m_horizontal_damping, m_vertical_damping, 0.0f };

	m_current_aim_pos = math::GetDampedValueOnAxes(m_current_aim_pos, m_destination_aim_pos, damping, 
		owner_axes, time_manager->GetDeltaTime(TimeScaleLayerKind::kCamera));

	//DrawSphere3D(m_current_aim_pos, 3, 8, 0xffffff, 0xffffff, TRUE);
}

void CameraAim::CalcRot()
{
	CalcAimPos();

	// 目的とする回転行列を取得
	// Z軸回転は無効化する
	const auto forward = v3d::GetNormalizedV(m_destination_aim_pos - m_owner_transform->GetPos(CoordinateKind::kWorld));

	m_owner_transform->SetRot(CoordinateKind::kWorld, forward);
}

void CameraAim::CalcDampedRot()
{
	CalcDampedAimPos();

	// 目的とする回転行列を取得
	// Z軸回転は無効化する
	const auto forward = v3d::GetNormalizedV(m_current_aim_pos - m_owner_transform->GetPos(CoordinateKind::kWorld));

	m_owner_transform->SetRot(CoordinateKind::kWorld, forward);
}


#pragma region Attach / Detach
void CameraAim::AttachTarget(const std::shared_ptr<Transform>& target_transform)
{
	m_target_transform		= target_transform;
	m_is_track				= true;
}

void CameraAim::DetachTarget()
{
	m_target_transform	= nullptr;
	m_is_track			= false;
}
#pragma endregion


#pragma region Setter
void CameraAim::SetRot				(const MATRIX& rot_matrix)
{
	m_owner_transform->SetRot(CoordinateKind::kWorld, rot_matrix);
}

void CameraAim::SetHorizontalDamping(const float horizontal_damping)
{
	m_horizontal_damping = std::clamp(horizontal_damping, 0.0f, kMaxDampingNum);
}

void CameraAim::SetVerticalDamping	(const float vertical_damping)
{
	m_vertical_damping = std::clamp(vertical_damping, 0.0f, kMaxDampingNum);
}

void CameraAim::SetScreen			(const Vector2D<float>& screen)
{
	m_screen.x		= std::clamp(screen.x, 0.0f, 1.0f);
	m_screen.y		= std::clamp(screen.y, 0.0f, 1.0f);
}

void CameraAim::SetDeadZone			(const Vector2D<float>& dead_zone)
{
	m_dead_zone.x	= std::clamp(dead_zone.x, 0.0f, 1.0f);
	m_dead_zone.y	= std::clamp(dead_zone.y, 0.0f, 1.0f);
}

void CameraAim::SetSoftZone			(const Vector2D<float>& soft_zone)
{
	m_soft_zone.x	= std::clamp(soft_zone.x, 0.0f, 1.0f);
	m_soft_zone.y	= std::clamp(soft_zone.y, 0.0f, 1.0f);
}

void CameraAim::SetBias				(const Vector2D<float>& bias)
{
	m_bias.x		= std::clamp(bias.x, -0.5f, 0.5f);
	m_bias.y		= std::clamp(bias.y, -0.5f, 0.5f);
}
#pragma endregion
