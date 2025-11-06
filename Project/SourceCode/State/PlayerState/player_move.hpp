#pragma once
#include "../../Interface/i_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class Move final : public IState<Player>
	{
	public:
		Move();
		~Move() override;

		void Update		(std::shared_ptr<Player>& player) override;
		void Enter		(std::shared_ptr<Player>& player) override;
		void Exit		(std::shared_ptr<Player>& player) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player>& player) override;

	private:
		bool m_is_first_move_frame;		// 移動を開始して最初のフレームであるかを判定
	};
}
