#pragma once

enum class InputKind
{
	kKey,
	kMouseButton,
	kMouseWheel,
	kMouseSlide,
	kPadButton,
	kPadTrigger,
	kPadStick,
};

enum class InputState
{
	kNone,		// 入力なし
	kPrev,		// 1フレーム前に入力された
	kSingle,	// 単押し
	kHold,		// 長押し
};

enum class InputModeKind
{
	kSingle,
	kTrigger,
	kHold,
};

enum class InputDirKind
{
	kLeft,
	kRight,
	kUp,
	kDown,
};