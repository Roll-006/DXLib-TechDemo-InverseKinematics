#include "player_state.hpp"

PlayerState::PlayerState()
{
	m_states[typeid(player_state::Idle)]		= std::make_shared<player_state::Idle>();
	m_states[typeid(player_state::Move)]		= std::make_shared<player_state::Move>();
	m_states[typeid(player_state::ActionNull)]	= std::make_shared<player_state::ActionNull>();
	m_states[typeid(player_state::Run)]			= std::make_shared<player_state::Run>();
	m_states[typeid(player_state::Crouch)]		= std::make_shared<player_state::Crouch>();

	m_move_state  [TimeKind::kCurrent] = GetState<player_state::Idle,		Player>();
	m_action_state[TimeKind::kCurrent] = GetState<player_state::ActionNull, Player>();
}

PlayerState::~PlayerState()
{

}

void PlayerState::Init(std::shared_ptr<Player> player)
{
	m_move_state  .at(TimeKind::kCurrent)->Exit(player);
	m_action_state.at(TimeKind::kCurrent)->Exit(player);
}

void PlayerState::Update(std::shared_ptr<Player> player)
{
	ChangeState(player);

	m_move_state  .at(TimeKind::kCurrent)->Update(player);
	m_action_state.at(TimeKind::kCurrent)->Update(player);
}


#pragma region Try判定
bool PlayerState::TryMove()
{
	if(GetCurrentActionStateType() == typeid(player_state::Crouch)) { return false; }

	const auto command = CommandHandler::GetInstance();
	return((command->IsExecute(CommandKind::kMoveUpPlayer,    TimeKind::kCurrent)
		 || command->IsExecute(CommandKind::kMoveDownPlayer,  TimeKind::kCurrent)
		 || command->IsExecute(CommandKind::kMoveLeftPlayer,  TimeKind::kCurrent)
		 || command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kCurrent)));
}

bool PlayerState::TryRun()
{
	const auto command				= CommandHandler::GetInstance();
	const auto input				= InputChecker::GetInstance();
	const auto move_state_type		= GetCurrentMoveStateType();
	const auto action_state_type	= GetCurrentActionStateType();

	if (move_state_type == typeid(player_state::Move) || action_state_type == typeid(player_state::Crouch))
	{
		if (command->IsExecute(CommandKind::kRun, TimeKind::kCurrent))
		{
			return true;
		}
	}
	// IDLEであった場合、次フレームのためにトリガーカウントを増やしておく
	else
	{
		// 現在の入力方式を保存する
		const bool is_trigger = command->GetInputModeKind(CommandKind::kRun) == InputModeKind::kTrigger ? true : false;

		// 入力モードを強制的にホールド方式に変更
		command->SetInputMode(CommandKind::kRun, InputModeKind::kHold);

		// もともとトリガー方式であれば元に戻す
		if (is_trigger) { command->SetInputMode(CommandKind::kRun, InputModeKind::kTrigger); }
	}

	return false;
}
#pragma endregion


void PlayerState::ChangeState(std::shared_ptr<Player>& player)
{
	// 移動ステート
	const auto next_move_state = m_move_state.at(TimeKind::kCurrent)->ChangeState(player);
	if (next_move_state)
	{
		m_move_state[TimeKind::kPrev]		= m_move_state.at(TimeKind::kCurrent);
		m_move_state.at(TimeKind::kCurrent) = next_move_state;

		m_move_state.at(TimeKind::kPrev)	->Exit (player);
		m_move_state.at(TimeKind::kCurrent)	->Enter(player);
	}

	// 行動ステート
	const auto next_action_state = m_action_state.at(TimeKind::kCurrent)->ChangeState(player);
	if (next_action_state)
	{
		m_action_state[TimeKind::kPrev]			= m_action_state.at(TimeKind::kCurrent);
		m_action_state.at(TimeKind::kCurrent)	= next_action_state;

		m_action_state.at(TimeKind::kPrev)		->Exit(player);
		m_action_state.at(TimeKind::kCurrent)	->Enter(player);
	}
}
