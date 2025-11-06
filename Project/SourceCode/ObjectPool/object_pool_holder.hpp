#pragma once
#include "../Base/singleton_base.hpp"
#include "../Base/object_pool_base.hpp"

namespace object_pool_concepts
{
	/// @brief オブジェクトプール型である
	template<typename T>
	concept ObjectPoolT = std::is_base_of_v<ObjectPoolBase, T>;
}

class ObjectPoolHolder final : public SingletonBase<ObjectPoolHolder>
{
public:
	template<object_pool_concepts::ObjectPoolT ObjectPoolT>
	void AddObjectPool(const std::shared_ptr<ObjectPoolT>& object_pool)
	{
		if (!m_object_pools.count(object_pool->GetName()))
		{
			m_object_pools[object_pool->GetName()] = object_pool;
		}
	}
	void RemoveObjectPool(const std::string& object_pool_name);

	[[nodiscard]] std::shared_ptr<ObjectPoolBase> GetObjectPool(const std::string& object_pool_name);

private:
	ObjectPoolHolder();
	~ObjectPoolHolder() override;

private:
	std::unordered_map<std::string, std::shared_ptr<ObjectPoolBase>> m_object_pools;

	friend SingletonBase<ObjectPoolHolder>;
};
