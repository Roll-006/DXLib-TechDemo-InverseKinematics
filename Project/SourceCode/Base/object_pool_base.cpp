#include "object_pool_base.hpp"

ObjectPoolBase::ObjectPoolBase(const std::string& name) : 
	m_name(name)
{

}

void ObjectPoolBase::ReturnObj(const std::shared_ptr<ObjBase>& obj)
{
	if (!m_objects.count(obj->GetName())) { return; }

	// 非アクティブ化して追加
	obj->Deactivate();
	m_objects[obj->GetName()].push(obj);
}

void ObjectPoolBase::DestroyObjects(const std::string& obj_name)
{
	while (!m_objects.at(obj_name).empty())
	{
		const auto obj = m_objects.at(obj_name).front();
		auto i = obj.use_count();
		obj->RemoveToObjManager();
		i = obj.use_count();
		m_objects.at(obj_name).pop();
	}

	m_objects.erase(obj_name);
}

std::shared_ptr<ObjBase> ObjectPoolBase::GetObj(const std::string& obj_name)
{
	if (!m_objects.count(obj_name))     { return nullptr; }
	if (m_objects.at(obj_name).empty()) { return nullptr; }

	const auto obj = m_objects.at(obj_name).front();
	m_objects.at(obj_name).pop();

	// アクティブ化・初期化して取得
	obj->Activate();
	obj->Init();

	return obj;
}

int ObjectPoolBase::GetMaxPoolSize(const std::string& obj_name) const
{
	if (!m_pool_size.count(obj_name)) { return 0; }

	return m_pool_size.at(obj_name);
}

int ObjectPoolBase::GetPoolSize(const std::string& obj_name) const
{
	if (!m_objects.count(obj_name)) { return 0; }

	return static_cast<int>(m_objects.at(obj_name).size());
}

void ObjectPoolBase::CreateObj(const std::shared_ptr<ObjBase> obj)
{
	// 既に指定したプールサイズに到達している場合は追加を許可しない
	m_objects[obj->GetName()];
	if (m_objects.at(obj->GetName()).size() >= GetMaxPoolSize(obj->GetName())) { return; }

	m_objects.at(obj->GetName()).push(obj);

	// 非アクティブ化
	obj->Deactivate();
}
