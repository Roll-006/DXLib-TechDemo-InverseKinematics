#include "input_checker.hpp"
#include "../Command/command_handler.hpp"

InputChecker::InputChecker():
	m_xinput				(-1),
	m_current_device		(DeviceKind::kKeyboard),
	m_is_lock_mouse_pos		(true),
	m_key_input				(0)
{	
	SetUseDirectInputFlag(TRUE);
	SetMouseDispFlag(FALSE);

	m_mouse_data[TimeKind::kPrev].pos	    = m_mouse_data[TimeKind::kCurrent].pos		= Window::kCenterPos;
	m_mouse_data[TimeKind::kPrev].dir		= m_mouse_data[TimeKind::kCurrent].dir		= Vector2D<float>(0.0f, 0.0f);
	m_mouse_data[TimeKind::kPrev].velocity  = m_mouse_data[TimeKind::kCurrent].velocity = Vector2D<float>(0.0f, 0.0f);

	LockCursor();

	const auto key_codes = CommandHandler::GetInstance()->GetKeyInputCodes();
	for (const auto& [kind, input_codes] : key_codes)
	{
		for (const auto& code : input_codes)
		{
			m_input_data[{ code.second, TimeKind::kPrev		}]	= InputData();
			m_input_data[{ code.second, TimeKind::kCurrent	}]	= InputData();
		}
	}

	AddInputData(InputKind::kMouseButton, kMouseButtonNum);
	AddInputData(InputKind::kMouseWheel,  kMouseWheelNum);
	AddInputData(InputKind::kMouseSlide,  kMouseSlideNum);
	AddInputData(InputKind::kPadButton,   kPadButtonNum);
	AddInputData(InputKind::kPadTrigger,  kPadTriggerNum);
	AddInputData(InputKind::kPadStick,    kPadStickNum);
}

InputChecker::~InputChecker()
{
	// 処理なし
}

void InputChecker::Update()
{
	ShiftDataCureentToPrev();

	UpdateMouse();

	GetJoypadXInputState(DX_INPUT_PAD1, &m_xinput);

	CheckInputAll();
	CountInputTimeAll();

	DetectCurrentInputDevice();
}

void InputChecker::LateUpdate()
{
	LockCursor();
}

void InputChecker::UpdateMouse()
{
	GetMousePoint(&m_mouse_data.at(TimeKind::kCurrent).pos.x, &m_mouse_data.at(TimeKind::kCurrent).pos.y);
	m_mouse_data.at(TimeKind::kCurrent).wheel_rotation = GetMouseWheelRotVol();
	CalcMouseVelocity();
	CalcMouseDir();
}

void InputChecker::LockCursor()
{
	if (!m_is_lock_mouse_pos) { return; }

	m_mouse_data.at(TimeKind::kPrev).pos = m_mouse_data.at(TimeKind::kCurrent).pos = Window::kCenterPos;
	SetMousePoint(m_mouse_data.at(TimeKind::kCurrent).pos.x, m_mouse_data.at(TimeKind::kCurrent).pos.y);
}

bool InputChecker::IsInput(const InputCode& input_code)
{
	switch (input_code.kind)
	{
	case InputKind::kKey:
		return m_key_input[input_code.code] >= 1;
		break;

	case InputKind::kMouseButton:
		return (GetMouseInput() & input_code.code) != 0;
		break;

	case InputKind::kMouseWheel:
		return GetInputParameter(static_cast<mouse::WheelKind>(input_code.code));
		break;

	case InputKind::kMouseSlide:
		switch (static_cast<mouse::SlideDirKind>(input_code.code))
		{
		case mouse::SlideDirKind::kLeft:  if (m_mouse_data.at(TimeKind::kCurrent).pos.x < m_mouse_data.at(TimeKind::kPrev).pos.x) { return true; } break;
		case mouse::SlideDirKind::kRight: if (m_mouse_data.at(TimeKind::kCurrent).pos.x > m_mouse_data.at(TimeKind::kPrev).pos.x) { return true; } break;
		case mouse::SlideDirKind::kDown:  if (m_mouse_data.at(TimeKind::kCurrent).pos.y > m_mouse_data.at(TimeKind::kPrev).pos.y) { return true; } break;
		case mouse::SlideDirKind::kUp:    if (m_mouse_data.at(TimeKind::kCurrent).pos.y < m_mouse_data.at(TimeKind::kPrev).pos.y) { return true; } break;
		}
		break;

	case InputKind::kPadButton:
		return m_xinput.Buttons[input_code.code];
		break;

	case InputKind::kPadTrigger:
		return GetInputParameter(static_cast<pad::TriggerKind>(input_code.code));
		break;

	case InputKind::kPadStick:
		return GetInputParameter(static_cast<pad::StickKind>(input_code.code));
		break;
	}
	return false;
}

int InputChecker::GetInputParameter(const InputCode& input_code) const
{
	int rota = 0;

	switch (input_code.kind)
	{
	case InputKind::kMouseWheel:
		rota = m_mouse_data.at(TimeKind::kCurrent).wheel_rotation;
		switch (static_cast<mouse::WheelKind>(input_code.code))
		{
		case mouse::WheelKind::kUp:		return rota > 0 ? rota : 0;	break;
		case mouse::WheelKind::kDown:	return rota < 0 ? rota : 0;	break;
		}
		break;

	case InputKind::kPadTrigger:
		switch (static_cast<pad::TriggerKind>(input_code.code))
		{
		case pad::TriggerKind::kLT: if (m_xinput.LeftTrigger  > kTriggerDeadZone)	{ return m_xinput.LeftTrigger ; } break;
		case pad::TriggerKind::kRT: if (m_xinput.RightTrigger > kTriggerDeadZone)	{ return m_xinput.RightTrigger; } break;
		}
		break;

	case InputKind::kPadStick:
		switch (static_cast<pad::StickKind>(input_code.code))
		{
		case pad::StickKind::kLSLeft:  if (m_xinput.ThumbLX < -kStickDeadZone) { return m_xinput.ThumbLX; } break;
		case pad::StickKind::kLSRight: if (m_xinput.ThumbLX >  kStickDeadZone) { return m_xinput.ThumbLX; } break;
		case pad::StickKind::kLSDown:  if (m_xinput.ThumbLY < -kStickDeadZone) { return m_xinput.ThumbLY; } break;
		case pad::StickKind::kLSUp:    if (m_xinput.ThumbLY >  kStickDeadZone) { return m_xinput.ThumbLY; } break;
		case pad::StickKind::kRSLeft:  if (m_xinput.ThumbRX < -kStickDeadZone) { return m_xinput.ThumbRX; } break;
		case pad::StickKind::kRSRight: if (m_xinput.ThumbRX >  kStickDeadZone) { return m_xinput.ThumbRX; } break;
		case pad::StickKind::kRSDown:  if (m_xinput.ThumbRY < -kStickDeadZone) { return m_xinput.ThumbRY; } break;
		case pad::StickKind::kRSUp:	   if (m_xinput.ThumbRY >  kStickDeadZone) { return m_xinput.ThumbRY; } break;
		}
		break;

	default:
		break;
	}
	return 0;
}

float InputChecker::GetInputTime(const InputCode& input_code, const TimeKind time_kind)
{
	return m_input_data.at({ input_code, time_kind }).input_time;
}

InputState InputChecker::GetInputState(const InputCode& input_code)
{
	// キー入力の場合はDxLib既存関数を使用する
	if (input_code.kind == InputKind::kKey)
	{
		if (m_key_input[input_code.code] == 0)
		{
			return InputState::kNone;
		}
		else if (m_key_input[input_code.code] == 1)
		{
			return InputState::kSingle;
		}
		else if (m_key_input[input_code.code] >= 2)
		{
			return InputState::kHold;
		}
		else
		{
			return InputState::kPrev;
		}
	}

	bool prev_is_input	  = false;
	bool current_is_input = false;

	// prev を検索
	if (m_input_data.count({ input_code, TimeKind::kPrev }))
	{
		prev_is_input = m_input_data.at({ input_code, TimeKind::kPrev }).is_input;
	}
	// current を検索
	if (m_input_data.count({ input_code, TimeKind::kCurrent }))
	{
		current_is_input = m_input_data.at({ input_code, TimeKind::kCurrent }).is_input;
	}

	// 状態を判定
	if (current_is_input)
	{
		return prev_is_input ? InputState::kHold : InputState::kSingle;
	}
	return prev_is_input ? InputState::kPrev : InputState::kNone;
}

void InputChecker::AddInputData(const InputKind kind, const int input_code_num)
{
	for (int i = 0; i < input_code_num; ++i)
	{
		// 中身は空で追加
		m_input_data[{ InputCode(kind, i), TimeKind::kPrev	  }] = InputData();
		m_input_data[{ InputCode(kind, i), TimeKind::kCurrent }] = InputData();
	}
}

void InputChecker::CalcMouseDir()
{
	m_mouse_data.at(TimeKind::kCurrent).dir = v2d::GetNormalizedV(m_mouse_data.at(TimeKind::kCurrent).velocity);
}

void InputChecker::CalcMouseVelocity()
{
	const Vector2D<int> distance = m_mouse_data.at(TimeKind::kCurrent).pos - m_mouse_data.at(TimeKind::kPrev).pos;
	m_mouse_data.at(TimeKind::kCurrent).velocity = v2d::ConvertVecType<float>(distance);
}

void InputChecker::CountInputTimeAll()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kNoneScale);

	for (auto& [key, data] : m_input_data)
	{
		// 過去フレームはスキップ
		if (key.second == TimeKind::kPrev) { continue; }

		if (data.is_input)
		{
			data.input_time += delta_time;
		}
		else
		{
			data.input_time = 0.0f;
		}
	}
}

void InputChecker::CheckInputAll()
{
	GetHitKeyStateAllEx(m_key_input);

	for (auto& [key, data] : m_input_data)
	{
		if (key.second != TimeKind::kCurrent) { continue; }

		switch (key.first.kind)
		{
		case InputKind::kKey:
			data.is_input = IsInput(key.first.code) ? true : false;
			break;

		case InputKind::kMouseButton:
			data.is_input = IsInput(static_cast<mouse::ButtonKind>(key.first.code))		? true : false;
			break;

		case InputKind::kMouseWheel:
			data.is_input = IsInput(static_cast<mouse::WheelKind>(key.first.code))		? true : false;
			break;

		case InputKind::kMouseSlide:
			data.is_input = IsInput(static_cast<mouse::SlideDirKind>(key.first.code))	? true : false;
			break;

		case InputKind::kPadButton:
			data.is_input = IsInput(static_cast<pad::ButtonKind>(key.first.code))		? true : false;
			break;

		case InputKind::kPadTrigger:
			data.is_input = IsInput(static_cast<pad::TriggerKind>(key.first.code))		? true : false;
			break;

		case InputKind::kPadStick:
			data.is_input = IsInput(static_cast<pad::StickKind>(key.first.code))		? true : false;
			break;
		}
	}
}

void InputChecker::ShiftDataCureentToPrev()
{
	// マウス入力はそのままシフト
	m_mouse_data.at(TimeKind::kPrev) = m_mouse_data.at(TimeKind::kCurrent);

	// キーボード / パッド入力のシフト
	for (auto& [key, data] : m_input_data)
	{
		if (key.second != TimeKind::kCurrent) continue;

		// Prev 側を直接参照してコピー
		m_input_data.at({ key.first, TimeKind::kPrev }) = data;
	}
}

void InputChecker::DetectCurrentInputDevice()
{
	for (const auto& [key, data] : m_input_data)
	{
		// 今フレームの入力だけ見る
		if (key.second != TimeKind::kCurrent)	{ continue; }
		if (!data.is_input)						{ continue; }

		switch (key.first.kind)
		{
		case InputKind::kKey:
		case InputKind::kMouseButton:
		case InputKind::kMouseWheel:
			m_current_device = DeviceKind::kKeyboard;
			return;

		case InputKind::kPadButton:
		case InputKind::kPadTrigger:
		case InputKind::kPadStick:
			m_current_device = DeviceKind::kPad;
			return;

		default:
			break;
		}
	}
}
