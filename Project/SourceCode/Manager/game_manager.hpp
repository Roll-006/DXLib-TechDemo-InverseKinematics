#pragma once
#include "../GameSystem/game_system_setter.hpp"
#include "../GameTime/game_time_manager.hpp"
#include "../Window/window.hpp"
#include "../Debugger/debugger.hpp"
#include "../Font/font_handler.hpp"

#include "scene_manager.hpp"
#include "obj_manager.hpp"

class GameManager final : public SingleInstanceBase<GameManager>
{
public:
	GameManager();
	~GameManager();

	void Run();

private:
	/// @brief ƒQ[ƒ€ƒ‹[ƒv‚ğŒp‘±‚·‚é‚©‚ğ”»’è
	[[nodiscard]] bool IsContinueLoop();

private:
	std::unique_ptr<GameSystemSetter>	m_game_system_setter;
	std::unique_ptr<Window>				m_window;
	std::unique_ptr<SceneManager>		m_scene_manager;
};
