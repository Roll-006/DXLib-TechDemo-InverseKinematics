#include "physical_obj_base.hpp"

PhysicalObjBase::PhysicalObjBase(const std::string& name, const std::string& tag) :
	ObjBase							(name, tag),
	mass_kind						(MassKind::kLight),
	m_project_pos					(std::nullopt),
	m_velocity						(v3d::GetZeroV()),
	m_move_velocity					(v3d::GetZeroV()),
	m_fall_velocity					(v3d::GetZeroV()),
	m_knockback_velocity			(v3d::GetZeroV()),
	m_knockback_speed				(0.0f),
	m_knockback_deceleration		(0.0f),
	m_is_landing					(true),
	m_is_using_projection_velocity	(true),
	m_model_handle					(-1)
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
	m_is_using_projection_velocity	= false;
}

void PhysicalObjBase::RemoveHitTriangles()
{
	for (const auto& collider : m_colliders)
	{
		collider.second->RemoveHitTriangle();
		collider.second->RemoveHitModelTriangle();
	}
}

void PhysicalObjBase::RemoveHitCollider()
{
	for (auto itr = m_hit_colliders.begin(); itr != m_hit_colliders.end(); )
	{
		if (!itr->second)
		{
			itr = m_hit_colliders.erase(itr);
		}
		else
		{
			++itr;
		}
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

void PhysicalObjBase::ProjectionVelocity()
{
	if (!m_is_using_projection_velocity) { return; }

	const auto landing_trigger = GetCollider(ColliderKind::kLandingTrigger);

	if (!landing_trigger) { return; }
	if (landing_trigger->GetHitTriangles().empty() && landing_trigger->GetHitModelTriangles().empty()) { return; }

	std::vector<Triangle> all_triangles;
	size_t all_size = 0;

	// リサイズ
	if (!landing_trigger->GetHitTriangles().empty())
	{
		all_size = landing_trigger->GetHitTriangles().size();
	}
	if (!landing_trigger->GetHitModelTriangles().empty())
	{
		for (const auto& [handle, triangle] : landing_trigger->GetHitModelTriangles())
		{
			all_size += triangle.size();
		}
	}
	all_triangles.reserve(all_size);

	// 合成
	if (!landing_trigger->GetHitTriangles().empty())
	{
		all_triangles.insert(all_triangles.end(), landing_trigger->GetHitTriangles().begin(), landing_trigger->GetHitTriangles().end());
	}
	if (!landing_trigger->GetHitModelTriangles().empty())
	{
		for (const auto& [handle, triangle] : landing_trigger->GetHitModelTriangles())
		{
			all_triangles.insert(all_triangles.end(), triangle.begin(), triangle.end());
		}
	}

	// ヒットしたポリゴンから三角形を生成
	// 三角形との距離を取得
	std::unordered_map<int, Triangle>	triangles;
	std::vector<std::pair<int, float>>	distance;
	for (size_t i = 0; i < all_triangles.size(); ++i)
	{
		triangles[static_cast<int>(i)] = all_triangles.at(i);

		const auto dist = math::GetDistanceTriangleToSphere(all_triangles.at(i), *std::dynamic_pointer_cast<Sphere>(landing_trigger->GetShape()));
		distance.emplace_back(std::make_pair(i, dist));
	}

	// 距離が最も近い三角形との交点を取得
	distance = algorithm::Sort(distance, SortKind::kAscending);
	for (const auto& dist : distance)
	{
		// 斜面に投影
		const auto cross_x	= math::GetNormalVector(triangles.at(dist.first).GetNormalVector(), axis::GetWorldYAxis());
		auto base_v			= math::GetNormalVector(triangles.at(dist.first).GetNormalVector(), cross_x);
		base_v				= v3d::GetNormalizedV(VGet(m_velocity.x, base_v.y, m_velocity.z));
		m_velocity			= math::GetProjectionVector(m_velocity, base_v);

		return;
	}
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
