#pragma once
#include "../Base/obj_base.hpp"
#include "../Part/modeler.hpp"
#include "../GameTime/game_time_manager.hpp"

class Skydome final : public ObjBase
{
public:
	Skydome();
	~Skydome() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() { return m_modeler; }
	
private:
	static constexpr float  kBasicScale = 17.0f;

private:
	std::shared_ptr<Modeler> m_modeler;
};
