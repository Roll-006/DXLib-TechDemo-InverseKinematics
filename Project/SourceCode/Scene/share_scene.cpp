#include "share_scene.hpp"
#include "../Manager/scene_manager.hpp"

ShareScene::ShareScene() : 
	m_is_active	 (true),
	m_scene_kind (SceneKind::kShare),
	m_main_camera(std::make_shared<MainCamera>())
{
	m_main_camera->AddToObjManager();
}

ShareScene::~ShareScene()
{

}

void ShareScene::Init()
{

}

void ShareScene::Update()
{
	CinemachineBrain::GetInstance()	->Update();
}

void ShareScene::LateUpdate()
{
	CinemachineBrain::GetInstance()	->LateUpdate();

}

void ShareScene::DrawToShadowMap() const
{
	
}

void ShareScene::Draw() const
{

}

std::shared_ptr<IScene> ShareScene::ChangeScene()
{
	return nullptr;
}
