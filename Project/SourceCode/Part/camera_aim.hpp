#pragma once
#include <memory>
#include <algorithm>
#include <Vector/vector_2d.hpp>

#include "transform.hpp"
#include "../GameTime/game_time_manager.hpp"

class CameraAim final
{
public:
	CameraAim(const std::shared_ptr<Transform>& owner_transform);
	~CameraAim();

	void CalcRot();
	void CalcDampedRot();

	/// @brief ターゲットを追尾する
	void TrackTarget()			{ m_is_track = true; }
	/// @brief ターゲットの追尾を停止する
	void StopTrackingTarget()	{ m_is_track = false; }


	#pragma region Attach / Detach
	/// @brief ターゲットをアタッチする(上書き可)
	void AttachTarget(const std::shared_ptr<Transform>& target_transform);
	void DetachTarget();
	#pragma endregion


	#pragma region Setter
	/// @brief 回転量を設定する
	/// @brief WARNING : 回転量の補正値は自動的に適用されるため考慮する必要はない
	/// @param rot_matrix 回転行列
	void SetRot					(const MATRIX& rot_matrix);
	void SetTrackedObjOffset	(const VECTOR& tracked_obj_offset) { m_tracked_obj_offset = tracked_obj_offset; }
	/// @brief 水平方向のダンピング値を設定
	/// @param horizontal_damping 指定可能範囲 : 0.0～20.0
	void SetHorizontalDamping	(const float horizontal_damping);
	/// @brief 垂直方向のダンピング値を設定
	/// @param vertical_damping 指定可能範囲 : 0.0～20.0
	void SetVerticalDamping		(const float vertical_damping);
	/// @brief ターゲットをスクリーンのどこに位置させるかを設定
	/// @param screen 指定可能範囲 : 0.0～1.0
	void SetScreen				(const Vector2D<float>& screen);
	/// @brief デッドゾーンを設定
	/// @param dead_zone 指定可能範囲 : 0.0～1.0
	void SetDeadZone			(const Vector2D<float>& dead_zone);
	/// @brief ソフトゾーンを設定
	/// @param soft_zone 指定可能範囲 : 0.0～1.0
	void SetSoftZone			(const Vector2D<float>& soft_zone);
	/// @brief デッドゾーンおよびソフトゾーンのずらし量を設定
	/// @param bias 指定可能範囲 : -0.5～0.5
	void SetBias				(const Vector2D<float>& bias);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] VECTOR			GetAimPos()				const { return m_current_aim_pos; }
	[[nodiscard]] VECTOR			GetTrackedObjOffset()	const { return m_tracked_obj_offset; }
	[[nodiscard]] float				GetHorizontalDamping()	const { return m_horizontal_damping; }
	[[nodiscard]] float				GetVerticalDamping()	const { return m_vertical_damping; }
	[[nodiscard]] Vector2D<float>	GetScreen()				const { return m_screen; }
	[[nodiscard]] Vector2D<float>	GetDeadZone()			const { return m_dead_zone; }
	[[nodiscard]] Vector2D<float>	GetSoftZone()			const { return m_soft_zone; }
	[[nodiscard]] Vector2D<float>	GetBias()				const { return m_bias; }
	#pragma endregion

private:
	void CalcAimPos();
	void CalcDampedAimPos();

private:
	static constexpr float kMaxDampingNum = 20.0f;

	std::shared_ptr<Transform> m_owner_transform;
	std::shared_ptr<Transform> m_target_transform;

	MATRIX			m_rot_matrix;				// 回転行列
	VECTOR			m_destination_aim_pos;		// 目的とする見る座標
	VECTOR			m_current_aim_pos;			// 現在の見ている座標

	VECTOR			m_tracked_obj_offset;		// ターゲットを見る方向のオフセット

	float			m_horizontal_damping;		// 水平方向の減衰値
	float			m_vertical_damping;			// 垂直方向の減衰値
	Vector2D<float>	m_screen;					// ターゲットをスクリーンのどこに位置させるか
	Vector2D<float>	m_dead_zone;				// デッドゾーン
	Vector2D<float>	m_soft_zone;				// ソフトゾーン
	Vector2D<float> m_bias;						// デッドゾーンおよびソフトゾーンのずらし量

	bool			m_is_track;					// 追尾を行うかを判定

	friend void from_json	(const nlohmann::json& data, CameraAim& aim);
	friend void to_json		(nlohmann::json& data, const CameraAim& aim);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, CameraAim& aim)
{
	data.at("rot_matrix")			.get_to(aim.m_rot_matrix);
	data.at("destination_aim_pos")	.get_to(aim.m_destination_aim_pos);
	data.at("current_aim_pos")		.get_to(aim.m_current_aim_pos);
	data.at("tracked_obj_offset")	.get_to(aim.m_tracked_obj_offset);
	data.at("horizontal_damping")	.get_to(aim.m_horizontal_damping);
	data.at("vertical_damping")		.get_to(aim.m_vertical_damping);
	data.at("screen")				.get_to(aim.m_screen);
	data.at("dead_zone")			.get_to(aim.m_dead_zone);
	data.at("soft_zone")			.get_to(aim.m_soft_zone);
	data.at("bias")					.get_to(aim.m_bias);
	data.at("is_track")				.get_to(aim.m_is_track);
}

inline void to_json(nlohmann::json& data, const CameraAim& aim)
{
	data = nlohmann::json
	{
		{ "rot_matrix",				aim.m_rot_matrix },
		{ "destination_aim_pos",	aim.m_destination_aim_pos },
		{ "current_aim_pos",		aim.m_current_aim_pos },
		{ "tracked_obj_offset",		aim.m_tracked_obj_offset },
		{ "horizontal_damping",		aim.m_horizontal_damping },
		{ "vertical_damping",		aim.m_vertical_damping },
		{ "screen",					aim.m_screen },
		{ "dead_zone",				aim.m_dead_zone },
		{ "soft_zone",				aim.m_soft_zone },
		{ "bias",					aim.m_bias },
		{ "is_track",				aim.m_is_track }
	};
}
#pragma endregion
