#include "ExportToLua.h"
#include "Engine/core/main/Root.h"
//#include <luaex.h>

namespace Echo
{
	// ע��c++������lua����
	void registerInterfaceToLua()
	{
		//luaex::LuaEx* luaEx = Root::instance()->getLuaEx();
		//if (!luaEx)
		//	return;

		//// ��־�ӿ�ע��
		//{
		//	// ��־��,��־�ຯ��ע��
		//	luaEx->register_class("LogManager");
		//	luaEx->register_function<LogManager, const char*>("LogManager", "error", &LogManager::error);
		//	luaEx->register_function<LogManager, const char*>("LogManager", "warning", &LogManager::warning);
		//	luaEx->register_function<LogManager, const char*>("LogManager", "info", &LogManager::info);

		//	// ��־����ע��
		//	luaEx->register_object("LogManager", "log", LogManager::instance());
		//}

		//// ����ӿ�ע��
		//{
		//	luaEx->register_class("GameObject");
		//	luaEx->register_function<GameObject, const char*>("GameObject", "activeComponent", &GameObject::activeComponent);
		//}
	}
}