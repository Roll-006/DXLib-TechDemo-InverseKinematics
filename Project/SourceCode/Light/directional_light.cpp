#include "directional_light.hpp"

DirectionalLight::DirectionalLight(const std::string& name, const int priority, const VECTOR& dir) :
	m_name			(name),
	m_light_handle	(-1),
	m_priority		(priority),
	m_dir			(dir)
{
	m_light_handle = CreateDirLightHandle(m_dir);
}

DirectionalLight::~DirectionalLight()
{
	DeleteLightHandle(m_light_handle);
}


#pragma region Setter
void DirectionalLight::SetDir(const VECTOR& dir)
{
	m_dir = dir;

	SetLightDirectionHandle(m_light_handle, m_dir);
}
#pragma endregion
