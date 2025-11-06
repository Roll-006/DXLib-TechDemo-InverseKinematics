#pragma once
#include "../Base/singleton_base.hpp"
#include "../Command/command_handler.hpp"

class Debugger final : public SingletonBase<Debugger>
{
public:
	void Update();
	void Draw() const;

private:
	Debugger();
	~Debugger() override;

private:
	bool m_is_using_debug_mode;

	friend SingletonBase<Debugger>;
};
