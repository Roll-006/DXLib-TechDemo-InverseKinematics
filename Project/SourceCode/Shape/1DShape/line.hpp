#pragma once
#include "../../Base/shape_base.hpp"

class Line final : public ShapeBase
{
public:
	Line(const VECTOR& pos, const VECTOR& dir);
	Line();
	~Line() override;

	/// @brief 描画
	/// @param is_draw_frame フレームを描画させるかどうか
	/// @param alpha_blend_num 透過値 (0～UCHAR_MAX : 0で完全透過)
	/// @param frame_color フレーム色
	void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

	void Move(const VECTOR& velocity) override;

	void SetPos(const VECTOR& pos);

	/// @brief 直線上の点を取得
	/// @param length 始点からの長さ
	[[nodiscard]] VECTOR GetPoint(const float length) const{ return m_pos + m_dir * length; }

	[[nodiscard]] VECTOR GetPos() const	{ return m_pos; }
	[[nodiscard]] VECTOR GetDir() const	{ return m_dir; }

private:
	static constexpr int kDrawLength = 10000;

	VECTOR m_pos;
	VECTOR m_dir;
};
