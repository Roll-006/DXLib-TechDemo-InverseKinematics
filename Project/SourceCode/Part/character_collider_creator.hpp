#pragma once
#include <DxLib.h>

#include "../Base/physical_obj_base.hpp"
#include "../Part/modeler.hpp"

class CharacterColliderCreator
{
public:
	#pragma region コライダーの生成
	/// @brief カプセルコライダーを生成
	void CreateCapsuleCollider	(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float capsule_radius);

	/// @brief 着地トリガーを生成
	/// @brief WARNING : カプセルコライダーが生成された後に呼び出す必要あり
	void CreateLandingTrigger	(PhysicalObjBase* physical_obj, const float sphere_radius);

	/// @brief 視界用トリガーを生成
	void CreateVisionTrigger	(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float lenfth, const float fov);

	/// @brief 視認判定を受けるトリガーを生成
	void CreateVisibleTrigger	(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler);

	/// @brief 頭部トリガーを生成
	void CreateHeadTrigger		(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float sphere_radius);
	
	/// @brief 胴体トリガーを生成
	void CreateBodyTrigger		(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float up_body_capsule_radius, const float down_body_capsule_radius);
	
	/// @brief 腕部トリガーを生成
	void CreateArmTrigger		(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float up_arm_capsule_radius, const float down_arm_capsule_radius, const float hand_capsule_radius);

	/// @brief 脚部トリガーを生成
	void CreateLegTrigger		(PhysicalObjBase* physical_obj, std::shared_ptr<Modeler>& modeler, const float up_leg_capsule_radius, const float down_leg_capsule_radius);
	
	/// @brief メッシュトリガーを生成
	void CreateMeshTrigger		(PhysicalObjBase* phsyical_obj, std::shared_ptr<Modeler>& modeler);
	#pragma endregion


	#pragma region 位置計算
	/// @brief カプセルコライダーの位置を計算
	void CalcCapsuleColliderPos	(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider);

	/// @brief 着地トリガーの位置を計算
	/// @brief カプセルコライダーに依存して位置が計算される
	/// @brief カプセルが消失した場合、計算は行われない
	void CalcLandingTriggerPos	(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider);

	/// @brief 視界用トリガーの位置を計算
	/// @brief WARNING : トリガーが円錐であることを前提としている(軽量化を優先)
	void CalcVisionTriggerPos	(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider);

	/// @brief 視認判定を受けるトリガーの位置を計算
	/// @brief WARNING : トリガーが点であることを前提としている(軽量化を優先)
	void CalcVisibleTriggerPos	(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider);

	/// @brief 頭部トリガーの位置を計算
	/// @brief WARNING : トリガーがカプセルであることを前提としている(軽量化を優先)
	void CalcHeadTriggerPos		(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider);

	/// @brief 胴体トリガーの位置を計算
	/// @brief WARNING : トリガーがカプセルであることを前提としている(軽量化を優先)
	void CalcBodyTriggerPos		(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider);

	/// @brief 腕部トリガーの位置を計算
	/// @brief WARNING : トリガーがカプセルであることを前提としている(軽量化を優先)
	void CalcArmTriggerPos		(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider);

	/// @brief 脚部トリガーの位置を計算
	/// @brief WARNING : トリガーがカプセルであることを前提としている(軽量化を優先)
	void CalcLegTriggerPos		(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider);
	#pragma endregion


	#pragma region その他計算
	/// @brief カプセルコライダーの向きと長さを計算
	/// @brief WARNING : コライダーがカプセルであることを前提としている(軽量化を優先)
	void CalcCapsuleColliderDirAndLength(std::shared_ptr<Modeler>& modeler, const std::unordered_map<ColliderKind, std::shared_ptr<Collider>>& collider, const std::shared_ptr<Transform>& transform);
	#pragma endregion
};
