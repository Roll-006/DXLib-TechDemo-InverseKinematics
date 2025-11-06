#pragma once
#include "../Interface/i_scene.hpp"

#include "../Object/main_camera.hpp"
#include "../Command/command_handler.hpp"
#include "../VirtualCamera/cinemachine_brain.hpp"

class ShareScene final : public IScene
{
public:
	ShareScene();
	~ShareScene() override;

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

	std::shared_ptr<MainCamera>	m_main_camera;
};