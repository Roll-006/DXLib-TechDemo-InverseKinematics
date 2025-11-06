#pragma once
#include <memory>

#include "transform.hpp"
#include "../GameTime/fps.hpp"

class CameraNoise final
{
public:
	CameraNoise(const std::shared_ptr<Transform>& camera_transform);
	~CameraNoise();

private:
	std::shared_ptr<Transform> m_camera_transform;
};
