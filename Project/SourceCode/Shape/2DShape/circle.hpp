#pragma once
#include "triangle.hpp"

class Circle final : public ShapeBase
{
public:
	/// @brief コンストラクタ
	/// @param has_internal 内部があるかを判定(true : 円, false : 円周)
	/// @param normal_vector 法線ベクトル
	/// @param radius 半径
	Circle(const VECTOR& normal_vector, float radius);
	Circle();
	~Circle();

	/// @brief 描画
	/// @param is_draw_frame フレームを描画させるかどうか
	/// @param alpha_blend_num 透過値 (0～UCHAR_MAX : 0で完全透過)
	/// @param frame_color フレーム色
	void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

	void Move(const VECTOR& velocity) override;

	void SetPos			(const VECTOR&	pos);
	void SetNormalVector(const VECTOR&	normal_vector);
	void SetRadius		(const float	radius);

	[[nodiscard]] VECTOR GetPos()			const { return m_pos; }
	[[nodiscard]] VECTOR GetNormalVector()	const { return m_normal_vector; }
	[[nodiscard]] float  GetRadius()		const { return m_radius; }

private:
	static constexpr int kVertexNum = 30;		// 頂点の数(実際には正円だが、描画するために頂点を設定)

	std::array<Triangle, kVertexNum> m_triangles;			// 三角形

	VECTOR m_pos;				// 中心座標
	VECTOR m_normal_vector;		// 法線ベクトル
	float  m_radius;			// 半径
	float  m_interval_andle;	// 頂点間の間隔
};