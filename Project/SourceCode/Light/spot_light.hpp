#pragma once
#include <DxLib.h>

#include "../Interface/i_light.hpp"
#include "../Data/light_range_attenuation_data.hpp"

class SpotLight final : public ILight
{
public:
	SpotLight(const std::string& name, const int priority, const VECTOR& pos, const VECTOR& dir, const float angle, const float attenuation_angle, const LightRangeAttenuationData& range_attenuation);
	~SpotLight() override;

	void Activate()		override { SetLightEnableHandle(m_light_handle, TRUE ); }
	void Deactivate()	override { SetLightEnableHandle(m_light_handle, FALSE); }


	#pragma region Setter
	void SetPos					(const VECTOR& pos);
	void SetDir					(const VECTOR& dir);
	void SetAngle				(const float angle, const float attenuation_angle);
	void SetRangeAndAttenuation	(const LightRangeAttenuationData& range_attenuation);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] LightKind					GetLightKind()				const override	{ return LightKind::kSpotLight; }
	[[nodiscard]] std::string				GetName()					const override	{ return m_name; }
	[[nodiscard]] int						GetLightHandle()			const override	{ return m_light_handle; }
	[[nodiscard]] int						GetPriority()				const override	{ return m_priority; }
	[[nodiscard]] VECTOR					GetPos()					const			{ return m_pos; }
	[[nodiscard]] VECTOR					GetDir()					const			{ return m_dir; }
	[[nodiscard]] float						GetAngle()					const			{ return m_angle; }
	[[nodiscard]] float						GetAttenuationAngle()		const			{ return m_attenuation_angle; }
	[[nodiscard]] LightRangeAttenuationData	GetRangeAndAttenuation()	const			{ return m_range_attenuation; }
	[[nodiscard]] bool						IsActive()					const override	{ return GetLightEnableHandle(m_light_handle); }
	#pragma endregion

private:
	std::string					m_name;
	int							m_light_handle;
	int							m_priority;
	VECTOR						m_pos;
	VECTOR						m_dir;
	float						m_angle;
	float						m_attenuation_angle;
	LightRangeAttenuationData	m_range_attenuation;
};
