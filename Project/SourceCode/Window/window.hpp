#pragma once
#include <cassert>
#include <DxLib.h>

#include "../Base/single_instance_base.hpp"

#include "../GameTime/fps.hpp"
#include "../Input/input_checker.hpp"
#include <Vector/vector_2d.hpp>

class Window final : public SingleInstanceBase<Window>
{
public:
	Window();
	~Window();

	void Update();

private:
	void SetWindowMode();
	void SetWindowMode(const bool is_full_screen);

public:
	static constexpr Vector2D<float> kAspectRatio		= { 16.0f, 9.0f };
	static constexpr Vector2D<float> kCinemaScopeRatio	= { 2.35f, 1.0f };
	static constexpr Vector2D<int>	 kScreenSize		= { 1920, 1080 };
	static constexpr Vector2D<int>	 kScreenHalfSize	= { static_cast<int>(kScreenSize.x * 0.5f), static_cast<int>(kScreenSize.y * 0.5f) };
	static constexpr Vector2D<int>	 kCenterPos			= { kScreenHalfSize.x, kScreenHalfSize.y };

private:
	bool m_is_full_screen;
	int  m_change_window_count;
};
