#include "player_action_null.hpp"
#include "player_state.hpp"

player_state::ActionNull::ActionNull()
{

}

player_state::ActionNull::~ActionNull()
{

}

void player_state::ActionNull::Update(std::shared_ptr<Player>& player)
{
	
}

void player_state::ActionNull::Enter(std::shared_ptr<Player>& player)
{

}

void player_state::ActionNull::Exit(std::shared_ptr<Player>& player)
{
	
}

std::shared_ptr<IState<Player>> player_state::ActionNull::ChangeState(std::shared_ptr<Player>& player)
{
	if (player->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state	= player->GetState();
	const auto command	= CommandHandler::GetInstance();

	// Run
	if (state->TryRun())
	{
		return state->GetState<Run, Player>();
	}
	// Crouch
	if (command->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent))
	{
		return state->GetState<Crouch, Player>();
	}

	return nullptr;
}
