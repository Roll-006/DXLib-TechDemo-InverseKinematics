#pragma once
#include <memory>
#include "../Kind/scene_kind.hpp"

class IScene abstract
{
public:
	virtual ~IScene() = default;

	virtual void Init()						abstract;
	virtual void Update()					abstract;
	virtual void LateUpdate()				abstract;
	virtual void DrawToShadowMap()	const	abstract;
	virtual void Draw()				const	abstract;

	/// @brief アクティブ化する
	virtual void Activate() abstract;

	/// @brief 非アクティブ化する
	virtual void Deactivate() abstract;

	[[nodiscard]] virtual bool						IsActive()		const	abstract;
	[[nodiscard]] virtual std::shared_ptr<IScene>	ChangeScene()			abstract;
	[[nodiscard]] virtual SceneKind					GetSceneKind()	const	abstract;
};
