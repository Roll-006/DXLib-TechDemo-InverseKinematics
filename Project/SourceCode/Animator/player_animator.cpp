#include "../State/PlayerState/player_state.hpp"
#include "player_animator.hpp"

PlayerAnimator::PlayerAnimator(const std::shared_ptr<Modeler>& modeler, const std::shared_ptr<PlayerState>& state) :
	AnimatorBase	(modeler, ObjName.PLAYER),
	m_state			(state)
{
	LoadAnim();
}

PlayerAnimator::~PlayerAnimator()
{

}

void PlayerAnimator::Init()
{

}

void PlayerAnimator::Update()
{
	DivideBone();
	ChangeAnim();

	BlendAnim();
	PlayAnim();
}

void PlayerAnimator::LoadAnim()
{
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle),	 AnimPath.IDLE,   0, AnimTag.NONE, 50.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalk),	 AnimPath.WALK,   0, AnimTag.MOVE, 50.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kRun),	 AnimPath.RUN,    0, AnimTag.NONE, 60.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kCrouch), AnimPath.CROUCH, 0, AnimTag.NONE, 50.0f, true, true);
}

void PlayerAnimator::ChangeAnim()
{
	const auto type = m_state->GetCurrentMoveStateType();

	// Idle
	if (type == typeid(player_state::Idle))
	{
		CombineMoveNullWithAction();
	}
	// Move
	else if (type == typeid(player_state::Move))
	{
		CombineMoveWithAction();
	}
}


#pragma region 状態の合成
void PlayerAnimator::CombineMoveNullWithAction()
{
	const auto type = m_state->GetCurrentActionStateType();

	// Idle + Null (Idle)
	if (type == typeid(player_state::ActionNull))
	{
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kIdle));
	}
	// Idle + Crouch (Crouch)
	else if (type == typeid(player_state::Crouch))
	{
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kCrouch));
	}
}

void PlayerAnimator::CombineMoveWithAction()
{
	const auto type = m_state->GetCurrentActionStateType();

	// Move + Null (Walk)
	if (type == typeid(player_state::ActionNull))
	{
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kWalk));
	}
	// Move + Run (Run)
	else if (type == typeid(player_state::Run))
	{
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kRun));
	}
}
