#pragma once
#include <EffekseerForDXLib.h>

#include "../Base/single_instance_base.hpp"
#include "../Interface/i_scene.hpp"

#include "../ShadowMap/shadow_map.hpp"

// MEMO : DXLib‚Ì–½–¼‚É‡‚í‚¹‚ÄRender‚Å‚Í‚È‚­Drawer‚Æ‚·‚é
class Drawer final : public SingleInstanceBase<Drawer>
{
public:
	Drawer();
	~Drawer();

	void DrawToShadowMap(const std::shared_ptr<IScene>& current_scene) const;
	void Draw			(const std::shared_ptr<IScene>& current_scene) const;

private:
	std::unique_ptr<ShadowMap> m_shadow_map;
};
