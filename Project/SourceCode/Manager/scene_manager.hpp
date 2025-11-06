#pragma once
#include "../Drawer/drawer.hpp"
#include "../Debugger/debugger.hpp"

#include "../Scene/share_scene.hpp"
#include "../Scene/play_scene.hpp"
#include "../Input/input_checker.hpp"
#include "../Command/command_handler.hpp"

class SceneManager final : public SingleInstanceBase<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	void Update();
	void LateUpdate();
	void DrawToShadowMap()	const;
	void Draw()				const;

private:
	void ChangeScene();

private:
	std::shared_ptr<IScene> m_share_scene;
	std::shared_ptr<IScene> m_current_scene;
	std::unique_ptr<Drawer> m_drawer;
};
