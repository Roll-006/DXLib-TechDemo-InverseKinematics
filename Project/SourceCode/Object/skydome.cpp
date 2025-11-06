#include "skydome.hpp"

Skydome::Skydome() :
	ObjBase		(ObjName.SKYDOME, ObjTag.SKYDOME),
	m_modeler	(std::make_shared<Modeler>(m_transform, ModelPath.SKYDOME, kBasicScale))
{

}

Skydome::~Skydome()
{

}

void Skydome::Init()
{

}

void Skydome::Update()
{
	if (!IsActive()) { return; }
}

void Skydome::LateUpdate()
{
	if (!IsActive()) { return; }

	m_transform->SetPos(CoordinateKind::kWorld, GetCameraPosition());
}

void Skydome::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void Skydome::AddToObjManager()
{

}

void Skydome::RemoveToObjManager()
{

}

float Skydome::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
