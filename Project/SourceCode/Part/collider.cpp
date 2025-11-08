#include "collider.hpp"
#include "../Base/physical_obj_base.hpp"

Collider::Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase>& shape, PhysicalObjBase* owner_obj) :
	m_kind					(kind),
	m_shape					(shape),
	m_model_handle			(-1),
	m_is_closest_only_hit	(false),
	m_is_one_collision		(kind == ColliderKind::kAttackTrigger ? true : false),
	m_owner_obj				(owner_obj)
{
	m_is_closest_only_hit = kind == ColliderKind::kRay
						 || kind == ColliderKind::kLandingRay
						 || kind == ColliderKind::kProjectRay ? true : false;
	
	JudgeValidShape();
}

Collider::Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase>& shape, const bool is_closest_only_hit, PhysicalObjBase* owner_obj) :
	m_kind					(kind),
	m_shape					(shape),
	m_model_handle			(-1),
	m_is_closest_only_hit	(false),
	m_is_one_collision		(kind == ColliderKind::kAttackTrigger ? true : false),
	m_owner_obj				(owner_obj)
{
	m_is_closest_only_hit = kind == ColliderKind::kRay
						 || kind == ColliderKind::kLandingRay
						 || kind == ColliderKind::kProjectRay ? is_closest_only_hit : false;
	
	JudgeValidShape();
}

Collider::Collider(const ColliderKind kind, const int model_handle, PhysicalObjBase* owner_obj) :
	m_kind					(kind),
	m_shape					(nullptr),
	m_model_handle			(model_handle),
	m_is_closest_only_hit	(false),
	m_is_one_collision		(kind == ColliderKind::kAttackTrigger ? true : false),
	m_owner_obj				(owner_obj)
{
	JudgeValidShape();
}

Collider::~Collider()
{

}


void Collider::EnableAllRayCastHit()
{
	if (m_kind == ColliderKind::kRay || m_kind == ColliderKind::kLandingRay || m_kind == ColliderKind::kProjectRay)
	{
		m_is_closest_only_hit = false;
	}
}

void Collider::EnableClosestOnlyRayCastHit()
{
	if (m_kind == ColliderKind::kRay || m_kind == ColliderKind::kLandingRay || m_kind == ColliderKind::kProjectRay)
	{
		m_is_closest_only_hit = true;
	}
}


#pragma region 登録 / 解除
void Collider::AddHitTriangle(const Triangle& hit_triangle)
{
	m_hit_triangles.emplace_back(hit_triangle);
}

void Collider::AddHitTriangles(const std::vector<Triangle>& hit_triangles)
{
	m_hit_triangles.insert(m_hit_triangles.end(), hit_triangles.begin(), hit_triangles.end());
}

void Collider::RemoveHitTriangles()
{
	m_hit_triangles.clear();
}
#pragma endregion


void Collider::JudgeValidShape()
{
	switch (m_kind)
	{
	case ColliderKind::kRay:
	case ColliderKind::kLandingRay:
	case ColliderKind::kProjectRay:
		assert(m_shape->GetShapeKind() == ShapeKind::kSegment);
		break;

	default:
		break;
	}
}
