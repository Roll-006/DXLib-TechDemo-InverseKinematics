#pragma once
#include "../Base/singleton_base.hpp"
#include "../Input/input_checker.hpp"

class Debugger final : public SingletonBase<Debugger>
{
public:
	void Update();
	void Draw() const;

	[[nodiscard]] bool IsUsingDebug() const { return m_is_using_debug; }

private:
	Debugger();
	~Debugger() override;

	void JudgeUseDebugMode();

private:
	bool m_is_using_debug;
	int  m_debug_count;

	friend SingletonBase<Debugger>;
};
