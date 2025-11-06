#include "point.hpp"

Point::Point(const VECTOR& pos) : 
	ShapeBase	(ShapeKind::kPoint),
	m_pos		(pos)
{

}

Point::Point() :
	ShapeBase	(ShapeKind::kPoint),
	m_pos		(v3d::GetZeroV())
{

}

Point::~Point()
{

}

void Point::Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const
{
	DrawPixel3D(m_pos, frame_color);
}

void Point::Move(const VECTOR& velocity)
{
	m_pos += velocity;
}
