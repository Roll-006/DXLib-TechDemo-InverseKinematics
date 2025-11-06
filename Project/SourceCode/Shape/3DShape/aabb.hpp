#pragma once
#include "../../Base/shape_base.hpp"

class AABB : public ShapeBase
{
public:
	AABB();
	~AABB() override;

	/// @brief 描画
	/// @param is_draw_frame フレームを描画させるかどうか
	/// @param alpha_blend_num 透過値 (0～UCHAR_MAX : 0で完全透過)
	/// @param frame_color フレーム色
	void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

	void Move(const VECTOR& velocity) override;

private:
};