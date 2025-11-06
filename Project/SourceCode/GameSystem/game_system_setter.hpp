#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>

#include "../Base/single_instance_base.hpp"

#include "../Window/window.hpp"
#include "../Random/random_generator.hpp"

class GameSystemSetter final : public SingleInstanceBase<GameSystemSetter>
{
public:
	GameSystemSetter();
	~GameSystemSetter();

private:
	void SetUpGameSystem();
};
