#include "play_scene.hpp"
#include "../Manager/scene_manager.hpp"

PlayScene::PlayScene() :
	m_is_active		(true),
	m_scene_kind	(SceneKind::kPlay),
	m_elapsed_time	(0.0f),
	m_player		(std::make_shared<Player>()),
	m_stage			(std::make_shared<Stage>())
{
	m_player->AddToObjManager();
	m_stage->AddToObjManager();

	// カメラ設定
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->AddVirtualCameraController(std::make_shared<ControlVirtualCamerasController>(*m_player.get()));

	// ライトの設定
	const auto light_holder = LightHolder::GetInstance();
	light_holder->CreateLight(std::make_shared<DirectionalLight>(LightName.MOONLIGHT, 0, v3d::GetNormalizedV(VGet(0.0f, -1.0f, -0.5f))));

	const auto game_time_manager = GameTimeManager::GetInstance();
	game_time_manager->InitTimeScale();
}

PlayScene::~PlayScene()
{
	m_player->Init();

	m_player->RemoveToObjManager();
	m_stage ->RemoveToObjManager();

	// ライトの削除
	const auto light_holder = LightHolder::GetInstance();
	light_holder->DeleteLight(LightName.MOONLIGHT);
}

void PlayScene::Init()
{
	const auto game_time_manager = GameTimeManager::GetInstance();
	game_time_manager->InitTimeScale();
}

void PlayScene::Update()
{
	m_player	->Update();
	m_stage		->Update();
}

void PlayScene::LateUpdate()
{
	m_player	->LateUpdate();
	m_stage		->LateUpdate();
}

void PlayScene::DrawToShadowMap() const
{
	m_player	->Draw();
	m_stage		->Draw();
}

void PlayScene::Draw() const
{
	m_player	->Draw();
	m_stage		->Draw();
}

std::shared_ptr<IScene> PlayScene::ChangeScene()
{
	return nullptr;
}
