#pragma once
#include "../Interface/i_scene.hpp"

#include "../Object/player.hpp"
#include "../Object/stage.hpp"

#include "../Light/light_holder.hpp"
#include "../VirtualCamera/cinemachine_brain.hpp"

class PlayScene final : public IScene
{
public:
	PlayScene();
	~PlayScene() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void Activate()		override { m_is_active = true; }
	void Deactivate()	override { m_is_active = false; }

	[[nodiscard]] bool						IsActive()		const	override { return m_is_active; }
	[[nodiscard]] std::shared_ptr<IScene>	ChangeScene()			override;
	[[nodiscard]] virtual SceneKind			GetSceneKind()	const	override { return m_scene_kind; }

private:
	bool		m_is_active;
	SceneKind	m_scene_kind;
	float		m_elapsed_time;

	std::shared_ptr<Player>	 m_player;
	std::shared_ptr<Stage>	 m_stage;
};
