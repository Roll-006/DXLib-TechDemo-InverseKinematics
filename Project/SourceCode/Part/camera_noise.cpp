#include "camera_noise.hpp"

CameraNoise::CameraNoise(const std::shared_ptr<Transform>& camera_transform) :
	m_camera_transform(camera_transform)
{

}

CameraNoise::~CameraNoise()
{

}
