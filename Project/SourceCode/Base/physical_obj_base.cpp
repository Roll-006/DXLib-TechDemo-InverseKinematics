#include "physical_obj_base.hpp"

PhysicalObjBase::PhysicalObjBase(const std::string& name, const std::string& tag) :
	ObjBase					(name, tag),
	mass_kind				(MassKind::kLight),
	m_project_pos			(std::nullopt),
	m_velocity				(v3d::GetZeroV()),
	m_move_velocity			(v3d::GetZeroV()),
	m_fall_velocity			(v3d::GetZeroV()),
	m_knockback_velocity	(v3d::GetZeroV()),
	m_knockback_speed		(0.0f),
	m_knockback_deceleration(0.0f),
	m_is_landing			(true),
	m_is_project			(true),
	m_model_handle			(-1)
{

}

void PhysicalObjBase::DrawColliders() const
{
	for (const auto& collider : m_colliders)
	{
		const auto shape = collider.second->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}
}

void PhysicalObjBase::OnKnockback(const VECTOR& dir, const float initial_velocity, const float deceleration)
{
	m_knockback_speed			= initial_velocity;
	m_knockback_deceleration	= deceleration;
	m_knockback_velocity		= dir * m_knockback_speed;

	// ノックバック時はvelocityの貼り付けを行わない
	m_is_project	= false;
}

void PhysicalObjBase::RemoveHitTriangles()
{
	for (const auto& collider : m_colliders)
	{
		collider.second->RemoveHitTriangles();
	}
}

void PhysicalObjBase::ApplyGravity(const float gravity_acceleration, const float max_gravity)
{
	// 地面にいる場合は重力を与えない
	if (m_is_landing)
	{
		m_fall_velocity.y = 0.0f;
	}
	else
	{
		math::Decrease(m_fall_velocity.y, gravity_acceleration * GetDeltaTime(), -max_gravity);
	}
}

void PhysicalObjBase::ApplyVelocity()
{
	m_transform->Move(CoordinateKind::kWorld, m_velocity);

	for (const auto& collider : m_colliders)
	{
		const auto shape = collider.second->GetShape();
		if (shape != nullptr)
		{
			shape->Move(m_velocity);
		}
	}

	m_velocity = v3d::GetZeroV();
}

void PhysicalObjBase::ProjectVelocity()
{
	if (!m_is_project) { return; }

	const auto landing_ray = GetCollider(ColliderKind::kLandingRay);
	if (!landing_ray) { return; }

	const auto hit_triangle = landing_ray->GetHitTriangles();
	if (hit_triangle.empty()) { return; }

	// 光線の始点からの距離でソート
	const auto segment = std::static_pointer_cast<Segment>(landing_ray->GetShape());
	std::vector<std::pair<int, float>> distance;
	for (size_t i = 0; i < hit_triangle.size(); ++i)
	{
		distance.emplace_back(i, math::GetDistancePointToTriangle(segment->GetBeginPos(), hit_triangle.at(i)));
	}
	algorithm::Sort(distance, SortKind::kAscending);

	// 斜面に投影
	const auto cross_x	= math::GetNormalVector(hit_triangle.at(distance.front().first).GetNormalVector(), axis::GetWorldYAxis());
	auto base_v			= math::GetNormalVector(hit_triangle.at(distance.front().first).GetNormalVector(), cross_x);
	base_v				= v3d::GetNormalizedV(VGet(m_velocity.x, base_v.y, m_velocity.z));
	m_velocity			= math::GetProjectionVector(m_velocity, base_v);
}

void PhysicalObjBase::ApplyKnockbackVelocity()
{
	math::Decrease(m_knockback_speed, m_knockback_deceleration * GetDeltaTime(), 0.0f);
	m_knockback_velocity = v3d::GetNormalizedV(m_knockback_velocity) * m_knockback_speed;
	m_velocity += m_knockback_velocity;
}

std::shared_ptr<Collider> PhysicalObjBase::GetCollider(const ColliderKind kind) const
{
	return m_colliders.count(kind) ? m_colliders.at(kind) : nullptr;
}

void PhysicalObjBase::AddCollider(const std::shared_ptr<Collider>& collider)
{
	if (!m_colliders.count(collider->GetColliderKind()))
	{
		m_colliders[collider->GetColliderKind()] = collider;
	}
}

void PhysicalObjBase::RemoveCollider(const ColliderKind collider_kind)
{
	m_colliders.erase(collider_kind);
}
