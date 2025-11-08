#pragma once
#include "physical_obj_base.hpp"

#include "../Interface/i_state_controller.hpp"
#include "../Part/character_collider_creator.hpp"
#include "../Part/modeler.hpp"
#include "animator_base.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class CharacterBase abstract : public PhysicalObjBase
{
public:
	CharacterBase(const std::string& name, const std::string& tag);
	virtual ~CharacterBase() = default;

	void AddToObjManager()		override;
	void RemoveToObjManager()	override;

	void CalcCorrectMoveDir();
	void AllowCalcLookDir()		{ m_is_calc_look_dir = true; }


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<Modeler>		GetModeler()		const { return m_modeler; }
	[[nodiscard]] std::shared_ptr<AnimatorBase>	GetAnimator()		const { return m_animator; }
	[[nodiscard]] VECTOR						GetCurrentMoveDir()	const { return m_move_dir.at(TimeKind::kCurrent); }
	[[nodiscard]] VECTOR						GetCurrentLookDir()	const { return m_look_dir.at(TimeKind::kCurrent); }
	#pragma endregion

protected:
	/// @brief 見ている方向を回転に適用する
	void ApplyLookDirToRot(const VECTOR& look_dir);

	void CalcMoveDir();
	void CalcLookDir();
	void CalcMoveVelocity();

protected:
	float invincible_time;

	std::shared_ptr<Modeler>					m_modeler;
	std::shared_ptr<AnimatorBase>				m_animator;
	std::shared_ptr<CharacterColliderCreator>	m_collider_creator;

	std::unordered_map<TimeKind, VECTOR>		m_move_dir;					// 移動方向(WARNING : 長さは0～1の範囲を取る)
	std::unordered_map<TimeKind, VECTOR>		m_look_dir;					// 向いている方向
	VECTOR										m_destination;			// 補正先座標
	float										m_move_speed;
	float										m_move_dir_offset_speed;	// 移動方向を補正する速度
	float										m_look_dir_offset_speed;	// 見る方向の補正速度
	bool										m_is_calc_look_dir;

	friend void from_json(const nlohmann::json& data, CharacterBase& character_base);
	friend void to_json  (nlohmann::json& data, const CharacterBase& character_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, CharacterBase& character_base)
{
	from_json(data, static_cast<PhysicalObjBase&>(character_base));
}

inline void to_json(nlohmann::json& data, const CharacterBase& character_base)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const PhysicalObjBase&>(character_base));

	nlohmann::json derived_json =
	{
		
	};

	data = base_json;
	data.update(derived_json);
}
#pragma endregion
