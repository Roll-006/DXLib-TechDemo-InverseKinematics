#include "main_camera.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Command/command_handler.hpp"

MainCamera::MainCamera() : 
	PhysicalObjBase	(ObjName.MAIN_CAMERA, ObjTag.CAMERA),
	m_aim_pos		(v3d::GetZeroV())
{
	mass_kind = MassKind::kHeavy;

	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));

	// カメラが無視するコライダー
	const auto collision_manager = CollisionManager::GetInstance();
	const ColliderData ray_cast_data{ ObjTag.CAMERA, ColliderKind::kRayCast };
	collision_manager->AddIgnoreCollider(GetObjHandle(), ColliderKind::kCollider);
	collision_manager->AddIgnoreColliderPair(ray_cast_data, { ObjTag.PLAYER,	ColliderKind::kNone });
}

MainCamera::~MainCamera()
{

}

void MainCamera::Init()
{
	RemoveCollider(ColliderKind::kCollider);
}

void MainCamera::Update()
{
	if (!IsActive()) { return; }
}

void MainCamera::LateUpdate()
{
	if (!IsActive()) { return; }

	SetAim();

	CalcRayCastPos();
}

void MainCamera::Draw() const
{
	if (!IsActive()) { return; }

	//DrawColliders();
}

void MainCamera::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase*	target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto			target_name				= target_obj->GetName();
	const auto			target_tag				= target_obj->GetTag();
	const auto			target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kRayCast:
		if (hit_collider_pair.intersection)
		{
			m_transform->SetPos(CoordinateKind::kWorld, *hit_collider_pair.intersection);
			SetAim();
		}
		break;

	default:
		break;
	}
}

void MainCamera::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj				(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj		(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj		(physical_obj);
	PhysicsManager	::GetInstance()->AddIgnoreObjGravity(GetObjHandle());

	CinemachineBrain::GetInstance()->SetMainCamera(std::static_pointer_cast<MainCamera>(shared_from_this()));
}

void MainCamera::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	PhysicsManager	::GetInstance()->RemoveIgnoreObjGravity	(obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj		(obj_handle);
	CollisionManager::GetInstance()->RemoveCollideObj		(obj_handle);
	ObjManager		::GetInstance()->RemoveObj				(obj_handle);
}

void MainCamera::ApplyMatrix(const MATRIX& matrix)
{
	m_transform->SetMatrix(CoordinateKind::kWorld, matrix);
}



#pragma region Getter
float MainCamera::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kCamera);
}
#pragma endregion


void MainCamera::SetAim()
{
	const VECTOR pos		= m_transform->GetPos(CoordinateKind::kWorld);
	const VECTOR target_pos	= pos + m_transform->GetForward(CoordinateKind::kWorld);

	SetCameraPositionAndTarget_UpVecY(pos, target_pos);
}

void MainCamera::CalcRayCastPos()
{
	// 光線の座標を計算
	auto ray = std::static_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	ray->SetBeginPos(m_aim_pos, true);
	ray->SetEndPos	(m_transform->GetPos(CoordinateKind::kWorld), true);
}
