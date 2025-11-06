#pragma once
#include <DxLib.h>

namespace pad
{
	enum class ButtonKind
	{
		kLeft	= XINPUT_BUTTON_DPAD_LEFT,
		kRight	= XINPUT_BUTTON_DPAD_RIGHT,
		kUp		= XINPUT_BUTTON_DPAD_UP,
		kDown	= XINPUT_BUTTON_DPAD_DOWN,
		kStart	= XINPUT_BUTTON_START,
		kBack	= XINPUT_BUTTON_BACK,
		kLSPush = XINPUT_BUTTON_LEFT_THUMB,
		kRSPush = XINPUT_BUTTON_RIGHT_THUMB,
		kLB		= XINPUT_BUTTON_LEFT_SHOULDER,
		kRB		= XINPUT_BUTTON_RIGHT_SHOULDER,
		kA		= XINPUT_BUTTON_A,
		kB		= XINPUT_BUTTON_B,
		kX		= XINPUT_BUTTON_X,
		kY		= XINPUT_BUTTON_Y,
	};

	enum class TriggerKind
	{
		kLT,
		kRT,
	};

	enum class StickKind
	{
		kLSLeft,	// 左スティック左方向
		kLSRight,	// 左スティック右方向
		kLSUp,		// 左スティック上方向
		kLSDown,	// 左スティック下方向
		kRSLeft,	// 右スティック左方向
		kRSRight,	// 右スティック右方向
		kRSUp,		// 右スティック上方向
		kRSDown,	// 右スティック下方向
	};
}
