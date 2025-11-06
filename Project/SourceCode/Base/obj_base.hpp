#pragma once
#include "../Name/obj_name.hpp"
#include "../Tag/obj_tag.hpp"

#include "../Part/transform.hpp"

class ObjManager;

class ObjBase abstract : public std::enable_shared_from_this<ObjBase>
{
public:
	ObjBase(const std::string& name, const std::string& tag);
	virtual ~ObjBase() = default;

	virtual void Init()						abstract;
	virtual void Update()					abstract;
	virtual void LateUpdate()				abstract;
	virtual void Draw()				const	abstract;

	/// @brief 各マネージャーに自身を登録する
	virtual void AddToObjManager()			abstract;
	virtual void RemoveToObjManager()		abstract;

	/// @brief オブジェクトをアクティブ化する
	void Activate()   { m_is_active = true; }
	/// @brief オブジェクトを非アクティブ化する(削除せずに機能を停止)
	void Deactivate() { m_is_active = false; }


	#pragma region Getter
	[[nodiscard]] virtual float				 GetDeltaTime() const abstract;
	[[nodiscard]] std::shared_ptr<Transform> GetTransform()			{ return m_transform; }
	[[nodiscard]] std::string				 GetName()		const	{ return m_name; }
	[[nodiscard]] std::string				 GetTag()		const	{ return m_tag; }
	[[nodiscard]] int						 GetObjHandle()	const	{ return m_obj_handle; }
	[[nodiscard]] bool						 IsActive()		const	{ return m_is_active; }
	#pragma endregion

protected:
	std::shared_ptr<Transform> m_transform;

private:
	std::string m_name;		// オブジェクトの名前
	std::string m_tag;		// オブジェクトの分類

	bool m_is_active;
	int  m_obj_handle;

	friend void from_json	(const nlohmann::json& data, ObjBase& obj_base);
	friend void to_json		(nlohmann::json& data, const ObjBase& obj_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, ObjBase& obj_base)
{
	data.at("transform").get_to(*obj_base.m_transform.get());
}

inline void to_json(nlohmann::json& data, const ObjBase& obj_base)
{
	data = nlohmann::json
	{
		{ "transform", *obj_base.m_transform.get() },
	};
}
#pragma endregion
