#include "triangle.hpp"

Triangle::Triangle(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3):
	ShapeBase		(ShapeKind::kTriangle),
    m_normal_vector	(v3d::GetZeroV()),
	m_centroid		(v3d::GetZeroV()),
	m_image_handle	(-1)
{
	m_vertexes[0].pos = pos1;
	m_vertexes[1].pos = pos2;
	m_vertexes[2].pos = pos3;
	m_edges.at(0)	= Segment(pos1, pos2);
	m_edges.at(1)	= Segment(pos2, pos3);
	m_edges.at(2)	= Segment(pos3, pos1);
	m_normal_vector = math::GetNormalVector(m_vertexes[1].pos - m_vertexes[2].pos, m_vertexes[0].pos - m_vertexes[2].pos);
	m_centroid		= math::GetCentroidOfATriangle(*this);

	for (int i = 0; i < kVertexNum; ++i)
	{
		m_vertexes[i].norm	= axis::GetWorldYAxis();
		m_vertexes[i].dif	= GetColorU8(UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
		m_vertexes[i].spc	= GetColorU8(UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
	}
}

Triangle::Triangle() :
	ShapeBase(ShapeKind::kTriangle),
	m_normal_vector	(v3d::GetZeroV()),
	m_centroid		(v3d::GetZeroV()),
	m_image_handle	(-1)
{
	for (auto& edge : m_edges)
	{
		edge = Segment();
	}

	for (int i = 0; i < kVertexNum; ++i)
	{
		m_vertexes[i].pos		= v3d::GetZeroV();
		m_vertexes[i].norm		= axis::GetWorldYAxis();
		m_vertexes[i].dif		= GetColorU8(UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
		m_vertexes[i].spc		= GetColorU8(UCHAR_MAX, UCHAR_MAX, UCHAR_MAX, UCHAR_MAX);
	}
}

Triangle::~Triangle()
{
	// ˆ—‚È‚µ
}

void Triangle::Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const
{
	//if (is_draw_normal_vector)
	//{
	//	const float length = math::GetAverageValue<float>(m_edges.at(0).GetLength(), m_edges.at(1).GetLength(), m_edges.at(2).GetLength());
	//	DrawLine3D(m_centroid, m_centroid + m_normal_vector * length, 0xffffff);
	//}

	if (is_draw_frame)
	{
		for (const auto& edge : m_edges)
		{
			edge.Draw(false, 0, frame_color);
		}
	}

	if (alpha_blend_num > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_blend_num);
		if (m_image_handle == -1)
		{
			DrawPolygon3D(m_vertexes, 1, DX_NONE_GRAPH, TRUE);
		}
		else
		{
			DrawPolygon3D(m_vertexes, 1, m_image_handle, TRUE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Triangle::Move(const VECTOR& velocity)
{
	m_vertexes[0].pos += velocity;
	m_vertexes[1].pos += velocity;
	m_vertexes[2].pos += velocity;
	m_edges.at(0)	= Segment(m_vertexes[0].pos, m_vertexes[1].pos);
	m_edges.at(1)	= Segment(m_vertexes[1].pos, m_vertexes[2].pos);
	m_edges.at(2)	= Segment(m_vertexes[2].pos, m_vertexes[0].pos);
	m_normal_vector = math::GetNormalVector(m_vertexes[1].pos - m_vertexes[2].pos, m_vertexes[0].pos - m_vertexes[2].pos);
	m_centroid		= math::GetCentroidOfATriangle(*this);
}

void Triangle::LoadTexture(const std::string& file_path, const float u1, const float v1, const float u2, const float v2, const float u3, const float v3)
{
	m_image_handle = LoadGraph(file_path.c_str(), TRUE);

	m_vertexes[0].u = u1;
	m_vertexes[0].v = v1;
	m_vertexes[1].u = u2;
	m_vertexes[1].v = v2;
	m_vertexes[2].u = u3;
	m_vertexes[2].v = v3;
}
