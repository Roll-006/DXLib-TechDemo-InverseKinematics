#pragma once
#include <memory>
#include "../Concept/obj_concepts.hpp"

template<obj_concepts::CharacterT CharacterT>
class IState abstract
{
public:
	virtual ~IState() = default;

	virtual void Update		(std::shared_ptr<CharacterT>& character) abstract;
	virtual void Enter		(std::shared_ptr<CharacterT>& character) abstract;
	virtual void Exit		(std::shared_ptr<CharacterT>& character) abstract;

	[[nodiscard]] virtual std::shared_ptr<IState<CharacterT>> ChangeState(std::shared_ptr<CharacterT>& character) abstract;
};
