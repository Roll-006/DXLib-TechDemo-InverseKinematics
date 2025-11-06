#include "light_holder.hpp"

LightHolder::LightHolder()
{
	SetUseLighting(TRUE);
	SetLightEnable(FALSE);
}

LightHolder::~LightHolder()
{

}

void LightHolder::CreateLight(const std::shared_ptr<ILight> light)
{
	const auto light_handle = light->GetLightHandle();
	m_lights[light_handle]	= light;

	//m_priority.emplace_back(light_handle, light->GetPriority());
	//algorithm::Sort(m_priority, SortKind::kDescending);
}

void LightHolder::DeleteLight(const int light_handle)
{
	if (m_lights.count(light_handle))
	{
		m_lights.at(light_handle)->Deactivate();
		m_lights.erase(light_handle);
	}
}

void LightHolder::DeleteLight(const std::string& light_name)
{
	for (auto itr = m_lights.begin(); itr != m_lights.end();)
	{
		if (itr->second->GetName() == light_name)
		{
			itr = m_lights.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

std::shared_ptr<ILight> LightHolder::GetLight(const int light_handle) const
{
	return m_lights.at(light_handle);
}

std::shared_ptr<ILight> LightHolder::GetLight(const std::string& light_name) const
{
	for (const auto& light : m_lights)
	{
		if (light.second->GetName() == light_name)
		{
			return light.second;
		}
	}
}

int LightHolder::GetCurrentActiveLightNum() const
{
	int active_light_num = 0;

	for (const auto& light : m_lights)
	{
		if (light.second->IsActive())
		{
			++active_light_num;
		}
	}

	return active_light_num;
}
