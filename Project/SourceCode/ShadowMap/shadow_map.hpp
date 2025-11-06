#pragma once
#include <DxLib.h>
#include "../Base/single_instance_base.hpp"

#include "../Part/transform.hpp"
#include "../Event/event_system.hpp"

class ShadowMap final : public SingleInstanceBase<ShadowMap>
{
public:
	ShadowMap();
	~ShadowMap();

	void SetupDrawShadowMap();
	void EndDrawShadowMap();

	void UseShadowMap();
	void UnuseShadowMap();

private:
	static constexpr int	kSizeX			= 8192;
	static constexpr int	kSizeY			= 8192;
	static constexpr VECTOR kMinOffsetPos	= { -400.0f, -100.0f, -400.0f };
	static constexpr VECTOR kMaxOffsetPos	= {  400.0f,  100.0f,  400.0f };

	int		m_size_x;
	int		m_size_y;
	VECTOR	m_min_offset_pos;
	VECTOR	m_max_offset_pos;

	int m_shadow_map_handle;

	friend SingleInstanceBase<ShadowMap>;
};
