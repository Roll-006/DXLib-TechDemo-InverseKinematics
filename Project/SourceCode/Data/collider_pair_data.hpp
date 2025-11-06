#pragma once
#include "../Part/collider.hpp"

/// @brief 衝突対象のコライダーデータ
struct TargetColliderData
{
	std::shared_ptr<Collider>		collider			= nullptr;
	std::optional<VECTOR>			intersection		= std::nullopt;		// 衝突時の交点
	//VECTOR							hit_normal_vector	= {};
};

/// @brief 一対一の衝突コライダーデータ
struct ColliderPairOneToOneData
{
	std::shared_ptr<Collider>		owner_collider		= nullptr;
	std::shared_ptr<Collider>		target_collider		= nullptr;
	std::optional<VECTOR>			intersection		= std::nullopt;		// 衝突時の交点
	//VECTOR							hit_normal_vector	= {};
};

/// @brief 一対多の衝突コライダーデータ
struct ColliderPairOneToManyData
{
	std::shared_ptr<Collider>		owner_collider		= nullptr;
	std::vector<TargetColliderData>	target_data			= {};
};
