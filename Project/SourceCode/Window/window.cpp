#include "window.hpp"

Window::Window():
	m_is_full_screen		(true),
	m_change_window_count	(m_is_full_screen ? 1 : 0)
{
	SetWindowMode(m_is_full_screen);
}

Window::~Window()
{

}

void Window::Update()
{
	SetWindowMode();
}

void Window::SetWindowMode()
{
	const auto input = InputChecker::GetInstance();

	// [ALT + ENTER]
	const bool is_change = 
		input->IsInput(KEY_INPUT_LALT) &&
		input->GetInputState(KEY_INPUT_RETURN) == InputState::kSingle;

	if (is_change)
	{
		++m_change_window_count;
		m_is_full_screen = m_change_window_count % 2 == 0 ? true : false;
		ChangeWindowMode(m_is_full_screen ? FALSE : TRUE);
	}
}

void Window::SetWindowMode(const bool is_full_screen)
{
	++m_change_window_count;
	m_is_full_screen = is_full_screen;
	ChangeWindowMode(m_is_full_screen ? FALSE : TRUE);
}
