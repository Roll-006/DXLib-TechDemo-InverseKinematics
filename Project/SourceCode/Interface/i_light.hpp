#pragma once
#include "../Name/light_name.hpp"
#include "../Kind/light_kind.hpp"

class ILight abstract
{
public:
	virtual ~ILight() = default;

	virtual void Activate()		abstract;
	virtual void Deactivate()	abstract;

	[[nodiscard]] virtual LightKind		GetLightKind()		const abstract;
	[[nodiscard]] virtual std::string	GetName()			const abstract;
	[[nodiscard]] virtual int			GetLightHandle()	const abstract;
	[[nodiscard]] virtual int			GetPriority()		const abstract;
	[[nodiscard]] virtual bool			IsActive()			const abstract;

protected:

};
