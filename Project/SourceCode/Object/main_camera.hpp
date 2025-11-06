#pragma once
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class CinemachineBrain;

class MainCamera final : public PhysicalObjBase
{
public:
	MainCamera();
	~MainCamera() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	void ApplyMatrix(const MATRIX& matrix);
	void SetAimPos(const VECTOR& aim_pos) { m_aim_pos = aim_pos; }


	#pragma region Getter
	[[nodiscard]] float	GetDeltaTime()  const override;
	#pragma endregion

private:
	/// @brief 見る方向を設定
	void SetAim();

	/// @brief レイキャストトリガーの位置を計算
	void CalcRayCastPos();

private:
	VECTOR	m_aim_pos;
};
