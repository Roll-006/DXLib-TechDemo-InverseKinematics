#include "player_crouch.hpp"
#include "player_state.hpp"
#include "../../VirtualCamera/cinemachine_brain.hpp"

player_state::Crouch::Crouch()
{

}

player_state::Crouch::~Crouch()
{

}

void player_state::Crouch::Update(std::shared_ptr<Player>& player)
{
	player->Crouch();
}

void player_state::Crouch::Enter(std::shared_ptr<Player>& player)
{
	const auto cinemachine_brain	= CinemachineBrain::GetInstance();
	const auto crouch_camera		= cinemachine_brain->GetVirtualCamera(ObjName.ROT_CROUCH_CAMERA);
	if (crouch_camera)
	{
		crouch_camera->Activate();
	}
}

void player_state::Crouch::Exit(std::shared_ptr<Player>& player)
{
	CommandHandler::GetInstance()->InitCurrentTriggerInputCount(CommandKind::kCrouch);

	const auto cinemachine_brain	= CinemachineBrain::GetInstance();
	const auto crouch_camera		= cinemachine_brain->GetVirtualCamera(ObjName.ROT_CROUCH_CAMERA);
	if (crouch_camera)
	{
		crouch_camera->Deactivate();
	}
}

std::shared_ptr<IState<Player>> player_state::Crouch::ChangeState(std::shared_ptr<Player>& player)
{
	if (player->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state		= player->GetState();
	const auto command		= CommandHandler::GetInstance();
	const auto command_mode = command->GetInstance()->GetInputModeKind(CommandKind::kCrouch);

	// Null
	if (!command->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent))
	{
		return state->GetState<ActionNull, Player>();
	}
	// Run
	if (state->TryRun())
	{
		// Crouchコマンドがホールド方式で、入力中であった場合は移行を許可しない
		if (!(command_mode == InputModeKind::kHold && command->IsExecute(CommandKind::kCrouch, TimeKind::kCurrent)))
		{
			return state->GetState<Run, Player>();
		}
		command->InitCurrentTriggerInputCount(CommandKind::kRun);
	}

	return nullptr;
}
