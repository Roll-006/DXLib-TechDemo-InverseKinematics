#pragma once
#include <memory>
#include <unordered_map>

#include "../Algorithm/algorithm.hpp"
#include "../Base/singleton_base.hpp"

#include "directional_light.hpp"
#include "point_light.hpp"
#include "spot_light.hpp"

// MEMO : <制約> 標準ライトは使用せず、自作ライトのみを使用するものとする

class LightHolder final : public SingletonBase<LightHolder>
{
public:
	void CreateLight(const std::shared_ptr<ILight> light);
	void DeleteLight(const int light_handle);
	void DeleteLight(const std::string& light_name);

	[[nodiscard]] std::shared_ptr<ILight>	GetLight(const int light_handle)		const;
	[[nodiscard]] std::shared_ptr<ILight>	GetLight(const std::string& light_name) const;
	[[nodiscard]] int						GetCurrentActiveLightNum()				const;

private:
	LightHolder();
	~LightHolder() override;

private:
	//static constexpr int kActiveLightMaxNum = 3;

	std::unordered_map<int, std::shared_ptr<ILight>>	m_lights;
	//std::vector<std::pair<int, int>>					m_priority;

	friend SingletonBase<LightHolder>;
};
