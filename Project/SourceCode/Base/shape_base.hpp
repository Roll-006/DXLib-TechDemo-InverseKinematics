#pragma once
#include <memory>
#include <DxLib.h>

#include "../Kind/shape_kind.hpp"
#include "../Calculation/math.hpp"

class ShapeBase abstract
{
public:
	ShapeBase(const ShapeKind shape_kind) : m_shape_kind(shape_kind){}

	virtual ~ShapeBase() = default;

	/// @brief •`‰æ
	/// @param is_draw_frame ƒtƒŒ[ƒ€‚ğ•`‰æ‚³‚¹‚é‚©‚Ç‚¤‚© (ˆêŸŒ³}Œ`‚Å‚ÍŒø‰Ê‚È‚µ)
	/// @param alpha_blend_num “§‰ß’l (0`UCHAR_MAX : 0‚ÅŠ®‘S“§‰ß) (ˆêŸŒ³}Œ`‚Å‚ÍŒø‰Ê‚È‚µ)
	/// @param frame_color ƒtƒŒ[ƒ€F
	virtual void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const abstract;
	
	virtual void Move(const VECTOR& velocity) abstract;

	[[nodiscard]] ShapeKind GetShapeKind() const { return m_shape_kind; }

private:
	ShapeKind m_shape_kind;

	friend void from_json	(const nlohmann::json& data, ShapeBase& shape_base);
	friend void to_json		(nlohmann::json& data, const ShapeBase& shape_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, ShapeBase& shape_base)
{
	data.at("m_shape_kind").get_to(shape_base.m_shape_kind);
}

inline void to_json(nlohmann::json& data, const ShapeBase& shape_base)
{
	data = nlohmann::json
	{
		{ "m_shape_kind", shape_base.m_shape_kind }
	};
}
#pragma endregion
