#pragma once
#include <typeindex>

#include "player_idle.hpp"
#include "player_move.hpp"
#include "player_action_null.hpp"
#include "player_run.hpp"
#include "player_crouch.hpp"

#include "../../Command/command_handler.hpp"

class PlayerState final
{
public:
	PlayerState();
	~PlayerState();

	void Init  (std::shared_ptr<Player> player);
	void Update(std::shared_ptr<Player> player);


	#pragma region Try判定
	[[nodiscard]] bool TryMove();
	[[nodiscard]] bool TryRun();
	#pragma endregion


	#pragma region Getter
	/// @brief ステートを取得
	template<typename StateT, typename ObjT>
	requires state_concepts::StateT<StateT, ObjT>
	[[nodiscard]] std::shared_ptr<StateT> GetState()
	{
		return m_states.count(typeid(StateT)) ? std::static_pointer_cast<StateT>(m_states.at(typeid(StateT))) : nullptr;
	}

	[[nodiscard]] std::type_index GetCurrentMoveStateType()   const{ return typeid(*m_move_state  .at(TimeKind::kCurrent).get()); }
	[[nodiscard]] std::type_index GetCurrentActionStateType() const{ return typeid(*m_action_state.at(TimeKind::kCurrent).get()); }
	#pragma endregion

private:
	void ChangeState(std::shared_ptr<Player>& player);

private:
	std::unordered_map<TimeKind, std::shared_ptr<IState<Player>>>			m_move_state;
	std::unordered_map<TimeKind, std::shared_ptr<IState<Player>>>			m_action_state;

	std::unordered_map<std::type_index, std::shared_ptr<IState<Player>>>	m_states;
};
