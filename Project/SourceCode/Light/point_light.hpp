#pragma once
#include <DxLib.h>

#include "../Interface/i_light.hpp"
#include "../Data/light_range_attenuation_data.hpp"

class PointLight final : public ILight
{
public:
	PointLight(const std::string& name, const int priority, const VECTOR& pos, const LightRangeAttenuationData& range_attenuation);
	~PointLight() override;

	void Activate()		override { SetLightEnableHandle(m_light_handle, TRUE);  }
	void Deactivate()	override { SetLightEnableHandle(m_light_handle, FALSE); }


	#pragma region Setter
	void SetPos					(const VECTOR& pos);
	void SetRangeAndAttenuation	(const LightRangeAttenuationData& range_attenuation);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] LightKind					GetLightKind()				const override	{ return LightKind::kPointLight; }
	[[nodiscard]] std::string				GetName()					const override	{ return m_name; }
	[[nodiscard]] int						GetLightHandle()			const override	{ return m_light_handle; }
	[[nodiscard]] int						GetPriority()				const override	{ return m_priority; }
	[[nodiscard]] VECTOR					GetPos()					const			{ return m_pos; }
	[[nodiscard]] LightRangeAttenuationData	GetRangeAndAttenuation()	const			{ return m_range_attenuation; }
	[[nodiscard]] bool						IsActive()					const override	{ return GetLightEnableHandle(m_light_handle); }
	#pragma endregion

private:
	std::string					m_name;
	int							m_light_handle;
	int							m_priority;
	VECTOR						m_pos;
	LightRangeAttenuationData	m_range_attenuation;
};
