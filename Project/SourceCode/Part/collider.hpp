#pragma once
#include <cassert>
#include <utility>

#include "../Kind/collider_kind.hpp"
#include "../IncludeList/shape.hpp"

class PhysicalObjBase;

class Collider final
{
public:
	Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase>& shape, PhysicalObjBase* owner_obj);
	Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase>& shape, const bool is_closest_only_hit, PhysicalObjBase* owner_obj);
	Collider(const ColliderKind kind, const int model_handle, PhysicalObjBase* owner_obj);
	~Collider();

	/// @brief すべてのコライダーとの衝突判定を有効にする(レイキャストでのみ有効な関数)
	void EnableAllRayCastHit();
	/// @brief 最も近いコライダーとの衝突判定のみを有効にする(レイキャストでのみ有効な関数)
	void EnableClosestOnlyRayCastHit();


	#pragma region 登録 / 解除
	// 衝突した三角形を追加する
	void AddHitTriangle(const Triangle& hit_triangle);
	/// @brief 衝突した三角形をまとめて追加する
	void AddHitTriangles(const std::vector<Triangle>& hit_triangles);
	/// @brief 衝突した三角形の登録を解除する
	void RemoveHitTriangles();
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] ColliderKind										GetColliderKind()		const { return m_kind; }
	[[nodiscard]] std::shared_ptr<ShapeBase>						GetShape()				const { return m_shape; }
	[[nodiscard]] int												GetModelHandle()		const { return m_model_handle; }
	[[nodiscard]] PhysicalObjBase*									GetOwnerObj()			const { return m_owner_obj; }
	[[nodiscard]] std::vector<Triangle>								GetHitTriangles()		const { return m_hit_triangles; }

	/// @brief 最も近いコライダーとのみ衝突を許可するかを判定(レイキャストでのみ有効な関数)
	[[nodiscard]] bool												IsClosestOnlyHit()		const { return m_is_closest_only_hit; }
	[[nodiscard]] bool												IsOneCollision()		const { return m_is_one_collision; }
	#pragma endregion

private:
	/// @brief 有効な図形であるかを判定
	void JudgeValidShape();

private:
	ColliderKind				m_kind;
	std::shared_ptr<ShapeBase>	m_shape;
	int							m_model_handle;
	bool						m_is_closest_only_hit;	// 自身と最も近いコライダーのみ衝突を許可する(レイキャストでのみ有効)
	bool						m_is_one_collision;		// 衝突判定を一度のみ許可する
	PhysicalObjBase*			m_owner_obj;

	std::vector<Triangle>		m_hit_triangles;		// 衝突した三角形
};
