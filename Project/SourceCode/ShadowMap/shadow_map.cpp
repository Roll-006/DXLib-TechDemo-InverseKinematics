#include "shadow_map.hpp"

ShadowMap::ShadowMap() :
	m_size_x			(0),
	m_size_y			(0),
	m_min_offset_pos	(kMinOffsetPos),
	m_max_offset_pos	(kMaxOffsetPos),
	m_shadow_map_handle	(MakeShadowMap(kSizeX, kSizeY))
{
	SetShadowMapLightDirection(m_shadow_map_handle, v3d::GetNormalizedV(VGet(0.5f, -0.5f, 0.5f)));
}

ShadowMap::~ShadowMap()
{
	DeleteShadowMap(m_shadow_map_handle);
}

void ShadowMap::SetupDrawShadowMap()
{
	const auto center_pos	= GetCameraPosition();
	const auto min_pos		= center_pos + m_min_offset_pos;
	const auto max_pos		= center_pos + m_max_offset_pos;

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadow_map_handle, min_pos, max_pos);

	ShadowMap_DrawSetup(m_shadow_map_handle);
}

void ShadowMap::EndDrawShadowMap()
{
	ShadowMap_DrawEnd();
}

void ShadowMap::UseShadowMap()
{
	SetUseShadowMap(0, m_shadow_map_handle);
}

void ShadowMap::UnuseShadowMap()
{
	SetUseShadowMap(0, -1);
}
