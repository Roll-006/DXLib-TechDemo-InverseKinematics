#include "circle.hpp"

Circle::Circle(const VECTOR& normal_vector, float radius):
	ShapeBase		(ShapeKind::kCircle),
	m_pos			(v3d::GetZeroV()),
	m_normal_vector	(normal_vector),
	m_radius		(radius),
	m_interval_andle(360.0f / kVertexNum * math::kDegToRad)
{
	SetPos(m_pos);
}

Circle::Circle() :
	ShapeBase		(ShapeKind::kCircle),
	m_pos			(v3d::GetZeroV()),
	m_normal_vector	(v3d::GetZeroV()),
	m_radius		(0.0f),
	m_interval_andle(360.0f / kVertexNum * math::kDegToRad)
{
	SetPos(m_pos);
}

Circle::~Circle()
{
	// ˆ—‚È‚µ
}

// FIXME : F‚ª‚Â‚©‚È‚¢ƒ~ƒX‚ ‚è
void Circle::Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const
{
	//if (is_draw_normal_vector)
	//{
	//	DrawLine3D(m_pos, m_pos + m_normal_vector * kNormalVectorLength, 0xd900ff);
	//}

	if (is_draw_frame)
	{
		for (const auto& triangle : m_triangles)
		{
			DrawLine3D(triangle.GetPos(0), triangle.GetPos(1), frame_color);
		}
	}

	if (alpha_blend_num > 0)
	{
		for (const auto& triangle : m_triangles)
		{
			triangle.Draw(false, alpha_blend_num, 0xffffff);
		}
	}
}

void Circle::Move(const VECTOR& velocity)
{
	m_pos += velocity;

	for (auto& triangle : m_triangles)
	{
		triangle.Move(velocity);
	}
}

void Circle::SetPos(const VECTOR& pos)
{
	VECTOR dir1 = math::GetNormalVector(m_normal_vector);
	VECTOR dir2 = math::GetNormalVector(m_normal_vector, dir1);
	m_pos		= pos;
	
	for (size_t i = 0; i < m_triangles.size(); ++i)
	{
		VECTOR pos1 = m_pos + m_radius * (cosf(m_interval_andle *  i     ) * dir1 + sinf(m_interval_andle *  i     ) * dir2);
		VECTOR pos2 = m_pos + m_radius * (cosf(m_interval_andle * (i + 1)) * dir1 + sinf(m_interval_andle * (i + 1)) * dir2);

		m_triangles.at(i) = Triangle(pos1, pos2, m_pos);
	}
}

void Circle::SetNormalVector(const VECTOR& normal_vector)
{
	m_normal_vector = v3d::GetNormalizedV(normal_vector);
}

void Circle::SetRadius(const float	radius)
{
	m_radius = radius;
}
