#pragma once
#include <memory>
#include <vector>
#include "../Concept/state_concepts.hpp"

template<obj_concepts::ObjT ObjT>
class IStateController abstract
{
public:
	virtual ~IStateController() = default;

	virtual void Init		(std::shared_ptr<ObjT> obj) abstract;
	virtual void Update		(std::shared_ptr<ObjT> obj) abstract;
	virtual void LateUpdate	(std::shared_ptr<ObjT> obj) abstract;

protected:
	virtual void CreateState()			abstract;
	virtual void AddStopStatePair()		abstract;
	virtual void AddCheckStopState()	abstract;

	/// @brief ステートを変更
	virtual void ChangeState(std::shared_ptr<ObjT>& obj) abstract;

	/// @brief 変更するステートを生成
	[[nodiscard]] virtual std::vector<std::shared_ptr<IState<ObjT>>> CreateChangeState(std::shared_ptr<ObjT>& obj) abstract;

	/// @brief 未来のステート構成を生成
	[[nodiscard]] virtual std::vector<std::shared_ptr<IState<ObjT>>> CreateFutureState(const std::vector<std::shared_ptr<IState<ObjT>>>& next_state) abstract;

	/// @brief ステートの停止処理
	virtual void StopState(std::vector<std::shared_ptr<IState<ObjT>>>& future_state, const std::shared_ptr<IState<ObjT>>& stop_state) abstract;
};
