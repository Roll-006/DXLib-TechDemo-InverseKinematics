#include <JSON/json_loader.hpp>

#include "control_virtual_cameras_controller.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Object/player.hpp"
#include "../Debugger/debugger.hpp"

ControlVirtualCamerasController::ControlVirtualCamerasController(Player& player) :
	virtual_camera_controller_kind(VirtualCameraControllerKind::kControl),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_is_using_freedom_camera		(false),
	m_player						(player),
	m_rot_stand_control_camera		(nullptr),
	m_rot_crouch_control_camera		(nullptr),
	m_freedom_control_camera		(nullptr),
	m_rot_aim_transform				(std::make_shared<Transform>()),
	m_freedom_aim_transform			(std::make_shared<Transform>()),
	m_current_aim_pos				(v3d::GetZeroV()),
	m_move_dir						(v3d::GetZeroV()),
	m_velocity						(v3d::GetZeroV()),
	m_rot_camera_input_angle		(v3d::GetZeroV()),
	m_freedom_camera_input_angle	(v3d::GetZeroV())
{
	nlohmann::json data;
	if (json_loader::Load("Data/JSON/control_virtual_cameras_controller.json", data))
	{
		const auto rot_stand_control_camera		= data.at("control_virtual_cameras_controller").at("rot_stand_control_camera").get<VirtualCamera>();
		m_rot_stand_control_camera				= std::make_shared<VirtualCamera>(rot_stand_control_camera);
		m_rot_stand_control_camera				->AttachTarget(m_rot_aim_transform);

		const auto rot_crouch_control_camera	= data.at("control_virtual_cameras_controller").at("rot_crouch_control_camera").get<VirtualCamera>();
		m_rot_crouch_control_camera				= std::make_shared<VirtualCamera>(rot_crouch_control_camera);
		m_rot_crouch_control_camera				->AttachTarget(m_rot_aim_transform);

		const auto freedom_control_camera		= data.at("control_virtual_cameras_controller").at("freedom_control_camera").get<VirtualCamera>();
		m_freedom_control_camera				= std::make_shared<VirtualCamera>(freedom_control_camera);
		m_freedom_control_camera				->AttachTarget(m_freedom_aim_transform);

		virtual_camera_controller_kind	= data.at("control_virtual_cameras_controller").at("virtual_camera_controller_kind");
		camera_aim_offset_basic_speed	= data.at("control_virtual_cameras_controller").at("camera_aim_offset_basic_speed");
		max_vertical_input_angle		= data.at("control_virtual_cameras_controller").at("max_vertical_input_angle");
		min_vertical_input_angle		= data.at("control_virtual_cameras_controller").at("min_vertical_input_angle");
		move_speed_with_stick			= data.at("control_virtual_cameras_controller").at("move_speed_with_stick");
		move_speed_with_mouse			= data.at("control_virtual_cameras_controller").at("move_speed_with_mouse");
		move_speed_with_button			= data.at("control_virtual_cameras_controller").at("move_speed_with_button");
	}

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetBlendTime(0.2f);
	cinemachine_brain->AddVirtualCamera(m_rot_stand_control_camera,		true);
	cinemachine_brain->AddVirtualCamera(m_rot_crouch_control_camera,	false);
	cinemachine_brain->AddVirtualCamera(m_freedom_control_camera,		false);
}

ControlVirtualCamerasController::~ControlVirtualCamerasController()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCamera(m_rot_stand_control_camera	->GetCameraHandle());
	cinemachine_brain->RemoveVirtualCamera(m_rot_crouch_control_camera	->GetCameraHandle());
	cinemachine_brain->RemoveVirtualCamera(m_freedom_control_camera		->GetCameraHandle());
}

void ControlVirtualCamerasController::Init()
{

}

void ControlVirtualCamerasController::Update()
{
	if (!IsActive()) { return; }

	JudgeUseFreedomCamera();

	m_move_dir = v3d::GetZeroV();
	m_velocity = v3d::GetZeroV();

	MoveRotCamera();
	MoveFreedomCamera();
}

void ControlVirtualCamerasController::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcRotCameraAimPos();

	ApplyAngleRotCamera();
	ApplyAngleFreedomCamera();
}

VirtualCameraControllerKind ControlVirtualCamerasController::GetVirtualCameraControllerKind() const
{
	return virtual_camera_controller_kind;
}


#pragma region 回転カメラ
void ControlVirtualCamerasController::MoveRotCamera()
{
	if (m_is_using_freedom_camera[TimeKind::kCurrent]) { return; }

	CalcRotCameraMoveDirFromPad();
	CalcRotCameraMoveDirFromMouse();
	CalcRotCameraMoveDirFromCommand();

	CalcRotCameraInputAngle();
}

void ControlVirtualCamerasController::CalcRotCameraMoveDirFromPad()
{
	if (m_move_dir != v3d::GetZeroV()) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return; }

	// 各方向のパラメーターを取得
	const auto input = InputChecker::GetInstance();
	const auto up_param		= input->GetInputParameter(pad::StickKind::kRSUp);
	const auto down_param	= input->GetInputParameter(pad::StickKind::kRSDown);
	const auto left_param	= input->GetInputParameter(pad::StickKind::kRSLeft);
	const auto right_param	= input->GetInputParameter(pad::StickKind::kRSRight);

	// 速度ベクトル・入力判定を取得
	if (up_param)	 { m_velocity.x = -math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxSlope,  up_param);	  }
	if (down_param)  { m_velocity.x =  math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinSlope, -down_param);  }
	if (left_param)  { m_velocity.y = -math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinSlope, -left_param);  }
	if (right_param) { m_velocity.y =  math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxSlope,  right_param); }
	m_velocity *= move_speed_with_stick;

	m_move_dir = v3d::GetNormalizedV(m_velocity);
}

void ControlVirtualCamerasController::CalcRotCameraMoveDirFromMouse()
{
	if (m_move_dir != v3d::GetZeroV()) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard) { return; }

	// 移動速度を取得
	Vector2D<float> velocity_2d = InputChecker::GetInstance()->GetMouseVelocity(TimeKind::kCurrent);
	m_velocity = VGet(velocity_2d.y, velocity_2d.x, 0.0f) * move_speed_with_mouse;

	m_move_dir = v3d::GetNormalizedV(m_velocity);
}

void ControlVirtualCamerasController::CalcRotCameraMoveDirFromCommand()
{
	const auto input	= InputChecker	::GetInstance();
	const auto command	= CommandHandler::GetInstance();

	// パッド・マウスの入力は例外的にコマンド入力の動きを適用しない
	for (int i = 0; i < 4; ++i) { if (input->IsInput(static_cast<mouse::SlideDirKind>(i)))	{ return; } }
	for (int i = 0; i < 8; ++i) { if (input->IsInput(static_cast<pad::StickKind>(i)))		{ return; } }

	// コマンドパターンで入力された場合の速度・方向を取得
	if (command->IsExecute(CommandKind::kMoveUpCamera,		TimeKind::kCurrent)) { m_move_dir.x = -1; }
	if (command->IsExecute(CommandKind::kMoveDownCamera,	TimeKind::kCurrent)) { m_move_dir.x =  1; }
	if (command->IsExecute(CommandKind::kMoveLeftCamera,	TimeKind::kCurrent)) { m_move_dir.y = -1; }
	if (command->IsExecute(CommandKind::kMoveRightCamera,	TimeKind::kCurrent)) { m_move_dir.y =  1; }

	if (m_move_dir != v3d::GetZeroV())
	{
		m_move_dir = v3d::GetNormalizedV(m_move_dir);
		m_velocity = m_move_dir * move_speed_with_button;
	}
}

void ControlVirtualCamerasController::CalcRotCameraAimPos()
{
	const auto modeler = m_player.GetModeler();
	modeler->ApplyMatrix();

	// 追跡するボーンから行列を取得
	const TCHAR*	bone_name			= BonePath.SPINE_2;
	const auto		model_handle		= modeler->GetModelHandle();
	const auto		bone_index			= MV1SearchFrame(model_handle, bone_name);
	auto			bone_world_m		= MV1GetFrameLocalWorldMatrix(model_handle, bone_index);
	auto			aim_pos				= MGetTranslateElem(bone_world_m);

	// ボーンと同じ高さの位置を追跡
	const auto		player_transform	= m_player.GetTransform();
	const auto		begin_pos			= player_transform->GetPos(CoordinateKind::kWorld);
	const auto		distance			= begin_pos - aim_pos;
	aim_pos = begin_pos + player_transform->GetUp(CoordinateKind::kWorld) * VSize(distance);

	m_current_aim_pos = math::GetApproachedVector(m_current_aim_pos, aim_pos, camera_aim_offset_basic_speed * VSize(aim_pos - m_current_aim_pos));

	m_rot_aim_transform->SetPos(CoordinateKind::kWorld, m_current_aim_pos);
}

void ControlVirtualCamerasController::CalcRotCameraInputAngle()
{
	const auto time_manager = GameTimeManager::GetInstance();
	m_velocity *= time_manager->GetDeltaTime(TimeScaleLayerKind::kCamera);

	// 角度を取得
	const auto command = CommandHandler::GetInstance();
	if (command->IsExecute(CommandKind::kMoveUpCamera,		TimeKind::kCurrent)) { m_rot_camera_input_angle.x += m_velocity.x; }
	if (command->IsExecute(CommandKind::kMoveDownCamera,	TimeKind::kCurrent)) { m_rot_camera_input_angle.x += m_velocity.x; }
	if (command->IsExecute(CommandKind::kMoveLeftCamera,	TimeKind::kCurrent)) { m_rot_camera_input_angle.y += m_velocity.y; }
	if (command->IsExecute(CommandKind::kMoveRightCamera,	TimeKind::kCurrent)) { m_rot_camera_input_angle.y += m_velocity.y; }

	m_rot_camera_input_angle.y = math::ConnectMinusValueToValue(m_rot_camera_input_angle.y, DX_PI_F);

	// 角度制限
	if (m_rot_camera_input_angle.x < min_vertical_input_angle * math::kDegToRad) { m_rot_camera_input_angle.x = min_vertical_input_angle * math::kDegToRad; }
	if (m_rot_camera_input_angle.x > max_vertical_input_angle * math::kDegToRad) { m_rot_camera_input_angle.x = max_vertical_input_angle * math::kDegToRad; }
}

void ControlVirtualCamerasController::ApplyAngleRotCamera()
{
	MATRIX result_m = math::ConvertEulerAnglesToXYZRotMatrix(m_rot_camera_input_angle);
	m_rot_aim_transform->SetRot(CoordinateKind::kWorld, MGetRotElem(result_m));
}
#pragma endregion


#pragma region 自由移動カメラ
void ControlVirtualCamerasController::MoveFreedomCamera()
{
	if (!m_is_using_freedom_camera[TimeKind::kCurrent]) { return; }

	const auto input = InputChecker::GetInstance();
	if (!input->IsInput(KEY_INPUT_LCONTROL))
	{
		// 現在入力されているvelocityを取得
		const auto forward = GetMoveForward();
		const auto right = GetMoveRight();
		if (input->IsInput(KEY_INPUT_UP))
		{
			m_move_dir += input->IsInput(KEY_INPUT_LSHIFT) ? axis::GetWorldYAxis() : forward;
		}
		if (input->IsInput(KEY_INPUT_DOWN))
		{
			m_move_dir -= input->IsInput(KEY_INPUT_LSHIFT) ? axis::GetWorldYAxis() : forward;
		}
		if (input->IsInput(KEY_INPUT_LEFT))
		{
			m_move_dir -= right;
		}
		if (input->IsInput(KEY_INPUT_RIGHT))
		{
			m_move_dir += right;
		}
		m_velocity = v3d::GetNormalizedV(m_move_dir) * 2.0f;
		m_freedom_aim_transform->Move(CoordinateKind::kWorld, m_velocity);
	}
	else
	{
		CalcFreedomCameraInputAngle();
	}
}

void ControlVirtualCamerasController::CalcFreedomCameraInputAngle()
{
	const auto input = InputChecker::GetInstance();

	// 角度を取得
	const auto delta_time = m_freedom_control_camera->GetDeltaTime();
	if (input->IsInput(KEY_INPUT_UP))	 { m_freedom_camera_input_angle.x += -2.0f * delta_time; }
	if (input->IsInput(KEY_INPUT_DOWN))  { m_freedom_camera_input_angle.x +=  2.0f * delta_time; }
	if (input->IsInput(KEY_INPUT_LEFT))  { m_freedom_camera_input_angle.y += -2.0f * delta_time; }
	if (input->IsInput(KEY_INPUT_RIGHT)) { m_freedom_camera_input_angle.y +=  2.0f * delta_time; }

	m_freedom_camera_input_angle.y = math::ConnectMinusValueToValue(m_freedom_camera_input_angle.y, DX_PI_F);

	// 角度制限
	if (m_freedom_camera_input_angle.x < min_vertical_input_angle * math::kDegToRad) { m_freedom_camera_input_angle.x = min_vertical_input_angle * math::kDegToRad; }
	if (m_freedom_camera_input_angle.x > max_vertical_input_angle * math::kDegToRad) { m_freedom_camera_input_angle.x = max_vertical_input_angle * math::kDegToRad; }
}

void ControlVirtualCamerasController::ApplyAngleFreedomCamera()
{
	MATRIX result_m = math::ConvertEulerAnglesToXYZRotMatrix(m_freedom_camera_input_angle);
	m_freedom_aim_transform->SetRot(CoordinateKind::kWorld, MGetRotElem(result_m));
}

void ControlVirtualCamerasController::JudgeUseFreedomCamera()
{
	const auto debugger = Debugger::GetInstance();

	m_is_using_freedom_camera[TimeKind::kPrev]		= m_is_using_freedom_camera[TimeKind::kCurrent];
	m_is_using_freedom_camera[TimeKind::kCurrent]	= debugger->IsUsingDebug() ? true : false;

	if (m_is_using_freedom_camera[TimeKind::kCurrent])
	{
		// アクティブ化されて最初のフレームは回転カメラの情報を引き継ぐ
		if (!m_is_using_freedom_camera[TimeKind::kPrev])
		{
			m_freedom_aim_transform->SetMatrix(CoordinateKind::kWorld, m_rot_aim_transform->GetMatrix(CoordinateKind::kWorld));
			m_freedom_camera_input_angle = m_rot_camera_input_angle;
		}

		m_freedom_control_camera->Activate();
	}
	else
	{
		if (m_is_using_freedom_camera[TimeKind::kPrev])
		{
			m_rot_aim_transform->SetRot(CoordinateKind::kWorld, m_freedom_aim_transform->GetRotMatrix(CoordinateKind::kWorld));
			m_rot_camera_input_angle = m_freedom_camera_input_angle;
		}

		m_freedom_control_camera->Deactivate();
	}
}

VECTOR ControlVirtualCamerasController::GetMoveForward()
{
	// MEMO : cinemachine brainを介するよりDxLib既存の関数を使用したほうが取得が早い
	auto forward = GetCameraFrontVector();
	forward.y = 0.0f;

	return v3d::GetNormalizedV(forward);
}

VECTOR ControlVirtualCamerasController::GetMoveRight()
{
	auto right = GetCameraRightVector();
	right.y = 0.0f;

	return v3d::GetNormalizedV(right);
}
#pragma endregion
