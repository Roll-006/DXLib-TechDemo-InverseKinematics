#include "scene_manager.hpp"

SceneManager::SceneManager() : 
	m_share_scene	(std::make_shared<ShareScene>()),
	m_current_scene	(std::make_shared<PlayScene>()),
	m_drawer		(std::make_unique<Drawer>())
{

}

SceneManager::~SceneManager()
{
	// ˆ—‚È‚µ
}

void SceneManager::Update()
{
	InputChecker	::GetInstance()->Update();
	CommandHandler	::GetInstance()->Update();
	Debugger		::GetInstance()->Update();

	ChangeScene();

	m_share_scene	->Update();
	m_current_scene	->Update();

	PhysicsManager	::GetInstance()->Update();
}

void SceneManager::LateUpdate()
{
	PhysicsManager	::GetInstance()->LateUpdate();

	m_share_scene	->LateUpdate();
	m_current_scene	->LateUpdate();

	CollisionManager::GetInstance()->LateUpdate();
	InputChecker	::GetInstance()->LateUpdate();
}

void SceneManager::DrawToShadowMap() const
{
	m_drawer->DrawToShadowMap(m_current_scene);
}

void SceneManager::Draw() const
{
	m_drawer->Draw(m_current_scene);
	Debugger::GetInstance()->Draw();
}

void SceneManager::ChangeScene()
{
	const auto next_scene = m_current_scene->ChangeScene();
	if (next_scene)
	{
		m_current_scene = next_scene;
		m_current_scene->Init();
	}
}
