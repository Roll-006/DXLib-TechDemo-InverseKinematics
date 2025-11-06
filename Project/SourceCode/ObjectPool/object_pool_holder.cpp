#include "object_pool_holder.hpp"

ObjectPoolHolder::ObjectPoolHolder()
{
	// ˆ—‚È‚µ
}

ObjectPoolHolder::~ObjectPoolHolder()
{
	// ˆ—‚È‚µ
}

void ObjectPoolHolder::RemoveObjectPool(const std::string& object_pool_name)
{
	m_object_pools.erase(object_pool_name);
}

std::shared_ptr<ObjectPoolBase> ObjectPoolHolder::GetObjectPool(const std::string& object_pool_name)
{
	return m_object_pools.count(object_pool_name) ? m_object_pools.at(object_pool_name) : nullptr;
}
