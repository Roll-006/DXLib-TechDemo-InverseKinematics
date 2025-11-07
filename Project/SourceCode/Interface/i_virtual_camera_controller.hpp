#pragma once
#include "../Kind/virtual_camera_controller_kind.hpp"
#include "../VirtualCamera/virtual_camera.hpp"

class IVirtualCameraController abstract
{
public:
	virtual ~IVirtualCameraController() = default;

	virtual void Update()	  abstract;
	virtual void LateUpdate() abstract;

	/// @brief アクティブ化する
	virtual void Activate()   abstract;
	/// @brief 非アクティブ化する(削除せずに機能を停止)
	virtual void Deactivate() abstract;

	[[nodiscard]] virtual VirtualCameraControllerKind GetVirtualCameraControllerKind() const abstract;
	[[nodiscard]] virtual int GetControllerHandle() const abstract;
	[[nodiscard]] virtual bool IsActive() const abstract;
};
