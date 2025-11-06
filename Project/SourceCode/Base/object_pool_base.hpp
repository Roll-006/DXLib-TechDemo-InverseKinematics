#pragma once
#include <queue>

#include "obj_base.hpp"
#include "../Name/object_pool_name.hpp"

class ObjectPoolBase abstract
{
public:
	ObjectPoolBase(const std::string& name);
	virtual ~ObjectPoolBase() = default;

	/// @brief 使用後のオブジェクトを返却する
	/// @brief 自動的に非アクティブ化される
	void ReturnObj(const std::shared_ptr<ObjBase>& obj);

	/// @brief オブジェクトをプール内から破棄する
	/// @brief オブジェクト名を指定し、一致するオブジェクトをすべて破棄
	/// @brief WARNING : 各マネージャーの登録も解除される(RemoveToObjManagerが呼ばれる)
	void DestroyObjects(const std::string& obj_name);

	/// @brief 使用可能なオブジェクトを取得する
	/// @return 初期化・アクティブ化されたオブジェクト(使用可能なオブジェクトがない場合はnullptr)
	[[nodiscard]] std::shared_ptr<ObjBase> GetObj(const std::string& obj_name);

	[[nodiscard]] int GetMaxPoolSize(const std::string& obj_name) const;
	[[nodiscard]] int GetPoolSize   (const std::string& obj_name) const;
	[[nodiscard]] std::string GetName() const { return m_name; }

protected:
	/// @brief オブジェクトを生成
	/// @brief コンストラクタ時に呼び出される
	void CreateObj(const std::shared_ptr<ObjBase> obj);

protected:
	std::unordered_map<std::string, int> m_pool_size;
	std::unordered_map<std::string, std::queue<std::shared_ptr<ObjBase>>> m_objects;

	std::string m_name;
};
