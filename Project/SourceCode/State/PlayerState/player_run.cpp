#include "player_run.hpp"
#include "player_state.hpp"

player_state::Run::Run()
{

}

player_state::Run::~Run()
{

}

void player_state::Run::Update(std::shared_ptr<Player>& player)
{
	player->Run();
}

void player_state::Run::Enter(std::shared_ptr<Player>& player)
{

}

void player_state::Run::Exit(std::shared_ptr<Player>& player)
{
	CommandHandler::GetInstance()->InitCurrentTriggerInputCount(CommandKind::kRun);
}

std::shared_ptr<IState<Player>> player_state::Run::ChangeState(std::shared_ptr<Player>& player)
{
	if (player->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state		= player->GetState();
	const auto command		= CommandHandler::GetInstance();
	const auto command_mode = command->GetInputModeKind(CommandKind::kRun);

	// Null
	if (!state->TryRun())
	{
		return state->GetState<player_state::ActionNull, Player>();
	}
	// Crouch
	if (command->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent))
	{
		// Runコマンドがホールド方式で、入力中であった場合は移行を許可しない
		if (!(command_mode == InputModeKind::kHold && state->TryRun()))
		{
			return state->GetState<Crouch, Player>();
		}
		command->InitCurrentTriggerInputCount(CommandKind::kCrouch);
	}

	return nullptr;
}
