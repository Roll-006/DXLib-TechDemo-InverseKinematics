#include "square.hpp"

Square::Square(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const VECTOR& pos4):
	ShapeBase	(ShapeKind::kSquare),
	m_centroid	(v3d::GetZeroV())
{
	m_triangles.at(0) = Triangle(pos1, pos2, pos3);
	m_triangles.at(1) = Triangle(pos1, pos3, pos4);
	m_centroid = math::GetCentroidOfAQuadrilateral(*this);
}

Square::Square() :
	ShapeBase	(ShapeKind::kSquare),
	m_centroid	(v3d::GetZeroV())
{
	for (auto& triangle : m_triangles)
	{
		triangle = Triangle();
	}
}

Square::~Square()
{

}

void Square::Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const
{
	//if (is_draw_normal_vector)
	//{
	//	const float length = math::GetAverageValue<float>(
	//		m_triangles.at(0).GetEdge(0).GetLength(), 
	//		m_triangles.at(0).GetEdge(1).GetLength(),
	//		m_triangles.at(1).GetEdge(1).GetLength(),
	//		m_triangles.at(1).GetEdge(2).GetLength());

	//	DrawLine3D(m_centroid, m_centroid + GetNormalVector() * length, 0xffffff);
	//}

	if (is_draw_frame)
	{
		m_triangles.at(0).GetEdge(0).Draw(false, 0, frame_color);
		m_triangles.at(0).GetEdge(1).Draw(false, 0, frame_color);
		m_triangles.at(1).GetEdge(1).Draw(false, 0, frame_color);
		m_triangles.at(1).GetEdge(2).Draw(false, 0, frame_color);
	}

	for (const auto& triangle : m_triangles)
	{
		triangle.Draw(false, alpha_blend_num, 0xffffff);
	}
}

void Square::Move(const VECTOR& velocity)
{
	for (auto& triangle : m_triangles)
	{
		triangle.Move(velocity);
	}

	m_centroid = math::GetCentroidOfAQuadrilateral(*this);
}

void Square::LoadTexture(const std::string& file_path,
	const float u1, const float v1, 
	const float u2, const float v2, 
	const float u3, const float v3, 
	const float u4, const float v4)
{
	m_triangles.at(0).LoadTexture(file_path, u1, v1, u2, v2, u3, v3);
	m_triangles.at(1).LoadTexture(file_path, u1, v1, u3, v3, u4, v4);
}

void Square::LoadTexture(const std::string& file_path, const TextureDirKind texture_dir)
{
	// ê≥ñ Ç…å¸Ç©Ç¡Çƒí£ÇËïtÇØÇÈèÍçáÇèâä˙ílÇ∆Ç∑ÇÈ
	std::array<float, 8> texture_pos{
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	switch (texture_dir)
	{
	case TextureDirKind::kBack:
		std::rotate(texture_pos.begin(), next(texture_pos.begin(), 4), texture_pos.end());
		break;

	case TextureDirKind::kLeft:
		std::rotate(texture_pos.begin(), next(texture_pos.begin(), 6), texture_pos.end());
		break;

	case TextureDirKind::kRight:
		std::rotate(texture_pos.begin(), next(texture_pos.begin(), 2), texture_pos.end());
		break;

	default:
		break;
	}

	m_triangles.at(0).LoadTexture(file_path, texture_pos.at(0), texture_pos.at(1), texture_pos.at(2), texture_pos.at(3), texture_pos.at(4), texture_pos.at(5));
	m_triangles.at(1).LoadTexture(file_path, texture_pos.at(0), texture_pos.at(1), texture_pos.at(4), texture_pos.at(5), texture_pos.at(6), texture_pos.at(7));
}

VECTOR Square::GetPos(const int index) const
{
	switch (index)
	{
	case 0:	return m_triangles.at(0).GetPos(0); break;
	case 1:	return m_triangles.at(0).GetPos(1); break;
	case 2:	return m_triangles.at(0).GetPos(2); break;
	case 3:	return m_triangles.at(1).GetPos(2); break;

	default:
		break;
	}
	return v3d::GetZeroV();
}

const Segment& Square::GetEdge(const int index) const
{
	switch (index)
	{
	case 0:	return m_triangles.at(0).GetEdge(0); break;
	case 1:	return m_triangles.at(0).GetEdge(1); break;
	case 2:	return m_triangles.at(1).GetEdge(1); break;
	case 3:	return m_triangles.at(1).GetEdge(2); break;

	default:
		break;
	}
	return Segment();
}
