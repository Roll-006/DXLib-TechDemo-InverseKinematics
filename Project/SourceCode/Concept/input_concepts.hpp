#pragma once
#include <concepts>
#include "../IncludeList/input.hpp"

namespace input_concepts
{
	/// @brief マウス入力用の型である
	template<typename T>
	concept MouseT = std::is_same_v<mouse::ButtonKind,		T>
				  || std::is_same_v<mouse::WheelKind,		T>
				  || std::is_same_v<mouse::SlideDirKind,	T>;

	/// @brief キー&マウス用の型である
	template<typename T>
	concept KeyAndMouseT = std::is_same_v<int,					T>
						|| std::is_same_v<mouse::ButtonKind,	T>
						|| std::is_same_v<mouse::WheelKind,		T>
						|| std::is_same_v<mouse::SlideDirKind,	T>;

	/// @brief パッド入力用の型である
	template<typename T>
	concept PadT = std::is_same_v<pad::ButtonKind,	T>
				|| std::is_same_v<pad::TriggerKind,	T>
				|| std::is_same_v<pad::StickKind,	T>;

	/// @brief 入力用の型である
	template<typename T>
	concept InputT = std::is_same_v<int,					T>
				  || std::is_same_v<mouse::ButtonKind,		T>
				  || std::is_same_v<mouse::WheelKind,		T>
				  || std::is_same_v<mouse::SlideDirKind,	T>
				  || std::is_same_v<pad::ButtonKind,		T>
				  || std::is_same_v<pad::TriggerKind,		T>
				  || std::is_same_v<pad::StickKind,			T>;

	/// @brief パラメーターを取得する入力型である
	template<typename T>
	concept ParameterT = std::is_same_v<mouse::WheelKind,	T>
					  || std::is_same_v<pad::TriggerKind,	T>
					  || std::is_same_v<pad::StickKind,		T>;
}
