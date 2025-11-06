#pragma once
#include "../../Interface/i_state.hpp"
#include "../../Object/player.hpp"

namespace player_state
{
	class Run final : public IState<Player>
	{
	public:
		Run();
		~Run() override;

		void Update		(std::shared_ptr<Player>& player) override;
		void Enter		(std::shared_ptr<Player>& player) override;
		void Exit		(std::shared_ptr<Player>& player) override;

		[[nodiscard]] std::shared_ptr<IState<Player>> ChangeState(std::shared_ptr<Player>& player) override;

	private:

	};
}
