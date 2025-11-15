#include <JSON/json_loader.hpp>
#include <MixamoHelper/mixamo_helper.hpp>

#include "player.hpp"
#include "../Command/command_handler.hpp"
#include "../State/PlayerState/player_state.hpp"

Player::Player() :
	CharacterBase(ObjName.PLAYER, ObjTag.PLAYER),
	m_state			(std::make_shared<PlayerState>()),
	m_input_slope	(v3d::GetZeroV())
{
	destination = prev_destination = v3d::GetZeroV();


	mass_kind = MassKind::kMedium;
	
	// モデル・アニメーションを設定
	m_modeler  = std::make_shared<Modeler>(m_transform, ModelPath.CHARACTER, kBasicScale);
	m_animator = std::make_shared<PlayerAnimator>(m_modeler, m_state);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	// 初期pos・dirを設定
	m_look_dir[TimeKind::kCurrent] = m_look_dir[TimeKind::kNext] = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetPos(CoordinateKind::kWorld, VGet(0.0f, 0.0f, 0.0f));
	m_transform->SetRot(CoordinateKind::kWorld, m_look_dir.at(TimeKind::kCurrent));

	// コライダー・トリガーを設定
	m_collider_creator->CreateCapsuleCollider	(this, m_modeler, kCapsuleRadius);
	m_collider_creator->CreateLandingRay		(this, 8.0f);
	m_collider_creator->CreateProjectRay		(this, 30.0f);
	m_collider_creator->CreateVisibleTrigger	(this, m_modeler);

	const auto pos = m_transform->GetPos(CoordinateKind::kWorld);
}

Player::~Player()
{

}

void Player::Init()
{
	m_state->Init(std::static_pointer_cast<Player>(shared_from_this()));
}

void Player::Update()
{
	if (!IsActive()) { return; }

	m_is_calc_look_dir	= false;

	m_move_dir_offset_speed				= kMoveDirOffsetSpeed;
	m_look_dir_offset_speed				= kLookDirOffsetSpeed;

	m_state		->Update(std::static_pointer_cast<Player>(shared_from_this()));
	//m_animator	->Update();



	// 仮
	const auto	input	= InputChecker::GetInstance();
	auto		dir		= v3d::GetZeroV();

	if (input->IsInput(KEY_INPUT_1))
	{
		dir -= GetMoveRight();
	}
	if (input->IsInput(KEY_INPUT_2))
	{
		dir += GetMoveRight();
	}
	if (input->IsInput(KEY_INPUT_3))
	{
		dir += GetMoveForward();
	}
	if (input->IsInput(KEY_INPUT_4))
	{
		dir -= GetMoveForward();
	}
	if (input->IsInput(KEY_INPUT_5))
	{
		dir += axis::GetWorldYAxis();
	}
	if (input->IsInput(KEY_INPUT_6))
	{
		dir -= axis::GetWorldYAxis();
	}
	dir = v3d::GetNormalizedV(dir);
	prev_destination = destination;
	destination += dir * 10.0f * GetDeltaTime();
	//if (destination != prev_destination)
	{
		anim_ik.LeftLegIK(m_modeler->GetModelHandle(), destination);
	}
	DrawSphere3D(destination, 4, 8, 0xffffff, 0xffffff, FALSE);
	




	CalcMoveDir();
	CalcLookDir();
	CalcMoveVelocity();

	m_collider_creator->CalcCapsuleColliderPos	(m_modeler, m_colliders);
	m_collider_creator->CalcLandingRayPos		(m_modeler, m_colliders);
	m_collider_creator->CalcProjectRayPos		(m_modeler, m_colliders);
	m_collider_creator->CalcVisibleTriggerPos	(m_modeler, m_colliders);

	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));
}

void Player::LateUpdate()
{
	if (!IsActive()) { return; }
}

void Player::Draw() const
{
	if (!IsActive()) { return; }

	//m_modeler->Draw();
	mixamo_helper::DrawFrames(m_modeler->GetModelHandle(), true, true, true, false);
}

void Player::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase*	target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto			target_name				= target_obj->GetName();
	const auto			target_tag				= target_obj->GetTag();
	const auto			target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingRay:
		m_is_landing = true;
		break;

	case ColliderKind::kProjectRay:
		if (hit_collider_pair.intersection)
		{
			m_project_pos = hit_collider_pair.intersection;
		}
		break;

	default:
		break;
	}
}

#pragma region State
void Player::Idle()
{
	CalcMoveSpeedIdle();
}

void Player::Move()
{
	m_move_dir.at(TimeKind::kNext) = v3d::GetZeroV();
	m_input_slope = v3d::GetZeroV();

	CalcInputSlopeFromPad();
	CalcInputSlopeFromCommand();

	CalcDirOfMovement();
	AllowCalcLookDir();

	CalcMoveSpeedWalk();
}

void Player::Run()
{
	CalcMoveSpeedRun();
}

void Player::Crouch()
{

}

void Player::CalcDirOfMovement()
{
	if (m_move_dir.at(TimeKind::kCurrent) != v3d::GetZeroV())
	{
		m_look_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(m_move_dir.at(TimeKind::kCurrent));
	}
}

void Player::CalcMoveSpeedIdle()
{
	// 速い状態から歩き状態に移行した場合、急速に減速させる
	if (m_move_speed > kSlowWalkSpeed) { m_move_speed = kSlowWalkSpeed; }

	math::Decrease(m_move_speed, kAcceleration, 0.0f);
}

void Player::CalcMoveSpeedWalk()
{
	const auto type = m_state->GetCurrentActionStateType();
	if (type != typeid(player_state::ActionNull) && type != typeid(player_state::Crouch)) { return; }

	if (VSize(m_input_slope) <= kWalkStickSlopeLimit - InputChecker::kStickDeadZone)
	{
		// 速い状態から歩き状態に移行した場合、急速に減速させる
		if (m_move_speed > kWalkSpeed) { m_move_speed = kWalkSpeed; }

		math::Increase(m_move_speed, kAcceleration, kSlowWalkSpeed, false);
		math::Decrease(m_move_speed, kAcceleration, kSlowWalkSpeed);
		return;
	}

	// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
	if (m_move_speed < kSlowWalkSpeed) { m_move_speed = kSlowWalkSpeed; }

	math::Increase(m_move_speed, kAcceleration, kWalkSpeed, false);
	math::Decrease(m_move_speed, kAcceleration, kWalkSpeed);
}

void Player::CalcMoveSpeedRun()
{
	if (m_state->GetCurrentMoveStateType() == typeid(player_state::Idle)) { return; }

	// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
	if (m_move_speed < kWalkSpeed) { m_move_speed = kWalkSpeed; }

	math::Increase(m_move_speed, kAcceleration, kRunSpeed, false);
}
#pragma endregion


#pragma region Getter
float Player::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer);
}
#pragma endregion


void Player::CalcInputSlopeFromPad()
{
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return; }

	const auto forward	= GetMoveForward();
	const auto right	= GetMoveRight();

	// 各方向のパラメーターを取得
	const auto input			= InputChecker::GetInstance();
	const auto forward_param	= input->GetInputParameter(pad::StickKind::kLSUp);
	const auto backward_param	= input->GetInputParameter(pad::StickKind::kLSDown);
	const auto left_param		= input->GetInputParameter(pad::StickKind::kLSLeft);
	const auto right_param		= input->GetInputParameter(pad::StickKind::kLSRight);

	// 入力値を取得
	if (forward_param)	{ m_input_slope += forward * (forward_param	 - InputChecker::kStickDeadZone); }
	if (backward_param) { m_input_slope += forward * (backward_param + InputChecker::kStickDeadZone); }
	if (left_param)		{ m_input_slope += right   * (left_param	 + InputChecker::kStickDeadZone); }
	if (right_param)	{ m_input_slope += right   * (right_param	 - InputChecker::kStickDeadZone); }

	m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(m_input_slope);
}

void Player::CalcInputSlopeFromCommand()
{
	if (m_move_dir.at(TimeKind::kNext) != v3d::GetZeroV()) { return; }

	const auto command					= CommandHandler::GetInstance();
	const auto forward					= GetMoveForward();
	const auto right					= GetMoveRight();
	auto	   current_input_slope		= v3d::GetZeroV();
	auto	   continue_input_slope		= v3d::GetZeroV();
	
	// 現在入力されているvelocityを取得
	if (command->IsExecute(CommandKind::kMoveUpPlayer,    TimeKind::kCurrent))
	{
		current_input_slope += forward;
	}
	if (command->IsExecute(CommandKind::kMoveDownPlayer,  TimeKind::kCurrent))
	{
		current_input_slope -= forward;
	}
	if (command->IsExecute(CommandKind::kMoveLeftPlayer,  TimeKind::kCurrent))
	{
		current_input_slope -= right;
	}
	if (command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kCurrent))
	{
		current_input_slope += right;
	}
	m_input_slope = v3d::GetNormalizedV(current_input_slope) * InputChecker::kStickMaxSlope;

	// 継続して入力されていたvelocityが、現在のvelocityと逆を向いていた場合現在のvelocityを縮める
	if (std::abs(math::GetAngleBetweenTwoVector(m_input_slope, continue_input_slope) - DX_PI_F) < math::kEpsilonLow)
	{
		m_input_slope += continue_input_slope;
	}

	m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(m_input_slope);
}

VECTOR Player::GetMoveForward()
{
	// MEMO : cinemachine brainを介するよりDxLib既存の関数を使用したほうが取得が早い
	auto forward = GetCameraFrontVector();
	forward.y = 0.0f;

	return v3d::GetNormalizedV(forward);
}

VECTOR Player::GetMoveRight()
{
	auto right = GetCameraRightVector();
	right.y = 0.0f;
	
	return v3d::GetNormalizedV(right);
}
