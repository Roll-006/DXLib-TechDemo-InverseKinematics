#pragma once
#include <string>
#include <unordered_map>

#include "../Base/singleton_base.hpp"
#include "../Base/obj_base.hpp"
#include "../Concept/obj_concepts.hpp"

#include "../Handle/handle_creator.hpp"

class ObjManager final : public SingletonBase<ObjManager>
{
public:
	template<obj_concepts::ObjT ObjT>
	void AddObj(const std::shared_ptr<ObjT>& obj)
	{
		// 上書き不可
		if (!m_objects.count(obj->GetObjHandle()))
		{
			m_objects[obj->GetObjHandle()] = obj;
		}
	}
	void RemoveObj(const int obj_handle);

	/// @brief オブジェクトが存在しているかを判定
	[[nodiscard]] bool IsExist(const int obj_handle) { return m_objects.count(obj_handle); }

	/// @brief オブジェクトを取得
	/// @param obj_handle オブジェクトハンドル
	template<obj_concepts::ObjT ObjT>
	[[nodiscard]] std::shared_ptr<ObjT> GetObj(const int obj_handle)
	{
		return m_objects.count(obj_handle) ? std::dynamic_pointer_cast<ObjT>(m_objects.at(obj_handle)) : nullptr;
	}

	/// @brief オブジェクトを取得
	/// @param obj_name オブジェクト名(同じキャラクターが存在している場合はオブジェクトハンドルでの取得を推奨)
	template<obj_concepts::ObjT ObjT>
	[[nodiscard]] std::shared_ptr<ObjT> GetObj(const std::string& obj_name)
	{
		for (const auto& obj : m_objects)
		{
			if (obj.second->GetName() == obj_name)
			{
				return std::dynamic_pointer_cast<ObjT>(obj.second);
			}
		}

		return nullptr;
	}

	/// @brief オブジェクトハンドルを生成
	[[nodiscard]] int CreateObjHandle() { return HandleCreator::GetInstance()->CreateHandle(); }

private:
	ObjManager();
	~ObjManager() override;

private:
	std::unordered_map<int, std::shared_ptr<ObjBase>> m_objects;

	friend SingletonBase<ObjManager>;
};
