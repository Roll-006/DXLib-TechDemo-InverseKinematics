#pragma once
#include <concepts>

namespace common_concepts
{
	/// @brief ®”Œ^‚Å‚ ‚é
	template<typename T>
	concept IntegerT = std::is_integral_v<T>;

	/// @brief •‚“®¬”“_”Œ^‚Å‚ ‚é
	template<typename T>
	concept FloatingPointT = std::is_floating_point_v<T>;

	/// @brief ZpŒ^‚Å‚ ‚é
	template<typename T>
	concept ArithmeticT = std::is_arithmetic_v<T>;

	/// @brief enum‚à‚µ‚­‚Íenum classŒ^‚Å‚ ‚é
	template<typename T>
	concept EnumT = std::is_enum_v<T>;
}
