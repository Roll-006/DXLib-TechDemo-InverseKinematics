#pragma once
#include <DxLib.h>
#include "../Interface/i_light.hpp"

class DirectionalLight final : public ILight
{
public:
	DirectionalLight(const std::string& name, const int priority, const VECTOR& dir);
	~DirectionalLight() override;

	void Activate()		override { SetLightEnableHandle(m_light_handle, TRUE);  }
	void Deactivate()	override { SetLightEnableHandle(m_light_handle, FALSE); }


	#pragma region Setter
	void SetDir(const VECTOR& dir);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] LightKind		GetLightKind()		const override	{ return LightKind::kDirectionalLight; }
	[[nodiscard]] std::string	GetName()			const override	{ return m_name; }
	[[nodiscard]] int			GetLightHandle()	const override	{ return m_light_handle; }
	[[nodiscard]] int			GetPriority()		const override	{ return m_priority; }
	[[nodiscard]] VECTOR		GetDir()			const			{ return m_dir; }
	[[nodiscard]] bool			IsActive()			const override	{ return GetLightEnableHandle(m_light_handle); }
	#pragma endregion

private:
	std::string	m_name;
	int			m_light_handle;
	int			m_priority;
	VECTOR		m_dir;
};
