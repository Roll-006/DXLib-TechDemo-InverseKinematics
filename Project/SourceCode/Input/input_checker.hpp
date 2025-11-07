#pragma once
#include <vector>
#include <unordered_map>
#include <tuple>

#include "../Base/singleton_base.hpp"

#include "../GameTime/game_time_manager.hpp"
#include "../Window/window.hpp"

#include "../Data/input_data.hpp"
#include "../Kind/time_kind.hpp"
#include "../Kind/device_kind.hpp"
#include "../Concept/input_concepts.hpp"

#include <Vector/vector_2d.hpp>

class CommandHandler;

using InputKey		= std::pair<InputCode, TimeKind>;
using InputValue	= InputData;

struct KeyHash
{
	std::size_t operator()(const InputKey& input_key) const noexcept
	{
		auto h1 = std::hash<InputCode>{}(static_cast<InputCode>(input_key.first));
		auto h2 = std::hash<TimeKind>{}(static_cast<TimeKind>(input_key.second));
		return h1 ^ (h2 << 1);
	}
};

struct KeyEquality
{
	bool operator()(const InputKey& input_key1, const InputKey& input_key2) const noexcept
	{
		return input_key1 == input_key2;
	}
};

/// @brief 役割 : 入力デバイスの検出, 入力状態の取得, 入力時間の計測
class InputChecker final : public SingletonBase<InputChecker>
{
public:
	void Update();
	void LateUpdate();

	/// @brief マウスカーソルを画面中央に固定するかを設定
	void SetCursorToCenter(const bool is_lock) { m_is_lock_mouse_pos = is_lock; }

	/// @brief 入力テンプレート値から入力コードへ変換
	template<input_concepts::InputT InputT>
	[[nodiscard]] InputCode ConvertInputTemplateToInputCode(const InputT& input_t)
	{
		if (std::is_same_v<int,					InputT>) { return InputCode(InputKind::kKey,		static_cast<int>(input_t)); }
		if (std::is_same_v<mouse::ButtonKind,	InputT>) { return InputCode(InputKind::kMouseButton,static_cast<int>(input_t)); }
		if (std::is_same_v<mouse::WheelKind,	InputT>) { return InputCode(InputKind::kMouseWheel, static_cast<int>(input_t)); }
		if (std::is_same_v<mouse::SlideDirKind, InputT>) { return InputCode(InputKind::kMouseSlide, static_cast<int>(input_t)); }
		if (std::is_same_v<pad::ButtonKind,		InputT>) { return InputCode(InputKind::kPadButton,	static_cast<int>(input_t)); }
		if (std::is_same_v<pad::TriggerKind,	InputT>) { return InputCode(InputKind::kPadTrigger, static_cast<int>(input_t)); }
		if (std::is_same_v<pad::StickKind,		InputT>) { return InputCode(InputKind::kPadStick,	static_cast<int>(input_t)); }

		return InputCode();
	}

	#pragma region マウス情報
	[[nodiscard]] Vector2D<int>   GetMousePos	   (const TimeKind time_kind) const { return m_mouse_data.at(time_kind).pos; }
	[[nodiscard]] Vector2D<float> GetMouseDir	   (const TimeKind time_kind) const { return m_mouse_data.at(time_kind).dir; }
	[[nodiscard]] Vector2D<float> GetMouseVelocity (const TimeKind time_kind) const { return m_mouse_data.at(time_kind).velocity; }
	#pragma endregion


	/// @brief 入力判定
	/// @brief キー入力以外はenum classの定義を使用する必要あり
	template<input_concepts::InputT InputT>
	[[nodiscard]] bool IsInput(const InputT&	input_code)
	{
		return IsInput(ConvertInputTemplateToInputCode(input_code));
	}
	[[nodiscard]] bool IsInput(const InputCode& input_code);

	/// @brief 入力パラメータを取得
	/// @brief キー入力以外はenum classの定義を使用する必要あり
	template<input_concepts::ParameterT InputT>
	[[nodiscard]] int GetInputParameter(const InputT&	 input_code)
	{
		return GetInputParameter(ConvertInputTemplateToInputCode(input_code));
	}
	[[nodiscard]] int GetInputParameter(const InputCode& input_code) const;

	/// @brief 入力時間を取得
	/// @brief キー入力以外はenum classの定義を使用する必要あり
	template<input_concepts::InputT InputT>
	[[nodiscard]] float GetInputTime(const InputT&    input_code, const TimeKind time_kind)
	{
		return GetInputTime(ConvertInputTemplateToInputCode(input_code), time_kind);
	}
	[[nodiscard]] float GetInputTime(const InputCode& input_code, const TimeKind time_kind);

	/// @brief 入力状態を取得
	/// @brief キー入力以外はenum classの定義を使用する必要あり
	template<input_concepts::InputT InputT>
	[[nodiscard]] InputState GetInputState(const InputT&    input_code)
	{
		return GetInputState(ConvertInputTemplateToInputCode(input_code));
	}
	[[nodiscard]] InputState GetInputState(const InputCode& input_code);

	/// @brief 入力テンプレート値から入力の種類を取得
	template<input_concepts::InputT InputT>
	[[nodiscard]] InputKind GetInputKind(const InputT& input_code)
	{
		if (std::is_same_v<int,					InputT>) { return InputKind::kKey;			}
		if (std::is_same_v<mouse::ButtonKind,	InputT>) { return InputKind::kMouseButton;	}
		if (std::is_same_v<mouse::WheelKind,	InputT>) { return InputKind::kMouseWheel;	}
		if (std::is_same_v<mouse::SlideDirKind, InputT>) { return InputKind::kMouseSlide;	}
		if (std::is_same_v<pad::ButtonKind,		InputT>) { return InputKind::kPadButton;	}
		if (std::is_same_v<pad::TriggerKind,	InputT>) { return InputKind::kPadTrigger;	}
		if (std::is_same_v<pad::StickKind,		InputT>) { return InputKind::kPadStick;		}

		return InputKind::kKey;
	}

	/// @brief 現在の入力デバイスを取得	
	/// @brief キーボードとパッド両方が入力された場合はキーボードを優先
	[[nodiscard]] DeviceKind GetCurrentInputDevice() const { return m_current_device; }

private:
	InputChecker();
	~InputChecker() override;

	void UpdateMouse();

	/// @brief マウスカーソルを画面中央に固定する
	void LockCursor();

	/// @brief 入力情報の追加
	/// @param kind 入力の種類
	/// @param input_code_num kindが持つ入力コード数
	void AddInputData(const InputKind kind, const int input_code_num);

	void CalcMouseDir();
	void CalcMouseVelocity();

	/// @brief すべての入力時間を計測する
	void CountInputTimeAll();

	/// @brief すべての入力判定を行う
	void CheckInputAll();

	/// @brief 入力データを移行する
	void ShiftDataCureentToPrev();

	/// @brief 現在の入力デバイスを検出
	void DetectCurrentInputDevice();

public:
	static constexpr short			kStickMaxSlope		= SHRT_MAX;		// 傾きの最大値
	static constexpr short			kStickMinSlope		= SHRT_MIN;		// 傾きの最小値
	static constexpr short			kStickDeadZone		= 10000;		// スティック入力無効範囲(指定可能範囲 : -32768～32767)
	static constexpr unsigned char	kTriggerDeadZone	= 50;			// トリガー入力無効範囲  (指定可能範囲 : 0～UCHAR_MAX)

private:
	static constexpr int			kMouseButtonNum		= 8;
	static constexpr int			kMouseWheelNum		= 2;
	static constexpr int			kMouseSlideNum		= 4;
	static constexpr int			kPadButtonNum		= 16;
	static constexpr int			kPadTriggerNum		= 2;
	static constexpr int			kPadStickNum		= 8;

	XINPUT_STATE					m_xinput;	// TODO : 複数コントローラーに対応できるよう後にコンテナ化
	DeviceKind						m_current_device;
	bool							m_is_lock_mouse_pos;
	int								m_key_input[256];

	std::unordered_map<InputKey, InputValue, KeyHash, KeyEquality> m_input_data;
	std::unordered_map<TimeKind, MouseData> m_mouse_data;
	
	friend SingletonBase<InputChecker>;
};
