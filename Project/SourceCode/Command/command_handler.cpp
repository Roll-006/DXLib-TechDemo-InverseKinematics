#include "command_handler.hpp"

CommandHandler::CommandHandler()
{
	m_trigger_count[TimeKind::kCurrent][CommandKind::kRun]		= 0;
	m_trigger_count[TimeKind::kCurrent][CommandKind::kCrouch]	= 0;

	// 初期設定
	InitKeyCommand();
	InitPadCommand();
	InitInputMode();
}

CommandHandler::~CommandHandler()
{

}

void CommandHandler::Update()
{
	const auto input = InputChecker::GetInstance();

	m_execute_command[TimeKind::kPrev] = m_execute_command[TimeKind::kCurrent];
	m_execute_command[TimeKind::kCurrent].clear();

	m_trigger_count[TimeKind::kPrev] = m_trigger_count[TimeKind::kCurrent];

	// 現在の入力デバイスに合わせて処理を実行
	switch (input->GetCurrentInputDevice())
	{
	case DeviceKind::kKeyboard:
		TryExecuteCommand(m_key_codes);
		break;

	case DeviceKind::kPad:
		TryExecuteCommand(m_pad_codes);
		break;
	}
}

void CommandHandler::InitKeyCommand()
{
	m_key_codes.clear();

	// TODO : 後にJson化
	AddInputCode(CommandKind::kDecide,						CommandSlotKind::kStatic1,	KEY_INPUT_SPACE);
	AddInputCode(CommandKind::kDecide,						CommandSlotKind::kStatic2,	KEY_INPUT_RETURN);
	AddInputCode(CommandKind::kBack,						CommandSlotKind::kStatic1,	mouse::ButtonKind::kRight);
	AddInputCode(CommandKind::kSelectUp,					CommandSlotKind::kStatic1,	KEY_INPUT_W);
	AddInputCode(CommandKind::kSelectUp,					CommandSlotKind::kStatic2,	KEY_INPUT_UP);
	AddInputCode(CommandKind::kSelectUp,					CommandSlotKind::kStatic3,	mouse::WheelKind::kUp);
	AddInputCode(CommandKind::kSelectDown,					CommandSlotKind::kStatic1,	KEY_INPUT_S);
	AddInputCode(CommandKind::kSelectDown,					CommandSlotKind::kStatic2,	KEY_INPUT_DOWN);
	AddInputCode(CommandKind::kSelectDown,					CommandSlotKind::kStatic3,	mouse::WheelKind::kDown);
	AddInputCode(CommandKind::kSelectLeft,					CommandSlotKind::kStatic1,	KEY_INPUT_A);
	AddInputCode(CommandKind::kSelectLeft,					CommandSlotKind::kStatic2,	KEY_INPUT_LEFT);
	AddInputCode(CommandKind::kSelectRight,					CommandSlotKind::kStatic1,	KEY_INPUT_D);
	AddInputCode(CommandKind::kSelectRight,					CommandSlotKind::kStatic2,	KEY_INPUT_RIGHT);
	AddInputCode(CommandKind::kPause,						CommandSlotKind::kStatic1,	KEY_INPUT_ESCAPE);

	AddInputCode(CommandKind::kAimGun,						CommandSlotKind::kMain,		mouse::ButtonKind::kRight);
	AddInputCode(CommandKind::kAttack,						CommandSlotKind::kMain,		mouse::ButtonKind::kLeft);
	AddInputCode(CommandKind::kPullTrigger,					CommandSlotKind::kMain,		mouse::ButtonKind::kLeft);
	AddInputCode(CommandKind::kAimKnife,					CommandSlotKind::kMain,		KEY_INPUT_SPACE);
	AddInputCode(CommandKind::kInvestigate,					CommandSlotKind::kMain,		KEY_INPUT_F);
	AddInputCode(CommandKind::kMelee,						CommandSlotKind::kMain,		KEY_INPUT_F);
	AddInputCode(CommandKind::kEscape,						CommandSlotKind::kMain,		KEY_INPUT_F);
	AddInputCode(CommandKind::kReload,						CommandSlotKind::kMain,		KEY_INPUT_R);
	AddInputCode(CommandKind::kScope,						CommandSlotKind::kMain,		KEY_INPUT_LALT);
	AddInputCode(CommandKind::kMoveUpPlayer,				CommandSlotKind::kMain,		KEY_INPUT_W);
	AddInputCode(CommandKind::kMoveDownPlayer,				CommandSlotKind::kMain,		KEY_INPUT_S);
	AddInputCode(CommandKind::kMoveLeftPlayer,				CommandSlotKind::kMain,		KEY_INPUT_A);
	AddInputCode(CommandKind::kMoveRightPlayer,				CommandSlotKind::kMain,		KEY_INPUT_D);
	AddInputCode(CommandKind::kRun,							CommandSlotKind::kMain,		KEY_INPUT_LSHIFT);
	AddInputCode(CommandKind::kCrouch,						CommandSlotKind::kMain,		KEY_INPUT_E);
	AddInputCode(CommandKind::kQuickTurn,					CommandSlotKind::kMain,		KEY_INPUT_Q);

	AddInputCode(CommandKind::kMoveUpCamera,				CommandSlotKind::kMain,		KEY_INPUT_UP);
	AddInputCode(CommandKind::kMoveUpCamera,				CommandSlotKind::kStatic1,	mouse::SlideDirKind::kUp);
	AddInputCode(CommandKind::kMoveDownCamera,				CommandSlotKind::kMain,		KEY_INPUT_DOWN);
	AddInputCode(CommandKind::kMoveDownCamera,				CommandSlotKind::kStatic1,	mouse::SlideDirKind::kDown);
	AddInputCode(CommandKind::kMoveLeftCamera,				CommandSlotKind::kMain,		KEY_INPUT_LEFT);
	AddInputCode(CommandKind::kMoveLeftCamera,				CommandSlotKind::kStatic1,	mouse::SlideDirKind::kLeft);
	AddInputCode(CommandKind::kMoveRightCamera,				CommandSlotKind::kMain,		KEY_INPUT_RIGHT);
	AddInputCode(CommandKind::kMoveRightCamera,				CommandSlotKind::kStatic1,	mouse::SlideDirKind::kRight);

	AddInputCode(CommandKind::kSelectWeaponInsideLeft,		CommandSlotKind::kMain,		KEY_INPUT_2);
	AddInputCode(CommandKind::kSelectWeaponInsideUp,		CommandSlotKind::kMain,		KEY_INPUT_1);
	AddInputCode(CommandKind::kSelectWeaponInsideRight,		CommandSlotKind::kMain,		KEY_INPUT_4);
	AddInputCode(CommandKind::kSelectWeaponInsideDown,		CommandSlotKind::kMain,		KEY_INPUT_3);
	AddInputCode(CommandKind::kSelectWeaponOutsideLeft,		CommandSlotKind::kMain,		KEY_INPUT_6);
	AddInputCode(CommandKind::kSelectWeaponOutsideUp,		CommandSlotKind::kMain,		KEY_INPUT_5);
	AddInputCode(CommandKind::kSelectWeaponOutsideRight,	CommandSlotKind::kMain,		KEY_INPUT_8);
	AddInputCode(CommandKind::kSelectWeaponOutsideDown,		CommandSlotKind::kMain,		KEY_INPUT_7);
	AddInputCode(CommandKind::kSelectWeaponRotateLeft,		CommandSlotKind::kMain,		mouse::WheelKind ::kUp);
	AddInputCode(CommandKind::kSelectWeaponRotateRight,		CommandSlotKind::kMain,		mouse::WheelKind ::kDown);
	AddInputCode(CommandKind::kSideChangeWeapon,			CommandSlotKind::kMain,		mouse::ButtonKind::kMiddle);

	AddInputCode(CommandKind::kChangeDebugMode,				CommandSlotKind::kStatic1,	KEY_INPUT_RETURN);
	AddInputCode(CommandKind::kDebugTrigger1,				CommandSlotKind::kStatic1,	KEY_INPUT_LSHIFT);
	AddInputCode(CommandKind::kDebugTrigger2,				CommandSlotKind::kStatic1,	KEY_INPUT_D);
}

void CommandHandler::InitPadCommand()
{
	m_pad_codes.clear();

	// TODO : 後にJson化
	AddInputCode(CommandKind::kSelectUp,					CommandSlotKind::kStatic1,	pad::ButtonKind	::kUp);
	AddInputCode(CommandKind::kBack,						CommandSlotKind::kStatic1,	pad::ButtonKind	::kB);
	AddInputCode(CommandKind::kDecide,						CommandSlotKind::kStatic1,	pad::ButtonKind	::kA);
	AddInputCode(CommandKind::kSelectUp,					CommandSlotKind::kStatic1,	pad::ButtonKind	::kUp);
	AddInputCode(CommandKind::kSelectUp,					CommandSlotKind::kStatic2,	pad::StickKind	::kLSUp);
	AddInputCode(CommandKind::kSelectDown,					CommandSlotKind::kStatic1,	pad::ButtonKind	::kDown);
	AddInputCode(CommandKind::kSelectDown,					CommandSlotKind::kStatic2,	pad::StickKind	::kLSDown);
	AddInputCode(CommandKind::kSelectLeft,					CommandSlotKind::kStatic1,	pad::ButtonKind	::kLeft);
	AddInputCode(CommandKind::kSelectLeft,					CommandSlotKind::kStatic2,	pad::StickKind	::kLSLeft);
	AddInputCode(CommandKind::kSelectRight,					CommandSlotKind::kStatic1,	pad::ButtonKind	::kRight);
	AddInputCode(CommandKind::kSelectRight,					CommandSlotKind::kStatic2,	pad::StickKind	::kLSRight);
	AddInputCode(CommandKind::kPause,						CommandSlotKind::kStatic1,	pad::ButtonKind	::kStart);

	AddInputCode(CommandKind::kAimGun,						CommandSlotKind::kStatic1,	pad::TriggerKind::kLT);
	AddInputCode(CommandKind::kAttack,						CommandSlotKind::kStatic1,	pad::TriggerKind::kRT);
	AddInputCode(CommandKind::kPullTrigger,					CommandSlotKind::kStatic1,	pad::TriggerKind::kRT);
	AddInputCode(CommandKind::kAimKnife,					CommandSlotKind::kStatic1,	pad::ButtonKind	::kLB);
	AddInputCode(CommandKind::kInvestigate,					CommandSlotKind::kStatic1,	pad::ButtonKind	::kA);
	AddInputCode(CommandKind::kMelee,						CommandSlotKind::kStatic1,	pad::ButtonKind	::kA);
	AddInputCode(CommandKind::kEscape,						CommandSlotKind::kStatic1,	pad::ButtonKind	::kA);
	AddInputCode(CommandKind::kReload,						CommandSlotKind::kStatic1,	pad::ButtonKind	::kX);
	AddInputCode(CommandKind::kScope,						CommandSlotKind::kStatic1,	pad::ButtonKind	::kLSPush);
	AddInputCode(CommandKind::kMoveUpPlayer,				CommandSlotKind::kStatic1,	pad::StickKind	::kLSUp);
	AddInputCode(CommandKind::kMoveDownPlayer,				CommandSlotKind::kStatic1,	pad::StickKind	::kLSDown);
	AddInputCode(CommandKind::kMoveLeftPlayer,				CommandSlotKind::kStatic1,	pad::StickKind	::kLSLeft);
	AddInputCode(CommandKind::kMoveRightPlayer,				CommandSlotKind::kStatic1,	pad::StickKind	::kLSRight);
	AddInputCode(CommandKind::kRun,							CommandSlotKind::kStatic1,	pad::ButtonKind	::kLSPush);
	AddInputCode(CommandKind::kRun,							CommandSlotKind::kStatic1,	pad::ButtonKind	::kRB);
	AddInputCode(CommandKind::kCrouch,						CommandSlotKind::kStatic1,	pad::ButtonKind	::kB);
	AddInputCode(CommandKind::kQuickTurn,					CommandSlotKind::kStatic1,	pad::ButtonKind	::kRB);

	AddInputCode(CommandKind::kMoveUpCamera,				CommandSlotKind::kStatic1,	pad::StickKind	::kRSUp);
	AddInputCode(CommandKind::kMoveDownCamera,				CommandSlotKind::kStatic1,	pad::StickKind	::kRSDown);
	AddInputCode(CommandKind::kMoveLeftCamera,				CommandSlotKind::kStatic1,	pad::StickKind	::kRSLeft);
	AddInputCode(CommandKind::kMoveRightCamera,				CommandSlotKind::kStatic1,	pad::StickKind	::kRSRight);
	AddInputCode(CommandKind::kInitAim,						CommandSlotKind::kStatic1,	pad::ButtonKind	::kRB);

	AddInputCode(CommandKind::kSelectWeaponLeft,			CommandSlotKind::kStatic1,	pad::ButtonKind	::kLeft);
	AddInputCode(CommandKind::kSelectWeaponUp,				CommandSlotKind::kStatic1,	pad::ButtonKind	::kUp);
	AddInputCode(CommandKind::kSelectWeaponRight,			CommandSlotKind::kStatic1,	pad::ButtonKind	::kRight);
	AddInputCode(CommandKind::kSelectWeaponDown,			CommandSlotKind::kStatic1,	pad::ButtonKind	::kDown);

	AddInputCode(CommandKind::kChangeDebugMode,				CommandSlotKind::kStatic1,	pad::ButtonKind	::kBack);
	AddInputCode(CommandKind::kDebugTrigger1,				CommandSlotKind::kStatic1,	pad::TriggerKind::kLT);
	AddInputCode(CommandKind::kDebugTrigger2,				CommandSlotKind::kStatic1,	pad::TriggerKind::kRT);
}

void CommandHandler::InitInputMode()
{
	m_input_mode[CommandKind::kDecide]						= InputModeKind::kSingle;
	m_input_mode[CommandKind::kBack]						= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectUp]					= InputModeKind::kHold;
	m_input_mode[CommandKind::kSelectDown]					= InputModeKind::kHold;
	m_input_mode[CommandKind::kSelectLeft]					= InputModeKind::kHold;
	m_input_mode[CommandKind::kSelectRight]					= InputModeKind::kHold;
	m_input_mode[CommandKind::kPause]						= InputModeKind::kSingle;
	m_input_mode[CommandKind::kAimGun]						= InputModeKind::kHold;
	m_input_mode[CommandKind::kAttack]						= InputModeKind::kSingle;
	m_input_mode[CommandKind::kPullTrigger]					= InputModeKind::kHold;
	m_input_mode[CommandKind::kAimKnife]					= InputModeKind::kHold;
	m_input_mode[CommandKind::kInvestigate]					= InputModeKind::kSingle;
	m_input_mode[CommandKind::kMelee]						= InputModeKind::kSingle;
	m_input_mode[CommandKind::kEscape]						= InputModeKind::kSingle;
	m_input_mode[CommandKind::kReload]						= InputModeKind::kSingle;
	m_input_mode[CommandKind::kScope]						= InputModeKind::kSingle;
	m_input_mode[CommandKind::kMoveUpPlayer]				= InputModeKind::kHold;
	m_input_mode[CommandKind::kMoveDownPlayer]				= InputModeKind::kHold;
	m_input_mode[CommandKind::kMoveLeftPlayer]				= InputModeKind::kHold;
	m_input_mode[CommandKind::kMoveRightPlayer]				= InputModeKind::kHold;
	m_input_mode[CommandKind::kRun]							= InputModeKind::kTrigger;
	m_input_mode[CommandKind::kCrouch]						= InputModeKind::kTrigger;
	m_input_mode[CommandKind::kQuickTurn]					= InputModeKind::kSingle;
	m_input_mode[CommandKind::kMoveUpCamera]				= InputModeKind::kHold;
	m_input_mode[CommandKind::kMoveDownCamera]				= InputModeKind::kHold;
	m_input_mode[CommandKind::kMoveLeftCamera]				= InputModeKind::kHold;
	m_input_mode[CommandKind::kMoveRightCamera]				= InputModeKind::kHold;
	m_input_mode[CommandKind::kInitAim]						= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponInsideLeft]		= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponInsideUp]		= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponInsideRight]		= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponInsideDown]		= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponOutsideLeft]		= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponOutsideUp]		= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponOutsideRight]	= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponOutsideDown]		= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponRotateLeft]		= InputModeKind::kHold;
	m_input_mode[CommandKind::kSelectWeaponRotateRight]		= InputModeKind::kHold;
	m_input_mode[CommandKind::kSideChangeWeapon]			= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponLeft]			= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponUp]				= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponRight]			= InputModeKind::kSingle;
	m_input_mode[CommandKind::kSelectWeaponDown]			= InputModeKind::kSingle;

	m_input_mode[CommandKind::kChangeDebugMode]				= InputModeKind::kTrigger;
	m_input_mode[CommandKind::kDebugTrigger1]				= InputModeKind::kHold;
	m_input_mode[CommandKind::kDebugTrigger2]				= InputModeKind::kHold;
}

void CommandHandler::InitCurrentTriggerInputCount(const CommandKind kind)
{
	m_trigger_count.at(TimeKind::kCurrent).at(kind) = 0;
}


#pragma region Getter
InputModeKind CommandHandler::GetInputModeKind(const CommandKind kind) const
{
	return m_input_mode.at(kind);
}

int CommandHandler::GetCurrentTriggerCount(const CommandKind kind) const
{
	return m_trigger_count.at(TimeKind::kCurrent).at(kind);
}

InputCode CommandHandler::GetKeyInputCode(const CommandKind kind, const CommandSlotKind slot) const
{
	return m_key_codes.at(kind).at(slot);
}

InputCode CommandHandler::GetPadInputCode(const CommandKind kind, const CommandSlotKind slot) const
{
	return m_pad_codes.at(kind).at(slot);
}

bool CommandHandler::IsExecute(const CommandKind command_kind, const TimeKind time_kind)
{
	const auto input  = InputChecker::GetInstance();
	const auto codes  = input->GetCurrentInputDevice() == DeviceKind::kKeyboard ? m_key_codes : m_pad_codes;
	bool is_executing = false;

	switch (m_input_mode.at(command_kind))
	{
	case InputModeKind::kSingle:
		for (const auto& [kind, input_codes] : codes)
		{
			if (kind == command_kind)
			{
				if (time_kind == TimeKind::kCurrent)
				{
					for (const auto& code : input_codes)
					{
						if (input->GetInputState(code.second) == InputState::kSingle)
						{
							return true;
						}
					}
					return false;
				}
				else if (time_kind == TimeKind::kPrev)
				{
					for (const auto& code : input_codes)
					{
						if (input->GetInputState(code.second) == InputState::kPrev)
						{
							return true;
						}
					}
					return false;
				}
			}			
		}	
		break;

	// トリガー方式の場合、入力カウントによって実行されたかを判定
	case InputModeKind::kTrigger:
		return m_trigger_count.at(time_kind).at(command_kind) % 2 == 1 ? true : false;
		break;

	case InputModeKind::kHold:
		return std::find(m_execute_command.at(time_kind).begin(), m_execute_command.at(time_kind).end(), command_kind) != m_execute_command.at(time_kind).end();
		break;
	}

	return false;
}
#pragma endregion


void CommandHandler::AddInputCode(const CommandKind kind, const CommandSlotKind slot, const input_concepts::InputT auto& input_code)
{
	const auto input	= InputChecker::GetInstance();
	const auto code		= input->ConvertInputTemplateToInputCode(input_code);
	std::unordered_map<CommandKind, std::unordered_map<CommandSlotKind, InputCode>>* codes = nullptr;
	
	switch (input->GetInputKind(input_code))
	{
	case InputKind::kKey:
	case InputKind::kMouseButton:
	case InputKind::kMouseWheel:
	case InputKind::kMouseSlide:
		codes = &m_key_codes;
		break;

	case InputKind::kPadButton:
	case InputKind::kPadTrigger:
	case InputKind::kPadStick:
		codes = &m_pad_codes;
		break;
	}

	// 新規データのみ追加する
	auto& set = (*codes)[kind];
	set.insert({ slot, code });
}

void CommandHandler::RemoveInputCode(const CommandKind kind, const CommandSlotKind slot, const input_concepts::InputT auto& input_code)
{
	const auto input = InputChecker::GetInstance();
	const auto code  = input->ConvertInputTemplateToInputCode(input_code);
	std::unordered_map<CommandKind, std::unordered_set<InputCode>>* codes = nullptr;

	switch (input->GetInputKind(input_code))
	{
	case InputKind::kKey:
	case InputKind::kMouseButton:
	case InputKind::kMouseWheel:
	case InputKind::kMouseSlide:
		codes = &m_key_codes;
		break;

	case InputKind::kPadButton:
	case InputKind::kPadTrigger:
	case InputKind::kPadStick:
		codes = &m_pad_codes;
		break;
	}

	// 削除
	auto itr = codes->find(kind);
	if (itr != codes->end())
	{
		itr->second.erase(code);
		if (itr->second.empty())
		{
			codes->erase(itr);
		}
	}
}

void CommandHandler::TryExecuteCommand(const std::unordered_map<CommandKind, std::unordered_map<CommandSlotKind, InputCode>>& codes)
{
	const auto input = InputChecker::GetInstance();

	// 入力された情報を保存
	std::vector<CommandKind> executed_command;

	for (const auto& [kind, input_codes] : codes)
	{
		for (const auto& code : input_codes)
		{
			if (input->IsInput(code.second))
			{
				// 入力されていないコマンドのみ格納
				if (std::find(executed_command.begin(), executed_command.end(), kind) == executed_command.end())
				{
					// 特殊コマンドのトリガー方式であった場合、入力回数をカウント
					if (m_input_mode.at(kind) == InputModeKind::kTrigger)
					{
						if (input->GetInputState(code.second) == InputState::kSingle)
						{

							++m_trigger_count[TimeKind::kCurrent][kind];
						}
					}
					executed_command.emplace_back(kind);
					m_execute_command[TimeKind::kCurrent].emplace_back(kind);
				}
			}
		}
	}
}
