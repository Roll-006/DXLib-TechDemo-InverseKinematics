#include "collider.hpp"
#include "../Base/physical_obj_base.hpp"

Collider::Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase>& shape, PhysicalObjBase* owner_obj) :
	m_kind					(kind),
	m_shape					(shape),
	m_model_handle			(-1),
	m_is_closest_only_hit	(kind == ColliderKind::kRay || kind == ColliderKind::kProjectRay ? true : false),
	m_is_one_collision		(kind == ColliderKind::kAttackTrigger ? true : false),
	m_owner_obj				(owner_obj)
{
	JudgeValidShape();
}

Collider::Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase>& shape, const bool is_closest_only_hit, PhysicalObjBase* owner_obj) :
	m_kind					(kind),
	m_shape					(shape),
	m_model_handle			(-1),
	m_is_closest_only_hit	(kind == ColliderKind::kRay || kind == ColliderKind::kProjectRay ? is_closest_only_hit : false),
	m_is_one_collision		(kind == ColliderKind::kAttackTrigger ? true : false),
	m_owner_obj				(owner_obj)
{
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
	if (m_kind == ColliderKind::kRay || m_kind == ColliderKind::kProjectRay)
	{
		m_is_closest_only_hit = false;
	}
}

void Collider::EnableClosestOnlyRayCastHit()
{
	if (m_kind == ColliderKind::kRay || m_kind == ColliderKind::kProjectRay)
	{
		m_is_closest_only_hit = true;
	}
}

void Collider::JudgeValidShape()
{
	switch (m_kind)
	{
	case ColliderKind::kRay:
	case ColliderKind::kProjectRay:
		assert(m_shape->GetShapeKind() == ShapeKind::kSegment);
		break;

	default:
		break;
	}
}
