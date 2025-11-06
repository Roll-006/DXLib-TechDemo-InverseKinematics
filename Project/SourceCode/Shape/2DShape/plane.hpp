#pragma once
#include <array>

#include "square.hpp"

/// @brief 無限に続く平面
class Plane final : public ShapeBase
{
public:
	Plane(const VECTOR& pos, const VECTOR& normal_vector);
	Plane();
	~Plane() override;

	/// @brief 描画
	/// @param is_draw_frame フレームを描画させるかどうか
	/// @param alpha_blend_num 透過値 (0～UCHAR_MAX : 0で完全透過)
	/// @param frame_color フレーム色
	void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

	/// @brief 描画用の四角形を作成
	[[nodiscard]] Square&& CreateDrawSquare(const float edge_length) const;

	void Move(const VECTOR& velocity) override;

	void SetPos(const VECTOR& pos){ m_pos = pos; }

	VECTOR GetPos()			const	{ return m_pos; }
	VECTOR GetNormalVector() const	{ return m_normal_vector; }

private:
	static constexpr float kDrawDdgeLength = 300.0f;

	VECTOR  m_pos;				// 平面上の基準座標
	VECTOR  m_normal_vector;
};