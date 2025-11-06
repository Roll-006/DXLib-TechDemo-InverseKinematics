#pragma once

class IPoolable abstract
{
public:
	virtual ~IPoolable() = default;

	[[nodiscard]] virtual bool IsReturnPool() abstract;
};
