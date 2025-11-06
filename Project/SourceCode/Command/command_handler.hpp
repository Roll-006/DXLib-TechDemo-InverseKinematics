#pragma once
#include <unordered_set>

#include "../Kind/command_kind.hpp"
#include "../Base/singleton_base.hpp"
#include "../Input/input_checker.hpp"

class CommandHandler final : public SingletonBase<CommandHandler>
{
public:
	void Update();

	/// @brief コマンドを初期設定に戻す
	void InitKeyCommand();
	void InitPadCommand();
	void InitInputMode();

	/// @brief トリガーの入力回数をリセットする
	void InitCurrentTriggerInputCount(const CommandKind kind);

	/// @brief 特殊コマンドの入力モードを設定する
	/// @param kind コマンドの種類
	/// @param input_mode 入力モード
	void SetInputMode(const CommandKind kind, const InputModeKind input_mode) { m_input_mode.at(kind) = input_mode; }


	#pragma region Getter
	[[nodiscard]] InputModeKind GetInputModeKind		(const CommandKind kind) const;
	[[nodiscard]] int			GetCurrentTriggerCount	(const CommandKind kind) const;
	[[nodiscard]] std::unordered_map<CommandKind, std::unordered_map<CommandSlotKind, InputCode>> GetKeyInputCodes() const { return m_key_codes; }
	[[nodiscard]] std::unordered_map<CommandKind, std::unordered_map<CommandSlotKind, InputCode>> GetPadInputCodes() const { return m_pad_codes; }
	[[nodiscard]] InputCode		GetKeyInputCode			(const CommandKind kind, const CommandSlotKind slot) const;
	[[nodiscard]] InputCode		GetPadInputCode			(const CommandKind kind, const CommandSlotKind slot) const;

	/// @brief 指定のコマンドが実行状況を判定
	/// @brief command_kind 判定するコマンド
	/// @brief time_kind kCurrent : 現在実行中, kPrev : 1フレーム前に実行
	[[nodiscard]] bool IsExecute(const CommandKind command_kind, const TimeKind time_kind);
	#pragma endregion

private:
	CommandHandler();
	~CommandHandler() override;

	/// @brief コマンドに対応する入力コードの登録
	void AddInputCode(const CommandKind kind, const CommandSlotKind slot, const input_concepts::InputT auto& input_code);

	/// @brief コマンドに対応する入力コードの登録を解除
	void RemoveInputCode(const CommandKind kind, const CommandSlotKind slot, const input_concepts::InputT auto& input_code);

	/// @brief コマンドの実行判定
	void TryExecuteCommand(const std::unordered_map<CommandKind, std::unordered_map<CommandSlotKind, InputCode>>& codes);

private:
	std::unordered_map<TimeKind, std::vector<CommandKind>>							m_execute_command;		// 現在実行中のコマンド
	std::unordered_map<CommandKind, std::unordered_map<CommandSlotKind, InputCode>>	m_key_codes;
	std::unordered_map<CommandKind, std::unordered_map<CommandSlotKind, InputCode>>	m_pad_codes;

	std::unordered_map<CommandKind, InputModeKind>									m_input_mode;			// 入力モード
	std::unordered_map<TimeKind, std::unordered_map<CommandKind, int>>				m_trigger_count;		// トリガー方式入力カウント

	friend SingletonBase<CommandHandler>;
};
