#include "cone.hpp"

//Cone::Cone(const VECTOR& vertex, const VECTOR& dir, const float length, const float circle_radius) :
//	ShapeBase	(ShapeKind::kCone),
//	m_vertex	(vertex),
//	m_dir		(dir),
//	m_length	(length),
//	m_radius	(circle_radius),
//	m_fov		(2.0f * atan(circle_radius / length))
//{
//
//}

Cone::Cone(const VECTOR& vertex, const VECTOR& dir, const float length, const float fov) :
	ShapeBase	(ShapeKind::kCone),
	m_vertex	(vertex),
	m_dir		(dir),
	m_length	(length),
	m_radius	(length * tan(fov * 0.5f)),
	m_fov		(fov)
{

}

Cone::Cone() :
	ShapeBase	(ShapeKind::kCone),
	m_vertex	(v3d::GetZeroV()),
	m_dir		(v3d::GetZeroV()),
	m_length	(0.0f),
	m_radius	(0.0f),
	m_fov		(0.0f)
{

}

Cone::~Cone()
{

}

void Cone::Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const
{
	const auto circle_center_pos = m_vertex + m_dir * m_length;

	if (alpha_blend_num > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_blend_num);
		DrawCone3D(m_vertex, circle_center_pos, m_radius, kPolygonDetailed, 0xffffff, 0xffffff, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (is_draw_frame)
	{
		DrawCone3D(m_vertex, circle_center_pos, m_radius, kPolygonDetailed, frame_color, 0xffffff, FALSE);
	}
}

void Cone::Move(const VECTOR& velocity)
{
	m_vertex += velocity;
}
