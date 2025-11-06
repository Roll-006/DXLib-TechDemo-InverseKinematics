#pragma once
#include <DxLib.h>

namespace mouse
{
	enum class ButtonKind
	{
		kLeft	= MOUSE_INPUT_LEFT,
		kRight	= MOUSE_INPUT_RIGHT,
		kMiddle = MOUSE_INPUT_MIDDLE,
		k4		= MOUSE_INPUT_4,
		k5		= MOUSE_INPUT_5,
		k6		= MOUSE_INPUT_6,
		k7		= MOUSE_INPUT_7,
		k8		= MOUSE_INPUT_8,
	};

	enum class WheelKind
	{
		kUp,
		kDown,
	};

	enum class SlideDirKind
	{
		kLeft,
		kRight,
		kUp,
		kDown,
	};
}
