#pragma once
#include <unordered_set>

#include "../Base/singleton_base.hpp"
#include "../Base/physical_obj_base.hpp"

#include "../Data/collider_data.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Calculation/collision_calculator.hpp"

class CollisionManager final : public SingletonBase<CollisionManager>
{
public:
	void LateUpdate();
	void Draw() const;


	#pragma region 登録・解除
	/// @brief 衝突判定を行うオブジェクトを追加
	void AddCollideObj	 (const std::shared_ptr<PhysicalObjBase>& collide_obj);
	/// @brief 衝突判定を行うオブジェクトから除外
	void RemoveCollideObj(const int obj_handle);

	/// @brief 衝突判定を無視するコライダーを追加
	void AddIgnoreCollider	 (const int obj_handle, const ColliderKind kind);
	void AddIgnoreCollider	 (const ColliderKind kind);
	/// @brief 衝突判定を無視するコライダーから除外
	void RemoveIgnoreCollider(const int obj_handle, const ColliderKind kind);
	void RemoveIgnoreCollider(const ColliderKind kind);

	/// @brief 衝突判定を無視するコライダーのペアを追加
	/// @brief ・owner_tagに[""]を指定すると指定なしで登録
	/// @brief ・ColliderKindに[kNone]を指定すると指定なしで登録
	/// @brief ・owner_tagとColliderKindがどちらも指定なしの場合、AddIgnoreCollider関数を使用することとする
	/// @brief 例) 着地トリガーと攻撃判定用トリガーの判定を無視したい場合、
	/// @brief [ColliderData("", ColliderKind::kLandingTrigger), ColliderData("", ColliderKind::kAttackTrigger)]とする
	void AddIgnoreColliderPair	 (const ColliderData& owner_collider_data, const ColliderData& target_collider_data);
	/// @brief 衝突判定を無視するコライダーのペアから除外
	void RemoveIgnoreColliderPair(const ColliderData& owner_collider_data, const ColliderData& target_collider_data);
	#pragma endregion

private:
	CollisionManager();
	~CollisionManager() override;

	/// @brief 衝突判定を無視するペアを設定する
	/// @brief 汎用性が高いペアをマネージャー側で追加する
	void SetIgnoreColliderPairs();

	/// @brief 衝突可能かを判定
	[[nodiscard]] bool CanCollideObjAndObj			(const std::shared_ptr<PhysicalObjBase>& owner_obj, const std::shared_ptr<PhysicalObjBase>& target_obj);
	[[nodiscard]] bool CanCollideObjAndCollider		(const std::shared_ptr<PhysicalObjBase>& owner_obj, const std::shared_ptr<Collider>& target_collider);
	[[nodiscard]] bool CanCollideColliderAndCollider(const std::shared_ptr<Collider>& owner_collider, const std::shared_ptr<Collider>& target_collider);

	/// @brief 衝突判定を起こしたコライダーの組み合わせを生成
	std::vector<ColliderPairOneToManyData> CreateHitColliderPairs();


	#pragma region 衝突判定
	bool IsCollided					(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedPointAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedLineAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedSegmentAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedPlaneAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedTriangleAndTarget(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedSquareAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedAABBAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedOBBAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedSphereAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedCapsuleAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsCollidedModelAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	#pragma endregion

private:
	static constexpr float kIgnoreDistance = 200.0f;

	std::vector<std::shared_ptr<PhysicalObjBase>>						m_collide_objects;					// 衝突判定を行うオブジェクト
	std::unordered_map<int,  std::unordered_set<ColliderKind>>			m_ignore_collide_colliders;			// 衝突判定を無視するコライダー
	std::unordered_map<ColliderData, std::unordered_set<ColliderData>>	m_ignore_collide_collider_pairs;	// 衝突判定を無視するコライダーのペア
	int m_handle_create_count;

	friend SingletonBase<CollisionManager>;
};
