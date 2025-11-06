#include "player_idle.hpp"
#include "player_state.hpp"

player_state::Idle::Idle()
{

}

player_state::Idle::~Idle()
{

}

void player_state::Idle::Update(std::shared_ptr<Player>& player)
{
	player->Idle();
}

void player_state::Idle::Enter(std::shared_ptr<Player>& player)
{

}

void player_state::Idle::Exit(std::shared_ptr<Player>& player)
{
	
}

std::shared_ptr<IState<Player>> player_state::Idle::ChangeState(std::shared_ptr<Player>& player)
{
	if (player->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state = player->GetState();

	// Move
	if (state->TryMove())
	{
		return state->GetState<player_state::Move, Player>();
	}

	return nullptr;
}
