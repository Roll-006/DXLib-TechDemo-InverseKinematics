#pragma once
#include "../Base/character_base.hpp"

#include "../GameTime/game_time_manager.hpp"
#include "../Animator/player_animator.hpp"

class PlayerState;

class Player final : public CharacterBase
{
public:
	Player();
	~Player() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;


	#pragma region State
	void Idle();
	void Move();
	void Run();
	void Crouch();
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] float							GetDeltaTime()	const override;
	[[nodiscard]] std::shared_ptr<PlayerState>	GetState()		const { return m_state; }
	[[nodiscard]] float							GetMoveSpeed()	const { return m_move_speed; }
	#pragma endregion

private:
	void CalcMoveSpeedIdle();
	void CalcMoveSpeedWalk();
	void CalcMoveSpeedRun();
	void CalcDirOfMovement();

	void CalcInputSlopeFromPad();
	void CalcInputSlopeFromCommand();

	/// @brief カメラのforwardをもとにした進行方向ベクトルを取得する
	/// @return カメラのforwardのY軸を消し飛ばしたベクトル
	[[nodiscard]] VECTOR GetMoveForward();
	/// @brief カメラのrightをもとにした進行方向ベクトルを取得する
	/// @return カメラのrightのY軸を消し飛ばしたベクトル
	[[nodiscard]] VECTOR GetMoveRight();

private:
	static constexpr float  kBasicScale							= 0.3f;

	static constexpr float  kSlowWalkSpeed						= 30.0f;
	static constexpr float  kWalkSpeed							= 70.0f;
	static constexpr float  kRunSpeed							= 120.0f;
	static constexpr float  kAcceleration						= 1.0f;					// 加速度(減速度も共通)

	static constexpr float  kMoveDirOffsetSpeed					= 5.0f;					// 移動方向の補正速度
	static constexpr float  kLookDirOffsetSpeed					= 6.0f;					// 見る方向の補正角度

	static constexpr int    kWalkStickSlopeLimit				= 15000;				// 歩き状態とするスティック傾きの上限

	static constexpr float  kCapsuleRadius						= 8.0f;
	static constexpr float  kCollisionAreaRadius				= 37.0f;
	static constexpr VECTOR kCollisionAreaOffset				= { 0.0f, 26.0f, 0.0f };
	static constexpr float  kLandingTriggerRadius				= 6.0f;

	static constexpr float  kCameraAimOffsetBasicSpeed			= 40.0f;
	
private:
	std::shared_ptr<PlayerState>	m_state;
	VECTOR							m_input_slope;
};
