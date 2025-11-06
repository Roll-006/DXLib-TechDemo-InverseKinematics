#include "drawer.hpp"

Drawer::Drawer() :
	m_shadow_map(std::make_unique<ShadowMap>())
{
	
}

Drawer::~Drawer()
{

}

void Drawer::DrawToShadowMap(const std::shared_ptr<IScene>& current_scene) const
{
	m_shadow_map->SetupDrawShadowMap();

	current_scene->DrawToShadowMap();

	m_shadow_map->EndDrawShadowMap();
}

void Drawer::Draw(const std::shared_ptr<IScene>& current_scene) const
{
	m_shadow_map->UseShadowMap();

	current_scene->Draw();

	m_shadow_map->UnuseShadowMap();
	DrawEffekseer3D();
}
