#include "control_virtual_cameras_controller.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Object/player.hpp"
#include "../JSON/json_loader.hpp"

ControlVirtualCamerasController::ControlVirtualCamerasController(Player& player) :
	virtual_camera_controller_kind(VirtualCameraControllerKind::kControl),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_player						(player),
	m_rot_stand_control_camera		(std::make_shared<VirtualCamera>(ObjName.ROT_STAND_CAMERA,  BlendActivationPolicyKind::kKeepOriginCamera)),
	m_rot_crouch_control_camera		(std::make_shared<VirtualCamera>(ObjName.ROT_CROUCH_CAMERA, BlendActivationPolicyKind::kKeepOriginCamera)),
	m_aim_transform					(std::make_shared<Transform>()),
	m_current_aim_pos				(v3d::GetZeroV()),
	m_move_dir						(v3d::GetZeroV()),
	m_velocity						(v3d::GetZeroV())
{
	JSONLoader json_loader;
	nlohmann::json data;
	if (json_loader.Load("Data/JSON/control_virtual_cameras_controller.json", data))
	{
		const auto rot_stand_control_camera		= data.at("control_virtual_cameras_controller").at("rot_stand_control_camera").get<VirtualCamera>();
		m_rot_stand_control_camera				= std::make_shared<VirtualCamera>(rot_stand_control_camera);
		m_rot_stand_control_camera->AttachTarget(m_aim_transform);

		const auto rot_crouch_control_camera	= data.at("control_virtual_cameras_controller").at("rot_crouch_control_camera").get<VirtualCamera>();
		m_rot_crouch_control_camera				= std::make_shared<VirtualCamera>(rot_crouch_control_camera);
		m_rot_crouch_control_camera->AttachTarget(m_aim_transform);

		virtual_camera_controller_kind	= data.at("control_virtual_cameras_controller").at("virtual_camera_controller_kind");
		camera_aim_offset_basic_speed	= data.at("control_virtual_cameras_controller").at("camera_aim_offset_basic_speed");
		max_vertical_input_angle		= data.at("control_virtual_cameras_controller").at("max_vertical_input_angle");
		min_vertical_input_angle		= data.at("control_virtual_cameras_controller").at("min_vertical_input_angle");
		move_speed_with_stick			= data.at("control_virtual_cameras_controller").at("move_speed_with_stick");
		move_speed_with_mouse			= data.at("control_virtual_cameras_controller").at("move_speed_with_mouse");
		move_speed_with_button			= data.at("control_virtual_cameras_controller").at("move_speed_with_button");
	}

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetBlendTime(1.0f);
	cinemachine_brain->AddVirtualCamera(m_rot_stand_control_camera,  true);
	cinemachine_brain->AddVirtualCamera(m_rot_crouch_control_camera, false);
}

ControlVirtualCamerasController::~ControlVirtualCamerasController()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCamera(m_rot_stand_control_camera->GetCameraHandle());
	cinemachine_brain->RemoveVirtualCamera(m_rot_crouch_control_camera->GetCameraHandle());
}

void ControlVirtualCamerasController::Init()
{

}

void ControlVirtualCamerasController::Update()
{
	if (!IsActive()) { return; }

	Move();
}

void ControlVirtualCamerasController::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcAimPos();

	MATRIX result_m = math::ConvertEulerAnglesToXYZRotMatrix(m_input_angle[TimeKind::kCurrent]);
	m_aim_transform->SetRot(CoordinateKind::kWorld, MGetRotElem(result_m));
}

VirtualCameraControllerKind ControlVirtualCamerasController::GetVirtualCameraControllerKind() const
{
	return virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCamera> ControlVirtualCamerasController::GetHaveVirtualCamera(const std::string& name) const
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera = cinemachine_brain->GetVirtualCamera(name);
	if (!camera) { return nullptr; }

	if (   camera == m_rot_stand_control_camera
		|| camera == m_rot_crouch_control_camera)
	{
		return camera;
	}

	return nullptr;
}

std::vector<std::shared_ptr<VirtualCamera>> ControlVirtualCamerasController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCamera>>{m_rot_stand_control_camera, m_rot_crouch_control_camera};
}

void ControlVirtualCamerasController::Move()
{
	m_move_dir = v3d::GetZeroV();
	m_velocity = v3d::GetZeroV();

	CalcMoveDirFromPad();
	CalcMoveDirFromMouse();
	CalcMoveDirFromCommand();

	CalcInputAngle();
}

void ControlVirtualCamerasController::CalcMoveDirFromPad()
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

void ControlVirtualCamerasController::CalcMoveDirFromMouse()
{
	//if (m_is_init_aiming) { return; }
	if (m_move_dir != v3d::GetZeroV()) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard) { return; }

	// 移動速度を取得
	Vector2D<float> velocity_2d = InputChecker::GetInstance()->GetMouseVelocity(TimeKind::kCurrent);
	m_velocity = VGet(velocity_2d.y, velocity_2d.x, 0.0f) * move_speed_with_mouse;

	m_move_dir = v3d::GetNormalizedV(m_velocity);
}

void ControlVirtualCamerasController::CalcMoveDirFromCommand()
{
	//if (m_is_init_aiming) { return; }

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

void ControlVirtualCamerasController::CalcAimPos()
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

	m_aim_transform->SetPos(CoordinateKind::kWorld, m_current_aim_pos);
}

void ControlVirtualCamerasController::CalcInputAngle()
{
	const auto time_manager = GameTimeManager::GetInstance();
	m_velocity *= time_manager->GetDeltaTime(TimeScaleLayerKind::kCamera);

	// 角度を取得
	const auto command = CommandHandler::GetInstance();
	if (command->IsExecute(CommandKind::kMoveUpCamera,		TimeKind::kCurrent)) { m_input_angle[TimeKind::kCurrent].x += m_velocity.x; }
	if (command->IsExecute(CommandKind::kMoveDownCamera,	TimeKind::kCurrent)) { m_input_angle[TimeKind::kCurrent].x += m_velocity.x; }
	if (command->IsExecute(CommandKind::kMoveLeftCamera,	TimeKind::kCurrent)) { m_input_angle[TimeKind::kCurrent].y += m_velocity.y; }
	if (command->IsExecute(CommandKind::kMoveRightCamera,	TimeKind::kCurrent)) { m_input_angle[TimeKind::kCurrent].y += m_velocity.y; }

	m_input_angle[TimeKind::kCurrent].y = math::ConnectMinusValueToValue(m_input_angle[TimeKind::kCurrent].y, DX_PI_F);

	// 角度制限
	if (m_input_angle[TimeKind::kCurrent].x < min_vertical_input_angle * math::kDegToRad) { m_input_angle[TimeKind::kCurrent].x = min_vertical_input_angle * math::kDegToRad; }
	if (m_input_angle[TimeKind::kCurrent].x > max_vertical_input_angle * math::kDegToRad) { m_input_angle[TimeKind::kCurrent].x = max_vertical_input_angle * math::kDegToRad; }
}
