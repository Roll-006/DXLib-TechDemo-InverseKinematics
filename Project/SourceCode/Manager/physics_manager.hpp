#pragma once
#include "../Base/singleton_base.hpp"
#include "../Base/physical_obj_base.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Calculation/collision_calculator.hpp"

class PhysicsManager final : public SingletonBase<PhysicsManager>
{
public:
	void Update();
	void LateUpdate();


	#pragma region 登録・解除
	/// @brief 物理的挙動を行うオブジェクトを追加
	template<obj_concepts::PhysicalObjT PhysicalObjT>
	void AddPhysicalObj   (const std::shared_ptr<PhysicalObjT>& physical_obj)
	{
		// 上書き不可
		if (std::find(m_physical_objects.begin(), m_physical_objects.end(), physical_obj) == m_physical_objects.end())
		{
			m_physical_objects.emplace_back(physical_obj);
		}
	}
	void RemovePhysicalObj				(const int obj_handle);

	/// @brief 物理的な挙動全般を無視するオブジェクトを追加
	void AddIgnoreObjPhysicalBehavior   (const int obj_handle);
	void RemoveIgnoreObjPhysicalBehavior(const int obj_handle);

	/// @brief 重力の影響を無視するオブジェクトを追加
	void AddIgnoreObjGravity			(const int obj_handle);
	void RemoveIgnoreObjGravity			(const int obj_handle);

	/// @brief 押し戻しを無視するオブジェクトのペアを追加
	void AddIgnorePushBackPair   (const int obj_handle1, const int obj_handle2);
	void RemoveIgnorePushBackPair(const int obj_handle1, const int obj_handle2);
	#pragma endregion

private:
	PhysicsManager();
	~PhysicsManager() override;

	/// @brief すべてのオブジェクトの押し戻し処理を行う
	void ExecutePushBackPairs();


	#pragma region 押し戻し
	void PushBack				  (const std::shared_ptr<PhysicalObjBase>& low_priority_obj, const std::shared_ptr<PhysicalObjBase>& high_priority_obj);
	void PushBackTriangleAndTarget(const std::shared_ptr<PhysicalObjBase>& low_priority_obj, const std::shared_ptr<PhysicalObjBase>& high_priority_obj);
	void PushBackSphereAndTarget  (const std::shared_ptr<PhysicalObjBase>& low_priority_obj, const std::shared_ptr<PhysicalObjBase>& high_priority_obj);
	void PushBackCapsuleAndTarget (const std::shared_ptr<PhysicalObjBase>& low_priority_obj, const std::shared_ptr<PhysicalObjBase>& high_priority_obj);
	#pragma endregion


	/// @brief 物理的な挙動を適用するかを判定する
	[[nodiscard]] bool IsApplyPhysicalBehavior(const std::shared_ptr<PhysicalObjBase>& physical_obj) const
	{
		const auto itr = std::find(m_ignore_physical_behavior_obj_handle.begin(), m_ignore_physical_behavior_obj_handle.end(), physical_obj->GetObjHandle());

		// 無視するリストに登録されていれば適用しない
		if (itr != m_ignore_physical_behavior_obj_handle.end()) { return false; }

		return true;
	}

	/// @brief 重力を適用するかを判定する
	[[nodiscard]] bool IsApplyGravity(const std::shared_ptr<PhysicalObjBase>& physical_obj) const
	{
		const auto itr = std::find(m_ignore_gravity_obj_handle.begin(), m_ignore_gravity_obj_handle.end(), physical_obj->GetObjHandle());

		// 無視リストに登録されている、もしくは静的オブジェクトであった場合は重力を適用させない
		if (itr != m_ignore_gravity_obj_handle.end() || physical_obj->GetMassKind() == MassKind::kStatic) { return false; }

		return true;
	}

	[[nodiscard]] bool CanPushBack(const int obj_handle1, const int obj_handle2);

	void ProjectionPos(const std::shared_ptr<PhysicalObjBase> physical_obj);

private:
	static constexpr float kGravityAcceleration				= 180.0f;	// 重力加速度(デルタタイム適用前)
	static constexpr float kMaxGravity						= 140.0f;	// 最大重力(デルタタイム適用後の最大値)
	static constexpr float kIgnoreDistance					= 100.0f;

	std::vector<std::shared_ptr<PhysicalObjBase>> m_physical_objects;				// 物理的挙動を行うオブジェクト
	std::vector<int>					m_ignore_physical_behavior_obj_handle;		// 物理的な挙動全般を無視するオブジェクト
	std::vector<int>					m_ignore_gravity_obj_handle;				// 重力の影響を無視するオブジェクト
	std::vector<std::pair<int, int>>	m_ignore_push_back_pair_obj_handle;

	friend SingletonBase<PhysicsManager>;
};
