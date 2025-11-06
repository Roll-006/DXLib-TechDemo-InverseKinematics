#include "../Base/obj_base.hpp"
#include "obj_manager.hpp"

ObjManager::ObjManager()
{
	// ˆ—‚È‚µ
}

ObjManager::~ObjManager()
{
	// ˆ—‚È‚µ
}

void ObjManager::RemoveObj(const int obj_handle)
{
	m_objects.erase(obj_handle);
}
