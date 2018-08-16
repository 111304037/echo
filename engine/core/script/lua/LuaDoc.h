//Lua�ʼ�-����lua table��C API
//ת����ע������yuliying��CSDN����.
//Lua�汾5.2
	
/*���API:
	http://pgl.yoyo.org/luai/i/_

	lua_createtable
	ԭ��: void lua_createtable (lua_State *L, int narr, int nrec);
	����: ����һ���µ�table����֮����ջ��.narr�Ǹ�table���鲿�ֵĳ���,nrec�Ǹ�table hash���ֵĳ���.
		  ������ȷ�е�֪��Ҫ�Ŷ���Ԫ�ص�table��ʱ��,ʹ���������,lua����Ԥ����һЩ�ڴ�,��������.
		  �����ȷ��Ҫ��Ŷ���Ԫ�ؿ���ʹ�� lua_newtable ����������table.

	lua_newtable
	ԭ��: void lua_newtable (lua_State *L);
	����: ����һ���µ�table����֮����ջ��. ��ͬ��lua_createtable(L, 0, 0).

	lua_getfield
	ԭ��: void lua_getfield (lua_State *L, int index, const char *k);
	����: ��t[k]Ԫ��push��ջ��. ����t��index����table.
		  ����������ܴ���indexԪ����.

	lua_setfield
	ԭ��: void lua_setfield (lua_State *L, int index, const char *k);
	����: Ϊtable�е�key��ֵ. t[k] = v . ����t��index����table , vΪջ��Ԫ��.
		  ����������ܴ���newindexԪ����.
		  ������ɺ󵯳�ջ��Ԫ��(value).

	lua_gettable
	ԭ��: void lua_gettable (lua_State *L, int index);
	����: ��t[k]Ԫ��push��ջ��. ����t��index����table,kΪջ��Ԫ��.
		  ����������ܴ���indexԪ����.
		  ������ɺ󵯳�ջ��Ԫ��(key).

	lua_settable
	ԭ��: void lua_settable (lua_State *L, int index);
	����: Ϊtable�е�key��ֵ. t[k] = v . ����t��index����table , vΪջ��Ԫ��. kΪ-2����Ԫ��.
		  ����������ܴ���newindexԪ����.
		  ������ɺ󵯳�ջ������Ԫ��(key , value)

	lua_rawget
	ԭ��: void lua_rawget (lua_State *L, int index);
	����: ��lua_gettable��������, ���ǲ��ᴥ��__indexԪ����.

	lua_rawset
	ԭ��: void lua_rawset (lua_State *L, int index);
	����: ��lua_settable��������, ���ǲ��ᴥ��newindexԪ����.

	lua_rawgeti
	ԭ��: void lua_rawgeti (lua_State *L, int index, int n);
	����: ��t[n]Ԫ��push��ջ��.����t��index����table.
		  ����������ᴥ��indexԪ����.

	lua_rawseti
	ԭ��: void lua_rawseti (lua_State *L, int index, int n);
	����: Ϊtable�е�key��ֵ. t[n] = v .����t��index����table , vΪջ��Ԫ��.
		  ����������ᴥ��newindexԪ����.
		  ������ɺ󵯳�ջ��Ԫ��.

	lua_rawgetp
	ԭ��: void lua_rawgetp (lua_State *L, int index, const void *p);
	����: ��t[p]Ԫ��push��ջ��.����t��index����table. p��һ��lightuserdata.
		  ����������ᴥ��indexԪ����.

	lua_rawsetp
	ԭ��: void lua_rawsetp (lua_State *L, int index, const void *p);
	����: Ϊtable�е�key��ֵ. t[p] = v .����t��index����table , p��һ��lightuserdata , vΪջ��Ԫ��.
		  ����������ᴥ��newindexԪ����.
		  ������ɺ󵯳�ջ��Ԫ��.

	lua_getmetatable
	ԭ��: int lua_getmetatable (lua_State *L, int index);
	����: ��index��Ԫ�ص�Ԫ��push��ջ��. �����Ԫ��û��Ԫ��, ��������0 , ���ı�ջ.

	lua_setmetatable
	ԭ��: void lua_setmetatable (lua_State *L, int index);
	����: ��ջ��Ԫ������Ϊindex��Ԫ�ص�Ԫ��.
		  ������ɺ󵯳�ջ��Ԫ��.

	lua_istable
	ԭ��: int lua_istable (lua_State *L, int index);
	����: �ж�index��Ԫ���Ƿ�Ϊһ��table , ����Ƿ���1,���򷵻�0.

	lua_pushglobaltable
	ԭ��: void lua_pushglobaltable (lua_State *L);
	����: ��lua��ȫ�ֱ����ջ��.

	luaL_newmetatable
	ԭ��: int luaL_newmetatable (lua_State *L, const char *tname);
	����: ���ע������Ѿ�����Ϊtname��key,�򷵻�0.
		  ���򴴽�һ����table��Ϊuserdata��Ԫ��. ���Ԫ��洢��ע�����,����tnameΪkey. ����1.
		  ������ɺ󽫸�Ԫ������ջ��.

	luaL_getmetatable
	ԭ��: void luaL_getmetatable (lua_State *L, const char *tname);
	����: ��ע�������tnameΪkey��Ԫ��push��ջ��.

	luaL_setmetatable
	ԭ��: void luaL_setmetatable (lua_State *L, const char *tname);
	����: ��ջ��Ԫ�ش洢��ע�����, ����keyΪtname.

	luaL_getsubtable
	ԭ��: int luaL_getsubtable (lua_State *L, int idx, const char *fname);
	����: �� t[fname] push��ջ��, ����t��index����table , ���� t[fname] ҲΪһ��table.
	��� t[fname] ԭ���ʹ���,���� true ,���򷵻�false,���ҽ� t[fname] �½�Ϊһ�ſձ�.

	lua_getglobal
	ԭ��: void lua_getglobal (lua_State *L, const char *name);
	����: �� t[name] Ԫ��push��ջ��, ����tΪȫ�ֱ�.

	lua_setglobal
	ԭ��: void lua_setglobal (lua_State *L, const char *name);
	����: Ϊtable�е�key��ֵ. t[name] = v . ����tΪȫ�ֱ�. vΪջ��Ԫ��.
	������ɺ󵯳�ջ��Ԫ��(v).

	luaL_newlibtable
	ԭ��: void luaL_newlibtable (lua_State *L, const luaL_Reg l[]);
	����: ����һ�ſձ�, luaԤ�ȷ����㹻���ڴ������洢���Ǵ����ĺ�����.
	�Ժ����ǿ���ʹ�� luaL_setfuncs ����ע�����ǵĺ�����.

	luaL_setfuncs
	ԭ��: void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup);
	����: ������ luaL_Reg�����еĺ���ע�ᵽջ����table��.
		  ��upvalue������Ϊ0ʱ,�����������к���������Щupvalue. -2��-(nup+1)��Ԫ��ΪҪע���upvalue.
		  (ע��:��Щupvalue��c�е�upvalue,����lua�е�upvalue,������ע���c������ͨ�� lua_upvalueindex(n)��ȡ��ֵ.)
		  ������ɺ󵯳�ջ��������upvalue.

	luaL_newlib
	ԭ��: void luaL_newlib (lua_State *L, const luaL_Reg *l);
	����: ����һ���µ�table , ����luaL_Reg�����еĺ���ע�ᵽ����.
		  ����һ���� (luaL_newlibtable(L,l), luaL_setfuncs(L,l,0))

	lua_next
	ԭ��: int lua_next (lua_State *L, int index);
	����: �ú�����������һ��table.
		  ��ջ������һ��key , ����pushһ�� key-value��(ջ��key����һ����ֵ��) ,��ջ��.
		  ���table��û�и����Ԫ��, ��������0.
		  ������ʼʱջ��Ϊһ��nil , ����ȡ����һ����ֵ��.

	ͨ����������Ϊ:
	lua_pushnil(L);  // first key
	while (lua_next(L, t) != 0) {
		// uses 'key' (at index -2) and 'value' (at index -1)
		printf("%s - %s\n",
		lua_typename(L, lua_type(L, -2)),
		lua_typename(L, lua_type(L, -1)));
		// removes 'value'; keeps 'key' for next iteration
		lua_pop(L, 1);
	}
	ע��: �ڱ���table��ʱ�� ,������ȷ��֪��keyΪ�ַ���,��Ҫ��ջ�ϵ�keyʹ�� lua_tolstring ���� ,
		  ��Ϊ�����п��ܸı�key������ , Ӱ����һ�� lua_next����.

	lua_rawlen
	ԭ��: size_t lua_rawlen (lua_State *L, int index);
	����: ��ȡindex��Ԫ�صĳ���.
		  �����ַ�����˵,�����ַ�������.
		  ����table��˵,����#�������ĳ���. ����Ԫ����Ӱ��.
		  ����userdata��˵,�����ڴ�Ĵ�С.
		  ����Ԫ�ط���0.

	lua_len
	ԭ��: void lua_len (lua_State *L, int index);
	����: ��ȡindex��Ԫ��#�������Ľ�� , ������ջ��.


	��������:
		1.α����:
			Luaջ���������� ��ջ����,ջ��Ϊ-1,��ջ�͵ݼ�. ��ջ����,ջ��Ϊ1,��ջ������.
			α������һ������,������ջ��λ����,ͨ��һ����������α������λ��.
			α��������������ע���,������lua_CFunction�з���upvalue.
		2.ע���:
			Lua��ע�����һ��Ԥ�����table, �����ṩ��c api�洢һ����Ҫ�洢��ֵ.
			ע���ͨ�� LUA_REGISTRYINDEX α����������.
			���� lua_getfield ������������������ʹ������ȡע����е�һ����"hello"Ϊkey��ֵ :
			lua_getfield( L , LUA_REGISTRYINDEX , "hello");
		3. upvalue:
			��ʹ�� lua_pushcfunction ���� luaL_setfuncs ��һ��lua_CFunction ע�ᵽLua������ʱ,
			����ͬʱΪ�����������һЩupvalue .
			��������Щlua_CFunction �п���ʹ�� lua_upvalueindex(n) ��������ȡ��Ӧλ�õ�upvalue.


	lua_pushcclosure
	ԭ��:void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n);
	����:Pushes a new C closure onto the stack.
		 When a C function is created, it is possible to associate some values with it, thus creating 
		 a C closure (see ��3.4); these values are then accessible to the function whenever it is called. 
		 To associate values with a C function, first these values should be pushed onto the stack 
		 (when there are multiple values, the first value is pushed first). Then lua_pushcclosure is 
		 called to create and push the C function onto the stack, with the argument n telling how many 
		 values should be associated with the function. lua_pushcclosure also pops these values from 
		 the stack.
		 The maximum value for n is 255.
*/