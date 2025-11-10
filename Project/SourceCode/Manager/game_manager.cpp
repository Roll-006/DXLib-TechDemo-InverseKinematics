#include "game_manager.hpp"

GameManager::GameManager():
	m_game_system_setter(std::make_unique<GameSystemSetter>()),
	m_window			(std::make_unique<Window>()),
	m_scene_manager		(nullptr)
{
	HandleCreator			::Generate();
	HandleKeeper			::Generate();
	CommandHandler			::Generate();
	InputChecker			::Generate();
	Debugger				::Generate();
	EventSystem				::Generate();
	FontHandler				::Generate();
	LightHolder				::Generate();

	GameTimeManager			::Generate();
	ObjManager				::Generate();
	CollisionManager		::Generate();
	PhysicsManager			::Generate();
	CinemachineBrain		::Generate();

	m_scene_manager = std::make_unique<SceneManager>();
}

GameManager::~GameManager()
{	
	m_scene_manager = nullptr;

	HandleCreator			::Delete();
	HandleKeeper			::Delete();
	CommandHandler			::Delete();
	InputChecker			::Delete();
	Debugger				::Delete();

	GameTimeManager			::Delete();
	CollisionManager		::Delete();
	PhysicsManager			::Delete();

	CinemachineBrain		::Delete();
	LightHolder				::Delete();
	ObjManager				::Delete();
	EventSystem				::Delete();
}

void GameManager::Run()
{
	while (IsContinueLoop())
	{
		GameTimeManager::GetInstance()->Update();
		m_window->Update();

		m_scene_manager->Update();
		m_scene_manager->LateUpdate();
		m_scene_manager->DrawToShadowMap();
		m_scene_manager->Draw();

		GameTimeManager::GetInstance()->Draw();
		GameTimeManager::GetInstance()->WaitTime();
		ScreenFlip();
	}
}

bool GameManager::IsContinueLoop()
{
	if (ProcessMessage()  != 0) { return false; }
	if (ClearDrawScreen() != 0) { return false; }

	return true;
}
