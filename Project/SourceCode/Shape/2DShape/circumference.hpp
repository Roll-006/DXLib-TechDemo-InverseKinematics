//#pragma once
//#include <array>
//#include <DxLib.h>
//#include "triangle.hpp"
//#include "../../Base/shape_base.hpp"
//
//class Circumference : public ShapeBase
//{
//public:
//	Circumference(const VECTOR& normal_vector, float radius);
//	Circumference();
//	~Circumference();
//
//	void Init(const VECTOR& normal_vector, float radius);
//	void Update(const VECTOR& pos);
//
//	/// @brief 描画
//	/// @param is_draw_normal_vector 法線ベクトルを描画するかどうか
//	/// @param is_draw_frame 枠を描画させるかどうか
//	/// @param alpha_blend_num 透過値 (0～UCHAR_MAX : 0で完全透過)
//	void Draw(bool is_draw_normal_vector, bool is_draw_frame, int vertex_num, int alpha_blend_num) const;
//
//	[[nodiscard]] VECTOR GetPos() const		{ return m_pos; }
//	[[nodiscard]] VECTOR GetNormalVector() const { return m_normal_vector; }
//	[[nodiscard]] float  GetRadius() const		{ return m_radius; }
//
//private:
//	static constexpr float kNormalVectorLength	= 100.0f;
//	static constexpr int   kVertexNum			= 40;		// 頂点の数(実際には正円だが、描画するために頂点を設定)
//
//	VECTOR m_pos;				// 中心座標
//	VECTOR m_normal_vector;		// 法線ベクトル
//	float  m_radius;			// 半径
//	float  m_interval_andle;	// 頂点間の間隔
//};
