//
// Lua Example
//
// 2016��6��1�� 10:51:34
//

#pragma once
#include "Frame/Example.h"

struct lua_State; 
typedef int(*lua_CFunction) (lua_State *L);

namespace Examples
{
	class LuaModule : public Example
	{
		LuaModule(const LuaModule& rhs);
		LuaModule& operator=(const LuaModule& rhs);

	public: 
		LuaModule();
		virtual ~LuaModule();

	public: 
		// ������Ϣ
		virtual const LORD::String getHelp();

		// ��ʼ��
		virtual bool initialize();

		// ����
		virtual void tick(LORD::ui32 elapsedTime);

		// ����
		virtual void destroy();

		// �����¼� 
		virtual void keyboardProc(LORD::ui32 keyChar, bool isKeyDown) override;

		// ����¼�
		virtual void mouseLBProc(LORD::i32 xpos, LORD::i32 ypos) override;

		// ����¼�
		virtual void mouseMoveProc(LORD::i32 xpos, LORD::i32 ypos) override;

		// �����¼�
		virtual void mouseWhellProc(LORD::i32 parma) override; 

	private:
		void addLuaLoader(lua_State* L, lua_CFunction func); 

		bool doLua(lua_State* L, const LORD::String& fileName); 

	private:
		lua_State*							m_pState; 
	};
}