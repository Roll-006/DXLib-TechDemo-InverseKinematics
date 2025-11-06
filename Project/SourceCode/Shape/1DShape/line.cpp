#include "line.hpp"

Line::Line(const VECTOR& pos, const VECTOR& dir):
	ShapeBase(ShapeKind::kLine),
	m_pos(pos),
	m_dir(v3d::GetNormalizedV(dir))
{
	// èàóùÇ»Çµ
}

Line::Line() :
	ShapeBase(ShapeKind::kLine),
	m_pos(v3d::GetZeroV()),
	m_dir(v3d::GetZeroV())
{
	// èàóùÇ»Çµ
}

Line::~Line()
{

}

void Line::Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const
{
	VECTOR begin = m_pos - m_dir * kDrawLength * 0.5f;
	VECTOR end = begin + m_dir * kDrawLength;
	DrawLine3D(begin, end, frame_color);
}

void Line::Move(const VECTOR& velocity)
{
	m_pos += velocity;
}

void Line::SetPos(const VECTOR& pos)
{
	m_pos = pos;
}
