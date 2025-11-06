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
	const auto command = CommandHandler::GetInstance();
	if (   command->IsExecute(CommandKind::kDebugTrigger1,   TimeKind::kCurrent)
		&& command->IsExecute(CommandKind::kDebugTrigger2,   TimeKind::kCurrent)
		&& command->IsExecute(CommandKind::kChangeDebugMode, TimeKind::kCurrent))
	{
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
