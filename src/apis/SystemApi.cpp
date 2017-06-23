#include <iostream>
#include "SystemApi.h"
#include "Instance.h"



SystemApi::SystemApi(Instance *instance) : Api(instance)
{

}



static const luaL_Reg systemlib[] = {
        {"allowBytecode", SystemApi::lallowBytecode},
        {"allowGC",       SystemApi::lallowGC},
        {"timeout",       SystemApi::ltimeout},
        {NULL, NULL}
};



void SystemApi::load()
{
    lua_State *state = m_instance->state();

    luaL_newlib(state, systemlib);
    lua_setglobal(state, "system");

    luaopen_math(state);
    lua_setglobal(state, "math");
    luaopen_bit32(state);
    lua_setglobal(state, "bit32");
    luaopen_coroutine(state);
    lua_setglobal(state, "coroutine");
    luaopen_debug(state);
    lua_setglobal(state, "debug");
    luaopen_eris(state);
    lua_setglobal(state, "eris");
    luaopen_string(state);
    lua_setglobal(state, "string");
    luaopen_table(state);
    lua_setglobal(state, "table");
    luaopen_base(state);
    lua_pop(state, 1);

    lua_pushcfunction(state, lprint);
    lua_setglobal(state, "print");
}



int SystemApi::lprint(lua_State *state)
{
    for (int i = 1; i <= lua_gettop(state); i++) {
        if (i != 1) {
            std::cout << "  ";
        }
        switch (lua_type(state, i)) {
            case LUA_TNUMBER:
            case LUA_TSTRING:
                std::cout << lua_tostring(state, i);
                break;
            case LUA_TNIL:
                std::cout << "nil";
                break;
            case LUA_TBOOLEAN:
                std::cout << (lua_toboolean(state, i) == 1 ? "true" : "false");
                break;
            case LUA_TTABLE:
                std::cout << "table";
                break;
            case LUA_TFUNCTION:
                std::cout << "function";
                break;
            case LUA_TTHREAD:
                std::cout << "thread";
                break;
            case LUA_TLIGHTUSERDATA:
            case LUA_TUSERDATA:
                std::cout << "userdata";
                break;
            default:
                std::cout << "???";
                break;
        }
    }
    std::cout << std::endl;
    return 0;
}



int SystemApi::lallowBytecode(lua_State *state)
{
    // TODO: Use setting when settings are implemented
    lua_pushboolean(state, false);
    return 1;
}



int SystemApi::lallowGC(lua_State *state)
{
    // TODO: Use setting when settings are implemented
    lua_pushboolean(state, false);
    return 1;
}



int SystemApi::ltimeout(lua_State *state)
{
    // TODO: Use setting when settings are implemented
    lua_pushnumber(state, 3);
    return 1;
}
