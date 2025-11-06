#pragma once
#include <DxLib.h>
#include "../Calculation/vector_3d.hpp"

/// @brief XYZŽ²
struct Axes
{
	VECTOR x_axis;
	VECTOR y_axis;
	VECTOR z_axis;
};

namespace axis
{
	[[nodiscard]] inline VECTOR GetWorldXAxis() { return VGet(1.0f, 0.0f, 0.0f); }
	[[nodiscard]] inline VECTOR GetWorldYAxis() { return VGet(0.0f, 1.0f, 0.0f); }
	[[nodiscard]] inline VECTOR GetWorldZAxis() { return VGet(0.0f, 0.0f, 1.0f); }
	[[nodiscard]] inline Axes   GetWorldAxes () { return Axes(GetWorldXAxis(), GetWorldYAxis(), GetWorldZAxis()); }

	inline void Draw(const Axes& axes, const VECTOR& begin_pos, const float length)
	{
		DrawLine3D(begin_pos, begin_pos + axes.x_axis * length, GetColor(UCHAR_MAX,   0,   0));
		DrawLine3D(begin_pos, begin_pos + axes.y_axis * length, GetColor(  0, UCHAR_MAX,   0));
		DrawLine3D(begin_pos, begin_pos + axes.z_axis * length, GetColor(  0,   0, UCHAR_MAX));
	};
}


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, Axes& axes)
{
	data.at("x_axis").get_to(axes.x_axis);
	data.at("y_axis").get_to(axes.y_axis);
	data.at("z_axis").get_to(axes.z_axis);
}

inline void to_json(nlohmann::json& data, const Axes& axes)
{
	data = nlohmann::json
	{
		{ "x_axis",	axes.x_axis },
		{ "y_axis",	axes.y_axis },
		{ "z_axis",	axes.z_axis }
	};
}
#pragma endregion
