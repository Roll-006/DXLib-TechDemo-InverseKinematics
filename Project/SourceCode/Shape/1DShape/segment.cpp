#include "segment.hpp"

Segment::Segment(const VECTOR& begin_pos, const VECTOR& end_pos):
	ShapeBase	(ShapeKind::kSegment),
	m_begin_pos	(begin_pos),
	m_end_pos	(end_pos),
	m_dir		(v3d::GetZeroV()),
	m_length    (0.0f)
{
	m_dir		= v3d::GetNormalizedV(m_end_pos - m_begin_pos);
	m_length	= VSize(m_end_pos - m_begin_pos);
}

Segment::Segment(const VECTOR& begin_pos, const VECTOR& dir, const float length) :
	ShapeBase	(ShapeKind::kSegment),
	m_begin_pos	(begin_pos),
	m_end_pos	(v3d::GetZeroV()),
	m_dir		(v3d::GetNormalizedV(dir)),
	m_length    (length)
{
	m_end_pos = m_begin_pos + m_dir * m_length;
}

Segment::Segment() :
	ShapeBase	(ShapeKind::kSegment),
	m_begin_pos	(v3d::GetZeroV()),
	m_end_pos	(v3d::GetZeroV()),
	m_dir		(v3d::GetZeroV()),
	m_length	(0.0f)
{
	// èàóùÇ»Çµ
}

Segment::~Segment()
{
	// èàóùÇ»Çµ
}

void Segment::Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const
{
	DrawLine3D(m_begin_pos, m_end_pos, frame_color);
}

void Segment::Move(const VECTOR& velocity)
{
	m_begin_pos += velocity;
	m_end_pos	= m_begin_pos + m_dir * m_length;
}

void Segment::SetBeginPos(const VECTOR& begin_pos, const bool is_alone)
{
	if (is_alone)
	{
		const auto distance_v = m_end_pos - m_begin_pos;

		m_begin_pos = begin_pos;
		m_length	= VSize(distance_v);
		m_dir		= v3d::GetNormalizedV(distance_v);
	}
	else
	{
		m_begin_pos = begin_pos;
		m_end_pos   = m_begin_pos + m_dir * m_length;
	}
}

void Segment::SetEndPos(const VECTOR& end_pos, const bool is_alone)
{
	if (is_alone)
	{
		const auto distance_v = m_end_pos - m_begin_pos;

		m_end_pos	= end_pos;
		m_length	= VSize(distance_v);
		m_dir		= v3d::GetNormalizedV(distance_v);
	}
	else
	{
		m_end_pos   = end_pos;
		m_begin_pos = m_end_pos - m_dir * m_length;
	}
}

void Segment::SetDir(const VECTOR& dir)
{
	m_dir = dir;
	m_end_pos = m_begin_pos + m_dir * m_length;
}

void Segment::SetLength(const float length)
{
	m_length  = length;
	m_end_pos = m_begin_pos + m_dir * m_length;
}
