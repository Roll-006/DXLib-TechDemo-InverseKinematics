#include "virtual_camera.hpp"
#include "cinemachine_brain.hpp"
#include "../Handle/handle_creator.hpp"

VirtualCamera::VirtualCamera(const std::string& name, const BlendActivationPolicyKind blend_policy_kind) : 
	name						(name),
	m_camera_handle				(HandleCreator::GetInstance()->CreateHandle()),
	priority					(0),
	m_is_active					(true),
	m_transform					(std::make_shared<Transform>()),
	m_body						(nullptr),
	m_aim						(nullptr),
	m_noise						(nullptr),
	m_target_transform			(nullptr),
	blend_activation_policy_kind(blend_policy_kind)
{
	m_body	= std::make_shared<CameraBody> (m_transform);
	m_aim	= std::make_shared<CameraAim>  (m_transform);
	m_noise = std::make_shared<CameraNoise>(m_transform);
}

VirtualCamera::VirtualCamera() :
	name						(""),
	m_camera_handle				(HandleCreator::GetInstance()->CreateHandle()),
	priority					(0),
	m_is_active					(true),
	m_transform					(std::make_shared<Transform>()),
	m_body						(nullptr),
	m_aim						(nullptr),
	m_noise						(nullptr),
	m_target_transform			(nullptr),
	blend_activation_policy_kind(BlendActivationPolicyKind::kKeepOriginCamera)
{
	m_body	= std::make_shared<CameraBody> (m_transform);
	m_aim	= std::make_shared<CameraAim>  (m_transform);
	m_noise = std::make_shared<CameraNoise>(m_transform);
}

VirtualCamera::~VirtualCamera()
{

}

void VirtualCamera::Init()
{

}

void VirtualCamera::Update()
{
	if (!IsActive()) { return; }
}

void VirtualCamera::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcTransform();
}

void VirtualCamera::AttachTarget(const std::shared_ptr<Transform>& target_transform)
{
	m_target_transform = target_transform;

	m_body	->AttachTarget(m_target_transform);
	m_aim	->AttachTarget(m_target_transform);
}

void VirtualCamera::DetachTarget()
{
	m_target_transform = nullptr;

	m_body	->DetachTarget();
	m_aim	->DetachTarget();
}


void VirtualCamera::SetPriority(const int priority)
{
	this->priority = priority;

	CinemachineBrain::GetInstance()->SortPriority(std::static_pointer_cast<VirtualCamera>(shared_from_this()));
}

float VirtualCamera::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kCamera);
}

void VirtualCamera::CalcTransform()
{
	m_body	->CalcPos();
	m_aim	->CalcRot();
	m_body	->CalcDampedPos();
	m_aim	->CalcDampedRot();
}
