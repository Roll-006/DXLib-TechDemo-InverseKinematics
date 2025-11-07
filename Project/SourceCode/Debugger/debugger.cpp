#include "debugger.hpp"

Debugger::Debugger() : 
	m_is_using_debug(false),
	m_debug_count	(m_is_using_debug ? 1 : 0)
{

}

Debugger::~Debugger()
{

}

void Debugger::JudgeUseDebugMode()
{
	const auto input = InputChecker::GetInstance();
	if (input->IsInput(KEY_INPUT_LSHIFT) && input->IsInput(KEY_INPUT_SPACE) && input->GetInputState(KEY_INPUT_RETURN) == InputState::kSingle)
	{
		++m_debug_count;
		m_is_using_debug = m_debug_count % 2 == 0 ? true : false;
	}
}

void Debugger::Update()
{
	JudgeUseDebugMode();
}

void Debugger::Draw() const
{
	if (!m_is_using_debug) { return; }
}
