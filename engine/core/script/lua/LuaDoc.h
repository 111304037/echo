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
	prototype  : int lua_setmetatable (lua_State *L, int index);
	description: Pops a table from the stack and sets it as the new metatable for the value at the given acceptable index.

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
	prototype  : void lua_setglobal (lua_State *L, const char *name);
	description: Pops a value from the stack and sets it as the new value of global name. 
				 It is defined as a macro:
				 #define lua_setglobal(L,s)   lua_setfield(L, LUA_GLOBALSINDEX, s)

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
	prototype  : void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n);
	description: Pushes a new C closure onto the stack.
				 When a C function is created, it is possible to associate some values with it, thus creating 
				 a C closure (see ��3.4); these values are then accessible to the function whenever it is called. 
				 To associate values with a C function, first these values should be pushed onto the stack 
				 (when there are multiple values, the first value is pushed first). Then lua_pushcclosure is 
				 called to create and push the C function onto the stack, with the argument n telling how many 
				 values should be associated with the function. lua_pushcclosure also pops these values from 
				 the stack.
				 The maximum value for n is 255.

	lua_gettop
	prototype  ; int lua_gettop (lua_State *L);
	description: The lua_gettop function returns the number of elements in the stack, which is also the 
				 index of the top element. Notice that a negative index -x is equivalent to the positive 
				 index gettop - x + 1.

	lua_settop
	prototype  : lua_settop( lua_State *L, int idx)
	description: lua_settop sets the top (that is, the number of elements in the stack) to a specific value. 
				 If the previous top was higher than the new one, the top values are discarded. Otherwise, 
				 the function pushes nils on the stack to get the given size. As a particular case, lua_settop(L, 0) 
				 empties the stack. You can also use negative indices with lua_settop; that will set the top 
				 element to the given index. Using this facility, the API offers the following macro, which 
				 pops n elements from the stack:
				 #define lua_pop(L,n)  lua_settop(L, -(n)-1)

	lua_pushliteral
	prototype  :void lua_pushliteral (lua_State *L, const char *s);
	description:This macro is equivalent to lua_pushlstring, but can be used only when s is a literal string. 
				In these cases, it automatically provides the string length.

	lua_pushvalue
	prototype  :void lua_pushvalue (lua_State *L, int index);
	description:Pushes a copy of the element at the given valid index onto the stack

	lua_settable
	prototype  :void lua_settable (lua_State *L, int index);
	description:Does the equivalent to t[k] = v, where t is the value at the given valid index, v is the value 
				at the top of the stack, and k is the value just below the top.

				This function pops both the key and the value from the stack. As in Lua, this function may 
				trigger a metamethod for the "newindex" event (see ��2.8)

	lua_pop
	prototype  :void lua_pop (lua_State *L, int n);
	description:Pops n elements from the stack.
*/