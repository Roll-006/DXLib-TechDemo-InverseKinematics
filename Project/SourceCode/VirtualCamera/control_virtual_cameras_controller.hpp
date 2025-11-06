#pragma once
#include "../Interface/i_virtual_camera_controller.hpp"

#include "../Command/command_handler.hpp"
#include "virtual_camera.hpp"

class CinemachineBrain;
class Player;

class ControlVirtualCamerasController final : public IVirtualCameraController
{
public:
	ControlVirtualCamerasController(Player& player);
	~ControlVirtualCamerasController();

	void Init();
	void Update()		override;
	void LateUpdate()	override;

	void Activate()   override { m_is_active = true; }
	void Deactivate() override { m_is_active = false; }

	[[nodiscard]] VECTOR GetVelocity()		 const			{ return m_velocity; }
	[[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const override;
	[[nodiscard]] std::shared_ptr<VirtualCamera> GetHaveVirtualCamera(const std::string& name) const override;
	[[nodiscard]] std::vector<std::shared_ptr<VirtualCamera>> GetHaveAllVirtualCamera()  const override;
	[[nodiscard]] int  GetControllerHandle() const override { return m_controller_handle; }
	[[nodiscard]] bool IsActive()			 const override { return m_is_active; }

private:
	void Move();

	void CalcMoveDirFromPad();
	void CalcMoveDirFromMouse();
	void CalcMoveDirFromCommand();

	void CalcAimPos();
	void CalcInputAngle();

private:
	VirtualCameraControllerKind				virtual_camera_controller_kind;

	float									camera_aim_offset_basic_speed;
	float									max_vertical_input_angle;
	float									min_vertical_input_angle;
	float									move_speed_with_stick;
	float									move_speed_with_mouse;
	float									move_speed_with_button;

	int										m_controller_handle;
	bool									m_is_active;

	Player&									m_player;
	std::shared_ptr<VirtualCamera>			m_rot_stand_control_camera;			// 回転カメラ(立ち状態)
	std::shared_ptr<VirtualCamera>			m_rot_crouch_control_camera;		// 回転カメラ(しゃがみ状態)

	std::shared_ptr<Transform>				m_aim_transform;					// カメラが起点とするトランスフォーム
	VECTOR									m_current_aim_pos;					// 現在の起点座標

	VECTOR									m_move_dir;							// 移動方向
	VECTOR									m_velocity;							// 速度ベクトル
	std::unordered_map<TimeKind, VECTOR>	m_input_angle;						// 入力角度

	friend void from_json(const nlohmann::json& data, ControlVirtualCamerasController& cameras_controller);
	friend void to_json  (nlohmann::json& data, const ControlVirtualCamerasController& cameras_controller);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, ControlVirtualCamerasController& cameras_controller)
{
	data.at("virtual_camera_controller_kind")		.get_to(cameras_controller.virtual_camera_controller_kind);
	data.at("camera_aim_offset_basic_speed")		.get_to(cameras_controller.camera_aim_offset_basic_speed);
	data.at("max_vertical_input_angle")				.get_to(cameras_controller.max_vertical_input_angle);
	data.at("min_vertical_input_angle")				.get_to(cameras_controller.min_vertical_input_angle);
	data.at("move_speed_with_stick")				.get_to(cameras_controller.move_speed_with_stick);
	data.at("move_speed_with_mouse")				.get_to(cameras_controller.move_speed_with_mouse);
	data.at("move_speed_with_button")				.get_to(cameras_controller.move_speed_with_button);
	data.at("rot_stand_control_camera")				.get_to(*cameras_controller.m_rot_stand_control_camera.get());
	data.at("rot_crouch_control_camera")			.get_to(*cameras_controller.m_rot_crouch_control_camera.get());
}

inline void to_json(nlohmann::json& data, const ControlVirtualCamerasController& cameras_controller)
{
	data = nlohmann::json
	{
		{"virtual_camera_controller_kind",			cameras_controller.virtual_camera_controller_kind},
		{"camera_aim_offset_basic_speed",			cameras_controller.camera_aim_offset_basic_speed},
		{"max_vertical_input_angle",				cameras_controller.max_vertical_input_angle},
		{"min_vertical_input_angle",				cameras_controller.min_vertical_input_angle},
		{"move_speed_with_stick",					cameras_controller.move_speed_with_stick},
		{"move_speed_with_mouse",					cameras_controller.move_speed_with_mouse},
		{"move_speed_with_button",					cameras_controller.move_speed_with_button},
		{"rot_stand_control_camera",				*cameras_controller.m_rot_stand_control_camera.get()},
		{"rot_crouch_control_camera",				*cameras_controller.m_rot_crouch_control_camera.get()},
	};
}
#pragma endregion
