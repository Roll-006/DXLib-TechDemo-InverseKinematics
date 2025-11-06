#include "spot_light.hpp"

SpotLight::SpotLight(const std::string& name, const int priority, const VECTOR& pos, const VECTOR& dir, const float angle, const float attenuation_angle, const LightRangeAttenuationData& range_attenuation) :
	m_name				(name),
	m_light_handle		(-1),
	m_priority			(priority),
	m_pos				(pos),
	m_dir				(dir),
	m_angle				(angle),
	m_attenuation_angle	(attenuation_angle),
	m_range_attenuation	(range_attenuation)
{
	m_light_handle = CreateSpotLightHandle(
		m_pos, m_dir, m_angle, m_attenuation_angle, 
		m_range_attenuation.range, 
		m_range_attenuation.attenuation_none_distance, 
		m_range_attenuation.attenuation_distance, 
		m_range_attenuation.attenuation_squared_distance);
}

SpotLight::~SpotLight()
{
	DeleteLightHandle(m_light_handle);
}


#pragma region Setter
void SpotLight::SetPos(const VECTOR& pos)
{
	m_pos = pos;

	SetLightPositionHandle(m_light_handle, m_pos);
}

void SpotLight::SetDir(const VECTOR& dir)
{
	m_dir = dir;

	SetLightDirectionHandle(m_light_handle, m_dir);
}

void SpotLight::SetAngle(const float angle, const float attenuation_angle)
{
	m_angle				= angle;
	m_attenuation_angle = attenuation_angle;

	SetLightAngleHandle(m_light_handle, m_angle, m_attenuation_angle);
}

void SpotLight::SetRangeAndAttenuation(const LightRangeAttenuationData& range_attenuation)
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
