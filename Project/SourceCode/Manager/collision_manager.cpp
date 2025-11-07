#include "collision_manager.hpp"

CollisionManager::CollisionManager() : 
	m_handle_create_count(-1)
{
	SetIgnoreColliderPairs();
	AddIgnoreCollider(ColliderKind::kCollisionAreaTrigger);
}

CollisionManager::~CollisionManager()
{
	// 処理なし
}

void CollisionManager::LateUpdate()
{
	for (const auto& obj : m_collide_objects)
	{
		obj->ReleaseLanding();
		obj->RemoveHitTriangles();
		obj->RemoveHitCollider();
	}

	const auto collider_pairs = CreateHitColliderPairs();

	// 衝突したコライダーのオーナーオブジェクトの処理を実行
	for (const auto& pair : collider_pairs)
	{
		// レイキャストトリガーの場合は最初に衝突したコライダーのみ衝突したものとする
		if (pair.owner_collider->GetColliderKind() == ColliderKind::kRayCast && pair.owner_collider->IsClosestOnlyHit())
		{
			// コライダーとの距離を取得
			std::unordered_map<int, TargetColliderData> target;
			std::vector<std::pair<int, float>>			distance;
			for (size_t i = 0; i < pair.target_data.size(); ++i)
			{
				if (pair.target_data.at(i).intersection)
				{
					target[i] = TargetColliderData(pair.target_data.at(i).collider, pair.target_data.at(i).intersection);

					const auto dist = VSize(*pair.target_data.at(i).intersection - std::static_pointer_cast<Segment>(pair.owner_collider->GetShape())->GetBeginPos());
					distance.emplace_back(std::make_pair(i, dist));
				}
			}

			// 最も近いコライダーのみ判定
			distance = algorithm::Sort(distance, SortKind::kAscending);
			for (const auto& dist : distance)
			{
				pair.owner_collider			  ->GetOwnerObj()->OnCollide(ColliderPairOneToOneData(pair.owner_collider, target.at(dist.first).collider, target.at(dist.first).intersection));
				target.at(dist.first).collider->GetOwnerObj()->OnCollide(ColliderPairOneToOneData(target.at(dist.first).collider, pair.owner_collider, target.at(dist.first).intersection));
				break;
			}
		}
		// レイキャストトリガーでない場合はすべてのコライダーと衝突判定を行う
		else
		{
			for (const auto& target : pair.target_data)
			{
				pair.owner_collider->GetOwnerObj()->OnCollide(ColliderPairOneToOneData(pair.owner_collider, target.collider, target.intersection));
				target.collider	   ->GetOwnerObj()->OnCollide(ColliderPairOneToOneData(target.collider, pair.owner_collider, target.intersection));
			}
		}
	}
}

void CollisionManager::Draw() const
{
	for (const auto& obj : m_collide_objects)
	{
		obj->DrawColliders();
	}
}


#pragma region 登録・解除
void CollisionManager::AddCollideObj(const std::shared_ptr<PhysicalObjBase>& collide_obj)
{
	if (std::find(m_collide_objects.begin(), m_collide_objects.end(), collide_obj) == m_collide_objects.end())
	{
		m_collide_objects.emplace_back(collide_obj);
	}
}

void CollisionManager::RemoveCollideObj(const int obj_handle)
{
	const auto physical_obj = ObjManager::GetInstance()->GetObj<PhysicalObjBase>(obj_handle);

	if (std::find(m_collide_objects.begin(), m_collide_objects.end(), physical_obj) != m_collide_objects.end())
	{
		erase(m_collide_objects, physical_obj);
	}
}

void CollisionManager::AddIgnoreCollider(const int obj_handle, const ColliderKind kind)
{
	m_ignore_collide_colliders[obj_handle].insert(kind);
}

void CollisionManager::AddIgnoreCollider(const ColliderKind kind)
{
	// 指定なしは-2で追加させる
	AddIgnoreCollider(-2, kind);
}

void CollisionManager::RemoveIgnoreCollider(const int obj_handle, const ColliderKind kind)
{
	auto itr = m_ignore_collide_colliders.find(obj_handle);
	if (itr != m_ignore_collide_colliders.end())
	{
		itr->second.erase(kind);
	}
}

void CollisionManager::RemoveIgnoreCollider(const ColliderKind kind)
{
	// 指定なしは-2で追加させる
	RemoveIgnoreCollider(-2, kind);
}

void CollisionManager::AddIgnoreColliderPair(const ColliderData& owner_collider_data, const ColliderData& target_collider_data)
{
	m_ignore_collide_collider_pairs[owner_collider_data]. insert(target_collider_data);
	m_ignore_collide_collider_pairs[target_collider_data].insert(owner_collider_data);
}

void CollisionManager::RemoveIgnoreColliderPair(const ColliderData& owner_collider_data, const ColliderData& target_collider_data)
{
	auto itr = m_ignore_collide_collider_pairs.find(owner_collider_data);
	if (itr != m_ignore_collide_collider_pairs.end())
	{
		itr->second.erase(target_collider_data);
	}

	itr = m_ignore_collide_collider_pairs.find(target_collider_data);
	if (itr != m_ignore_collide_collider_pairs.end())
	{
		itr->second.erase(owner_collider_data);
	}
}
#pragma endregion


void CollisionManager::SetIgnoreColliderPairs()
{

}

bool CollisionManager::CanCollideObjAndObj(const std::shared_ptr<PhysicalObjBase>& owner_obj, const std::shared_ptr<PhysicalObjBase>& target_obj)
{
	// 静的オブジェクト同士は無視(地面と家など)
	if (owner_obj->GetMassKind() == MassKind::kStatic && target_obj->GetMassKind() == MassKind::kStatic) { return false; }

	// 距離が遠いオブジェクト同士は無視
	const auto owner_collision_area		= owner_obj->GetCollider (ColliderKind::kCollisionAreaTrigger);
	const auto target_collision_area	= target_obj->GetCollider(ColliderKind::kCollisionAreaTrigger);
	if (owner_collision_area && target_collision_area)
	{
		const auto sphere1 = std::static_pointer_cast<Sphere>(owner_collision_area ->GetShape());
		const auto sphere2 = std::static_pointer_cast<Sphere>(target_collision_area->GetShape());

		if (math::GetDistanceSphereToSphere(*sphere1.get(), *sphere2.get()) > kIgnoreDistance)
		{
			return false;
		}
	}

	// 無視リストに登録されているオブジェクトは無視
	const ColliderData owner_data { owner_obj ->GetTag(), ColliderKind::kNone };
	if (m_ignore_collide_collider_pairs.count(owner_data))
	{
		const ColliderData target_data{ target_obj->GetTag(), ColliderKind::kNone };
		if (m_ignore_collide_collider_pairs.at(owner_data).count(target_data))
		{
			return false;
		}
	}

	return true;
}

bool CollisionManager::CanCollideObjAndCollider(const std::shared_ptr<PhysicalObjBase>& owner_obj, const std::shared_ptr<Collider>& target_collider)
{
	const auto target_obj = target_collider->GetOwnerObj();

	const std::vector<ColliderData> target_data
	{
		{ target_obj->GetTag(), target_collider->GetColliderKind() },
		{ "",				    target_collider->GetColliderKind() }
	};

	// 無視リストに登録されているコライダーは無視
	const ColliderData owner_data{ owner_obj->GetTag(), ColliderKind::kNone };
	if (m_ignore_collide_collider_pairs.count(owner_data))
	{
		for (const auto& target : target_data)
		{
			if (m_ignore_collide_collider_pairs.at(owner_data).count(target))
			{
				return false;
			}
		}
	}

	return true;
}

bool CollisionManager::CanCollideColliderAndCollider(const std::shared_ptr<Collider>& owner_collider, const std::shared_ptr<Collider>& target_collider)
{
	// 無視リストに登録されているオブジェクトは無視
	const auto owner_obj	= owner_collider->GetOwnerObj();
	const auto target_obj	= target_collider->GetOwnerObj();

	const std::vector<ColliderData> owner_data
	{
		{ owner_obj->GetTag(), owner_collider->GetColliderKind() },
		{ "",				   owner_collider->GetColliderKind() }
	};

	const std::vector<ColliderData> target_data
	{
		{ target_obj->GetTag(), target_collider->GetColliderKind() },
		{ "",				    target_collider->GetColliderKind() }
	};

	// 無視リストに登録されているコライダーは無視
	for (const auto& owner : owner_data)
	{
		if (m_ignore_collide_collider_pairs.count(owner))
		{
			for (const auto& target : target_data)
			{
				if (m_ignore_collide_collider_pairs.at(owner).count(target))
				{
					return false;
				}
			}
		}
	}

	return true;
}

std::vector<ColliderPairOneToManyData> CollisionManager::CreateHitColliderPairs()
{
	std::vector<ColliderPairOneToManyData> collider_pairs;

	for (const auto& owner_obj : m_collide_objects)
	{
		// 非アクティブの場合はスキップ
		if (!owner_obj->IsActive()) { continue; }

		for (const auto& target_obj : m_collide_objects)
		{
			// 自身との当たり判定は避ける
			if (owner_obj == target_obj) { continue; }

			// 非アクティブの場合はスキップ
			if (!target_obj->IsActive()) { continue; }

			// 衝突不可の場合はスキップ
			if (!CanCollideObjAndObj(owner_obj, target_obj)) { continue; }

			for (const auto& owner_obj_collider : owner_obj->GetColliderAll())
			{
				// 単独で無視リストに登録されていた場合は無視
				auto itr = m_ignore_collide_colliders.find(owner_obj->GetObjHandle());
				if (itr != m_ignore_collide_colliders.end())
				{
					if (itr->second.count(owner_obj_collider.second->GetColliderKind()))
					{
						continue;
					}
				}
				itr = m_ignore_collide_colliders.find(-2);
				if (itr != m_ignore_collide_colliders.end())
				{
					if (itr->second.count(owner_obj_collider.second->GetColliderKind()))
					{
						continue;
					}
				}

				// 衝突不可の場合はスキップ
				if (!CanCollideObjAndCollider(target_obj, owner_obj_collider.second)) { continue; }

				for (const auto& target_obj_collider : target_obj->GetColliderAll())
				{
					// 単独で無視リストに登録されていた場合は無視
					auto itr = m_ignore_collide_colliders.find(target_obj->GetObjHandle());
					if (itr != m_ignore_collide_colliders.end())
					{
						if (itr->second.count(target_obj_collider.second->GetColliderKind()))
						{
							continue;
						}
					}
					itr = m_ignore_collide_colliders.find(-2);
					if (itr != m_ignore_collide_colliders.end())
					{
						if (itr->second.count(target_obj_collider.second->GetColliderKind()))
						{
							continue;
						}
					}

					// 衝突不可の場合はスキップ
					// TODO : 無視判定のループ回数が多いため改善を検討
					if (!CanCollideObjAndCollider		(owner_obj, target_obj_collider.second))				 { continue; }
					if (!CanCollideColliderAndCollider	(owner_obj_collider.second, target_obj_collider.second)) { continue; }


					// 衝突判定
					std::optional<VECTOR> intersection = std::nullopt;
					if (IsCollided(*owner_obj_collider.second, *target_obj_collider.second, intersection))
					{
						// 指定のオーナーのデータコンテナがまだない場合は新たに作成
						bool is_maked = std::any_of(collider_pairs.begin(), collider_pairs.end(), [=](const ColliderPairOneToManyData& data)
						{
							return data.owner_collider == owner_obj_collider.second;
						});
						if (!is_maked)
						{
							collider_pairs.emplace_back(owner_obj_collider.second, std::vector<TargetColliderData>());
						}

						// オーナーが同じデータにターゲットを追加
						for (size_t i = 0; i < collider_pairs.size(); ++i)
						{
							if (collider_pairs.at(i).owner_collider == owner_obj_collider.second)
							{
								collider_pairs.at(i).target_data.emplace_back(TargetColliderData(target_obj_collider.second, intersection));
							}
						}
					}
				}
			}
		}
	}

	return collider_pairs;
}


#pragma region 衝突判定
bool CollisionManager::IsCollided					(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto shape = owner_collider.GetShape();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return IsCollidedModelAndTarget(owner_collider, target_collider, intersection);
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kPoint:		return IsCollidedPointAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kLine:		return IsCollidedLineAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kSegment:	return IsCollidedSegmentAndTarget	(owner_collider, target_collider, intersection);
	case ShapeKind::kPlane:		return IsCollidedPlaneAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kTriangle:	return IsCollidedTriangleAndTarget	(owner_collider, target_collider, intersection);
	case ShapeKind::kSquare:	return IsCollidedSquareAndTarget	(owner_collider, target_collider, intersection);
	case ShapeKind::kAABB:		return IsCollidedAABBAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kOBB:		return IsCollidedOBBAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kSphere:	return IsCollidedSphereAndTarget	(owner_collider, target_collider, intersection);
	case ShapeKind::kCapsule:	return IsCollidedCapsuleAndTarget	(owner_collider, target_collider, intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsCollidedPointAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Point>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		return false;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kLine:
		return collision::IsCollidedPointAndLine	(owner_shape, *std::static_pointer_cast<Line>		(target_shape), intersection);

	case ShapeKind::kSegment:
		return collision::IsCollidedPointAndSegment	(owner_shape, *std::static_pointer_cast<Segment>	(target_shape), intersection);

	case ShapeKind::kPlane:
		return collision::IsCollidedPointAndPlane	(owner_shape, *std::static_pointer_cast<Plane>		(target_shape), intersection);

	case ShapeKind::kTriangle:
		return collision::IsCollidedPointAndTriangle(owner_shape, *std::static_pointer_cast<Triangle>	(target_shape), intersection);

	case ShapeKind::kSquare:
		return collision::IsCollidedPointAndSquare	(owner_shape, *std::static_pointer_cast<Square>		(target_shape), intersection);
	
	case ShapeKind::kOBB:
		return collision::IsCollidedPointAndOBB		(owner_shape, *std::static_pointer_cast<OBB>		(target_shape), intersection);

	case ShapeKind::kCapsule:
		return collision::IsCollidedPointAndCapsule	(owner_shape, *std::static_pointer_cast<Capsule>	(target_shape), intersection);

	case ShapeKind::kCone:
		return collision::IsCollidedPointAndCone	(owner_shape, *std::static_pointer_cast<Cone>		(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsCollidedLineAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Line>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		return false;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kLine:
		return collision::IsCollidedLineAndLine (owner_shape, *std::static_pointer_cast<Line> (target_shape), intersection);

	case ShapeKind::kPlane:
		return collision::IsCollidedLineAndPlane(owner_shape, *std::static_pointer_cast<Plane>(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsCollidedSegmentAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Segment>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		std::vector<Triangle> hit_triangles;
		const bool is_hit = collision::IsCollidedSegmentAndModel(owner_shape, target_collider.GetModelHandle(), intersection, hit_triangles);
		
		if (is_hit && target_collider.GetColliderKind() == ColliderKind::kCollider)
		{
			owner_collider.AddHitModelTriangle(target_collider.GetModelHandle(), hit_triangles);
		}

		return is_hit;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kSegment:
		return collision::IsCollidedSegmentAndSegment	(owner_shape, *std::static_pointer_cast<Segment> (target_shape), intersection);

	case ShapeKind::kPlane:
		return collision::IsCollidedSegmentAndPlane		(owner_shape, *std::static_pointer_cast<Plane>   (target_shape), intersection);

	case ShapeKind::kTriangle:
		return collision::IsCollidedSegmentAndTriangle	(owner_shape, *std::static_pointer_cast<Triangle>(target_shape), intersection);

	case ShapeKind::kSquare:
		return collision::IsCollidedSegmentAndSquare	(owner_shape, *std::static_pointer_cast<Square>  (target_shape), intersection);

	case ShapeKind::kSphere:
		return collision::IsCollidedSegmentAndSphere	(owner_shape, *std::static_pointer_cast<Sphere>  (target_shape), intersection);

	case ShapeKind::kCapsule:
		return collision::IsCollidedSegmentAndCapsule	(owner_shape, *std::static_pointer_cast<Capsule> (target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsCollidedPlaneAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Plane>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		return false;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:
		return collision::IsCollidedPlaneAndCapsule(owner_shape, *std::static_pointer_cast<Capsule>(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsCollidedTriangleAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Triangle>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		std::vector<Triangle> hit_triangles;
		const bool is_hit = collision::IsCollidedTriangleAndModel(owner_shape, target_collider.GetModelHandle(), intersection, hit_triangles);
		
		// 衝突対象がコライダーであった場合は三角形情報を追加する
		if (is_hit && target_collider.GetColliderKind() == ColliderKind::kCollider)
		{
			owner_collider.AddHitModelTriangle(target_collider.GetModelHandle(), hit_triangles);
		}

		return is_hit;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:
		return collision::IsCollidedTriangleAndCapsule(owner_shape, *std::static_pointer_cast<Capsule>(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsCollidedSquareAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Square>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		return false;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:
		return collision::IsCollidedSquareAndCapsule(owner_shape, *std::static_pointer_cast<Capsule>(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsCollidedAABBAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	return false;
}

bool CollisionManager::IsCollidedOBBAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	return false;
}

bool CollisionManager::IsCollidedSphereAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape	= target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Sphere>(owner_collider.GetShape());
	bool	   is_hit		= false;
	std::vector<Triangle> hit_triangles;

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		is_hit = collision::IsCollidedSphereAndModel(owner_shape, target_collider.GetModelHandle(), intersection, hit_triangles);
		
		// 衝突対象がコライダーであった場合は三角形情報を追加する
		if (is_hit && target_collider.GetColliderKind() == ColliderKind::kCollider)
		{
			owner_collider.AddHitModelTriangle(target_collider.GetModelHandle(), hit_triangles);
		}

		return is_hit;
	}

	switch (target_shape->GetShapeKind())
	{
	//case ShapeKind::kSegment:
	//	return collision::IsCollidedSegmentAndSphere(*std::dynamic_pointer_cast<Segment>(target_shape), owner_shape, intersection);

	case ShapeKind::kTriangle:
		is_hit = collision::IsCollidedTriangleAndSphere(*std::static_pointer_cast<Triangle>(target_shape), owner_shape, intersection);

		// 衝突対象がコライダーであった場合は三角形情報を追加する
		if (is_hit && target_collider.GetColliderKind() == ColliderKind::kCollider)
		{
			owner_collider.AddHitModelTriangle(target_collider.GetModelHandle(), hit_triangles);
		}
		break;

	case ShapeKind::kSphere:
		return collision::IsCollidedSphereAndSphere (owner_shape, *std::dynamic_pointer_cast<Sphere>(target_shape), intersection);
	
	case ShapeKind::kCapsule:
		return collision::IsCollidedSphereAndCapsule(owner_shape, *std::static_pointer_cast<Capsule>(target_shape), intersection);

	default:
		break;
	}

	return is_hit;
}

bool CollisionManager::IsCollidedCapsuleAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Capsule>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		std::vector<Triangle> hit_triangles;
		const bool is_hit = collision::IsCollidedCapsuleAndModel(owner_shape, target_collider.GetModelHandle(), intersection, hit_triangles);
		
		// 衝突対象がコライダーであった場合は三角形情報を追加する
		if (is_hit && target_collider.GetColliderKind() == ColliderKind::kCollider)
		{
			owner_collider.AddHitModelTriangle(target_collider.GetModelHandle(), hit_triangles);
		}
		return is_hit;
	}

	switch (target_shape->GetShapeKind())
	{
	//case ShapeKind::kSegment:
	//	return collision::IsCollidedSegmentAndCapsule(*std::dynamic_pointer_cast<Segment>(target_shape), owner_shape, intersection);

	case ShapeKind::kCapsule:
		return collision::IsCollidedCapsuleAndCapsule(owner_shape, *std::static_pointer_cast<Capsule>(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsCollidedModelAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	return false;
}
#pragma endregion
