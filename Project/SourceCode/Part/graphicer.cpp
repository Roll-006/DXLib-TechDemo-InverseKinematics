#include "graphicer.hpp"

Graphicer::Graphicer(const std::string& file_path) : 
	m_graphic_handle		(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kGraphic, file_path)),
	m_center_pos			(v2d::GetZeroV<Vector2D<int>>()),
	m_scale					(1.0f, 1.0f),
	m_angle					(0.0f),
	m_origin_width			(0),
	m_origin_height			(0),
	m_blend_mode			(DX_BLENDMODE_ALPHA),
	m_blend_num				(UCHAR_MAX),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false)
{
	GetGraphSize(m_graphic_handle, &m_origin_width, &m_origin_height);
}

Graphicer::Graphicer(const int graphic_handle) : 
	m_graphic_handle		(graphic_handle),
	m_center_pos			(v2d::GetZeroV<Vector2D<int>>()),
	m_scale					(1.0f, 1.0f),
	m_angle					(0.0f),
	m_origin_width			(0),
	m_origin_height			(0),
	m_blend_mode			(DX_BLENDMODE_ALPHA),
	m_blend_num				(UCHAR_MAX),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false)
{
	GetGraphSize(m_graphic_handle, &m_origin_width, &m_origin_height);
}

Graphicer::~Graphicer()
{
	// MEMO : 画像のDelete処理は行わず、ハンドル保管クラスから再利用する
}

void Graphicer::Draw(const bool is_draw_graphic_frame) const
{
	if (m_blend_num > 0)
	{
		SetDrawBlendMode(m_blend_mode, m_blend_num);

		DrawRotaGraphFast3(
			m_center_pos.x,
			m_center_pos.y,
			static_cast<int>(m_origin_width  * 0.5f),
			static_cast<int>(m_origin_height * 0.5f),
			m_scale.x,
			m_scale.y,
			m_angle,
			m_graphic_handle,
			TRUE,
			m_is_invert_horizontal,
			m_is_invert_vertical);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (is_draw_graphic_frame)
	{
		DrawBox(
			static_cast<int>(m_center_pos.x - m_origin_width  * 0.5f * m_scale.x),
			static_cast<int>(m_center_pos.y - m_origin_height * 0.5f * m_scale.y),
			static_cast<int>(m_center_pos.x + m_origin_width  * 0.5f * m_scale.x),
			static_cast<int>(m_center_pos.y + m_origin_height * 0.5f * m_scale.y),
			0xff0000, FALSE);
	}
}
