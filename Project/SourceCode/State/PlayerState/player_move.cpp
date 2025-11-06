#include "player_move.hpp"
#include "player_state.hpp"

player_state::Move::Move() :
	m_is_first_move_frame(false)
{

}

player_state::Move::~Move()
{

}

void player_state::Move::Update(std::shared_ptr<Player>& player)
{
	player->Move();

	m_is_first_move_frame = false;
}

void player_state::Move::Enter(std::shared_ptr<Player>& player)
{
	m_is_first_move_frame = true;
}

void player_state::Move::Exit(std::shared_ptr<Player>& player)
{
	
}

std::shared_ptr<IState<Player>> player_state::Move::ChangeState(std::shared_ptr<Player>& player)
{
	if (player->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state = player->GetState();

	// Idle
	if (!state->TryMove())
	{
		return state->GetState<player_state::Idle, Player>();
	}

	return nullptr;
}
