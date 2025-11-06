#pragma once
#include "../../Base/shape_base.hpp"

class Segment final : public ShapeBase
{
public:
	Segment(const VECTOR& begin_pos, const VECTOR& end_pos);
	Segment(const VECTOR& begin_pos, const VECTOR& dir, const float length);
	Segment();
	~Segment() override;

	/// @brief 描画
	/// @param is_draw_frame フレームを描画させるかどうか
	/// @param alpha_blend_num 透過値 (0～UCHAR_MAX : 0で完全透過)
	/// @param frame_color フレーム色
	void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

	void Move(const VECTOR& velocity) override;

	/// @brief 始点を設定する
	/// @param begin_pos 始点
	/// @param is_alone 始点を単独で設定するか(true : 終点の変化なし、長さの変化あり, false : 終点の変化あり、長さの変化なし)
	void SetBeginPos(const VECTOR& begin_pos, const bool is_alone);
	/// @brief 終点を設定する
	/// @param end_pos 終点
	/// @param is_alone 終点を単独で設定するか(true : 始点の変化なし、長さの変化あり, false : 始点の変化あり、長さの変化なし)
	void SetEndPos  (const VECTOR& end_pos,   const bool is_alone);

	/// @brief 向きを設定
	/// @brief 始点は変更されず終点が変化する
	void SetDir		(const VECTOR& dir);

	void SetLength  (const float length);

	/// @brief 直線上の点を取得
	/// @param length 始点からの長さ
	[[nodiscard]] VECTOR GetPoint(const float length) const { return m_begin_pos + m_dir * length; }

	[[nodiscard]] VECTOR GetBeginPos()	const { return m_begin_pos; }
	[[nodiscard]] VECTOR GetEndPos()	const { return m_end_pos; }
	[[nodiscard]] VECTOR GetCenterPos() const { return m_begin_pos + m_dir * m_length * 0.5f; }
	[[nodiscard]] VECTOR GetDir()		const { return m_dir; }
	[[nodiscard]] float  GetLength()	const { return m_length; }
	
private:
	VECTOR m_begin_pos;
	VECTOR m_end_pos;
	VECTOR m_dir;
	float  m_length;

	friend void from_json	(const nlohmann::json& data, Segment& segment);
	friend void to_json		(nlohmann::json& data, const Segment& segment);
};


//#pragma region from / to JSON
//inline void from_json(const nlohmann::json& data, Segment& segment)
//{
//	to_json(data, static_cast<const ShapeBase&>(segment));
//
//	data.at("begin_pos").get_to(segment.m_begin_pos);
//	data.at("end_pos")	.get_to(segment.m_end_pos);
//	data.at("dir")		.get_to(segment.m_dir);
//	data.at("length")	.get_to(segment.m_length);
//}
//
//inline void to_json(nlohmann::json& data, const Segment& segment)
//{
//	from_json(data, static_cast<ShapeBase&>(segment));
//
//	data = nlohmann::json
//	{
//		{ "begin_pos",	segment.m_begin_pos },
//		{ "end_pos",	segment.m_end_pos },
//		{ "dir",		segment.m_dir },
//		{ "length",		segment.m_length }
//	};
//}
//#pragma endregion
