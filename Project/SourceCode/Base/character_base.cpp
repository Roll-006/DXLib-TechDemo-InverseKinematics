#include "character_base.hpp"

#pragma region ïêäÌ
CharacterBase::CharacterBase(const std::string& name, const std::string& tag) : 
	PhysicalObjBase			(name, tag),
	invincible_time			(0.0f),
	m_modeler				(nullptr),
	m_animator				(nullptr),
	m_collider_creator		(std::make_shared<CharacterColliderCreator>()),
	m_move_dir				{ {TimeKind::kCurrent, v3d::GetZeroV()},{TimeKind::kNext, v3d::GetZeroV()} },
	m_look_dir				{ {TimeKind::kCurrent, v3d::GetZeroV()},{TimeKind::kNext, v3d::GetZeroV()} },
	m_destination		(v3d::GetZeroV()),
	m_move_speed			(0.0f),
	m_move_dir_offset_speed	(0.0f),
	m_look_dir_offset_speed	(0.0f),
	m_is_calc_look_dir		(false)
{
		
}

void CharacterBase::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(physical_obj);
}

void CharacterBase::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj (obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj(obj_handle);
	ObjManager		::GetInstance()->RemoveObj		  (obj_handle);
}

void CharacterBase::CalcCorrectMoveDir()
{
	const auto current_pos			= m_transform->GetPos(CoordinateKind::kWorld);
	const auto current_pos_y0		= VGet(current_pos.x, 0.0f, current_pos.z);
	const auto destination_y0	= VGet(m_destination.x, 0.0f, m_destination.z);

	if (VSize(destination_y0 - current_pos_y0) < 1.0f)
	{
		m_move_dir.at(TimeKind::kNext) = v3d::GetZeroV();
	}
	else
	{
		m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(destination_y0 - current_pos_y0);
	}
}

void CharacterBase::ApplyLookDirToRot(const VECTOR& look_dir)
{
	m_transform->SetRot  (CoordinateKind::kWorld, look_dir);
}

void CharacterBase::CalcMoveDir()
{
	// åªç›ÇÃdirÇñ⁄ìIÇ∆Ç∑ÇÈdirÇ…ãﬂÇ√ÇØÇƒÇ¢Ç≠
	m_move_dir.at(TimeKind::kCurrent) = math::GetApproachedVector(
		m_move_dir.at(TimeKind::kCurrent),
		m_move_dir.at(TimeKind::kNext),
		m_move_dir_offset_speed);
}

void CharacterBase::CalcLookDir()
{
	if (!m_is_calc_look_dir) { return; }

	// ÉàÅ[äpâÒì]ÇéÊìæÇµÅA-ÉŒÅ`ÉŒÇ≈ílÇä«óùÇ∑ÇÈ
	const auto current_yaw	= math::GetYawRotVector(m_look_dir.at(TimeKind::kCurrent));
	const auto next_yaw		= math::GetYawRotVector(m_look_dir.at(TimeKind::kNext));
	auto	   distance		= next_yaw - current_yaw;
	distance.y = math::ConnectMinusValueToValue(distance.y, DX_PI_F);

	// ÉJÉÅÉâÇäÓèÄÇ…ÇµÇƒâEë§Ç≈Ç†Ç¡ÇΩèÍçáÇÕîΩì]
	if (distance.y > 0) { m_look_dir_offset_speed *= -1; }

	// âÒì]ÇìKóp
	const auto look_dir_offset_speed = -m_look_dir_offset_speed * GetDeltaTime();
	const auto rot_q = quat::CreateQuaternion(axis::GetWorldYAxis(), look_dir_offset_speed);
	m_look_dir.at(TimeKind::kCurrent) = math::GetRotatedPos(m_look_dir.at(TimeKind::kCurrent), rot_q);

	// èIóπîªíË
	const auto angle = math::GetYawBetweenTwoVector(m_look_dir.at(TimeKind::kNext), m_look_dir.at(TimeKind::kCurrent));
	const auto dynamic_threshold = std::abs(look_dir_offset_speed * math::kStopThreshold);
	if (angle < dynamic_threshold)
	{
		m_look_dir.at(TimeKind::kCurrent) = m_look_dir.at(TimeKind::kNext);
	}
}

void CharacterBase::CalcMoveVelocity()
{
	m_move_velocity = m_move_dir.at(TimeKind::kCurrent) * m_move_speed;
	m_velocity += m_move_velocity;
}

