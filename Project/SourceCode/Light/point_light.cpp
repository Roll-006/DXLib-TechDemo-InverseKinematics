#include "point_light.hpp"

PointLight::PointLight(const std::string& name, const int priority, const VECTOR& pos, const LightRangeAttenuationData& range_attenuation) :
	m_name				(name),
	m_light_handle		(-1),
	m_priority			(priority),
	m_pos				(pos),
	m_range_attenuation	(range_attenuation)
{
	m_light_handle = CreatePointLightHandle(
		m_pos,
		m_range_attenuation.range, 
		m_range_attenuation.attenuation_none_distance,
		m_range_attenuation.attenuation_distance, 
		m_range_attenuation.attenuation_squared_distance);
}

PointLight::~PointLight()
{
	DeleteLightHandle(m_light_handle);
}


#pragma region Setter
void PointLight::SetPos(const VECTOR& pos)
{
	m_pos = pos;

	SetLightPositionHandle(m_light_handle, m_pos);
}

void PointLight::SetRangeAndAttenuation(const LightRangeAttenuationData& range_attenuation)
{
	m_range_attenuation = range_attenuation;

	SetLightRangeAttenHandle(
		m_light_handle,
		m_range_attenuation.range,
		m_range_attenuation.attenuation_none_distance,
		m_range_attenuation.attenuation_distance,
		m_range_attenuation.attenuation_squared_distance);
}
#pragma endregion
