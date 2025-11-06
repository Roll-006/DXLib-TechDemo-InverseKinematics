#pragma once
#include <concepts>
#include "../Interface/i_scene.hpp"

namespace scene_concepts
{
	/// @brief ®”Œ^‚Å‚ ‚é
	template<typename T>
	concept SceneT = std::is_base_of_v<IScene, T>;
}
