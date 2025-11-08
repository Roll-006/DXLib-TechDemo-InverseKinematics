#pragma once
#include <queue>
#include "../Base/obj_base.hpp"

#include "../Kind/mass_kind.hpp"
#include "../Part/collider.hpp"
#include "../Data/collider_pair_data.hpp"
#include "../GameTime/game_time_manager.hpp"

/// @brief 物理的な挙動を行うオブジェクトの基底クラス
class PhysicalObjBase abstract : public ObjBase
{
public:
	PhysicalObjBase(const std::string& name, const std::string& tag);
	virtual ~PhysicalObjBase() = default;

	void DrawColliders() const;

	virtual void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) abstract;

	/// @brief ノックバックを受けた
	/// @param dir 飛ばされる方向
	/// @param initial_velocity 初速
	/// @param deceleration 減速度
	void OnKnockback(const VECTOR& dir, const float initial_velocity, const float deceleration);

	/// @brief コライダーを登録
	/// @brief 異なる種類であれば複数のコライダーの追加を許可
	/// @brief 同種類コライダーであっても図形が異なれば上書きを許可
	void AddCollider(const std::shared_ptr<Collider>& collider);

	/// @brief コライダーの登録を解除
	void RemoveCollider(const ColliderKind collider_kind);

	/// @brief 衝突した三角形の登録を解除する
	void RemoveHitTriangles();

	/// @brief 重力を与える(適用させる)
	/// @brief 物理管理クラスから適用される
	/// @param gravity_acceleration 重力加速度 (デルタタイム適用前)
	/// @param max_gravity 最大重力 (デルタタイム適用後の最大値)
	void ApplyGravity(const float gravity_acceleration, const float max_gravity);

	/// @brief 速度ベクトルに落下速度ベクトルを加算
	void AddFallVelocity()
	{
		m_velocity += m_fall_velocity;
		m_velocity *= GetDeltaTime();
	}

	/// @brief 速度ベクトルをトランスフォームおよびコライダーに適用
	void ApplyVelocity();

	/// @brief 速度ベクトルを接している面に投影する
	void ProjectVelocity();

	void ApplyKnockbackVelocity();

	/// @brief 着地判定を解除する
	void ReleaseLanding()	{ m_is_landing  = false; }
	void RemoveProjectPos() { m_project_pos = std::nullopt; }

	void SetColliderModelHandle(const int model_handle) { m_model_handle = model_handle; }
	void SetVelocity(const VECTOR& velocity) { m_velocity = velocity; }

	[[nodiscard]] int					GetColliderModelHandle()	const { return m_model_handle; }
	[[nodiscard]] std::optional<VECTOR> GetProjectPos()				const { return m_project_pos; }
	[[nodiscard]] float					GetKnockBackSpeed()			const { return m_knockback_speed; }
	[[nodiscard]] VECTOR				GetVelocity()				const { return m_velocity; }
	[[nodiscard]] VECTOR				GetMoveVelocity()			const { return m_move_velocity; }
	[[nodiscard]] VECTOR				GetFallVelocity()			const { return m_fall_velocity; }
	[[nodiscard]] MassKind				GetMassKind()				const { return mass_kind; }
	[[nodiscard]] bool					IsLanding()					const { return m_is_landing; }
	[[nodiscard]] bool					IsProject()					const { return m_is_project; }
	[[nodiscard]] std::shared_ptr<Collider> GetCollider(const ColliderKind kind) const;
	[[nodiscard]] std::unordered_map<ColliderKind, std::shared_ptr<Collider>> GetColliderAll() const { return m_colliders; }

protected:
	MassKind mass_kind;

	std::optional<VECTOR> m_project_pos;
	VECTOR	 m_velocity;
	VECTOR   m_move_velocity;
	VECTOR	 m_fall_velocity;
	VECTOR   m_knockback_velocity;
	float    m_knockback_speed;
	float	 m_knockback_deceleration;

	bool	 m_is_landing;
	bool	 m_is_project;	// 地面に張り付けるかを判定

	std::unordered_map<ColliderKind, std::shared_ptr<Collider>> m_colliders;

private:
	int		m_model_handle;

	friend void from_json	(const nlohmann::json& data, PhysicalObjBase& physical_obj_base);
	friend void to_json		(nlohmann::json& data, const PhysicalObjBase& physical_obj_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, PhysicalObjBase& physical_obj_base)
{
	from_json(data, static_cast<ObjBase&>(physical_obj_base));

	data.at("mass_kind").get_to(physical_obj_base.mass_kind);
}

inline void to_json(nlohmann::json& data, const PhysicalObjBase& physical_obj_base)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const ObjBase&>(physical_obj_base));

	nlohmann::json derived_json =
	{
		{ "mass_kind", physical_obj_base.mass_kind },
	};

	data = base_json;
	data.update(derived_json);
}
#pragma endregion
