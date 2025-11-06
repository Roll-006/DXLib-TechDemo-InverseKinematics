#pragma once
#include <memory>

#include "transform.hpp"
#include "../GameTime/game_time_manager.hpp"

class CameraBody final
{
public:
	CameraBody(const std::shared_ptr<Transform>& owner_transform);
	~CameraBody();

	void CalcPos();
	void CalcDampedPos();

	/// @brief ターゲットを追尾する
	void TrackTarget()		  { m_is_track = true; }
	/// @brief ターゲットの追尾を停止する
	void StopTrackingTarget() { m_is_track = false; }


	#pragma region Attach / Detach
	/// @brief ターゲットをアタッチする(上書き可)
	void AttachTarget(const std::shared_ptr<Transform>& target_transform);
	void DetachTarget();
	#pragma endregion


	#pragma region Setter
	void SetFollowOffset	(const VECTOR& follow_offset) { m_follow_offset = follow_offset; }
	/// @brief 各座標のダンピング値を設定
	/// @param damping 指定可能範囲 : 0.0～20.0
	void SetDamping			(const VECTOR& damping);
	/// @brief ヨーのダンピング値を設定
	/// @param damping_yaw 指定可能範囲 : 0.0～20.0
	//void SetDampingYaw		(const float damping_yaw);
	///// @brief ピッチのダンピング値を設定
	///// @param damping_pitch 指定可能範囲 : 0.0～20.0
	//void SetDampingPitch	(const float damping_pitch);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] VECTOR GetFollowOffset()	const { return m_follow_offset; }
	[[nodiscard]] VECTOR GetDamping()		const { return m_damping; }
	//[[nodiscard]] float  GetDampingYaw()	const { return m_damping_yaw; }
	//[[nodiscard]] float  GetDampingPitch()	const { return m_damping_pitch; }
	#pragma endregion

private:
	static constexpr float kMaxDampingNum = 20.0f;

	std::shared_ptr<Transform> m_owner_transform;
	std::shared_ptr<Transform> m_target_transform;

	VECTOR m_destination_pos;		// 目的とする座標
	VECTOR m_current_pos;			// 現在の座標

	VECTOR m_follow_offset;			// カメラの補正座標(オフセット)

	VECTOR m_damping;				// 各座標の減衰値
	//float  m_damping_yaw;			// ヨーの減衰値
	//float  m_damping_pitch;		// ピッチの減衰値

	bool   m_is_track;				// 追尾を行うかを判定

	friend void from_json	(const nlohmann::json& data, CameraBody& body);
	friend void to_json		(nlohmann::json& data, const CameraBody& body);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, CameraBody& body)
{
	data.at("destination_pos")	.get_to(body.m_destination_pos);
	data.at("current_pos")		.get_to(body.m_current_pos);
	data.at("follow_offset")	.get_to(body.m_follow_offset);
	data.at("damping")			.get_to(body.m_damping);
	data.at("is_track")			.get_to(body.m_is_track);
}

inline void to_json(nlohmann::json& data, const CameraBody& body)
{
	data = nlohmann::json
	{
		{ "destination_pos",	body.m_destination_pos },
		{ "current_pos",		body.m_current_pos },
		{ "follow_offset",		body.m_follow_offset },
		{ "damping",			body.m_damping },
		{ "is_track",			body.m_is_track }
	};
}
#pragma endregion
