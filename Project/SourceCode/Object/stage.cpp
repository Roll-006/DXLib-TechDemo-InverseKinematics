#include "stage.hpp"

Stage::Stage() : 
	PhysicalObjBase	(ObjName.STAGE, ObjTag.STAGE),
	m_modeler		(std::make_shared<Modeler>(m_transform, ModelPath.STAGE, kBasicScale))
{	
	mass_kind = MassKind::kStatic;

	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_transform->SetPos(CoordinateKind::kWorld, kPos);
	m_modeler->ApplyMatrix();

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_modeler->GetModelHandle(), this));
}

Stage::~Stage()
{

}

void Stage::Init()
{

}

void Stage::Update()
{
	if (!IsActive()) { return; }
}

void Stage::LateUpdate()
{
	if (!IsActive()) { return; }
}

void Stage::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void Stage::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void Stage::AddToObjManager()
{
	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
	PhysicsManager	::GetInstance()->AddPhysicalObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
}

void Stage::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj (obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj(obj_handle);
	ObjManager		::GetInstance()->RemoveObj		  (obj_handle);
}

float Stage::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
