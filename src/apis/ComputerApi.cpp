#include "ComputerApi.h"
#include "../Instance.h"
#include <chrono>
#include <cmath>
#include <iostream>



ComputerApi::ComputerApi(Instance *instance) : Api(instance)
{

}



static const luaL_Reg computerlib[] = {
        {"realTime",         ComputerApi::lrealTime},
        {"uptime",           ComputerApi::luptime},
        {"address",          ComputerApi::laddress},
        {"freeMemory",       ComputerApi::lfreeMemory},
        {"totalMemory",      ComputerApi::ltotalMemory},
        {"pushSignal",       ComputerApi::lpushSignal},
        {"tmpAddress",       ComputerApi::ltmpAddress},
        {"users",            ComputerApi::lusers},
        {"addUser",          ComputerApi::laddUser},
        {"removeUser",       ComputerApi::lremoveUser},
        {"energy",           ComputerApi::lenergy},
        {"maxEnergy",        ComputerApi::lmaxEnergy},
        {"getArchitectures", ComputerApi::lgetArchitectures},
        {"getArchitecture",  ComputerApi::lgetArchitecture},
        {"setArchitecture",  ComputerApi::lsetArchitecture},
        {NULL, NULL}
};



void ComputerApi::load()
{
    lua_State *state = m_instance->state();

    luaL_newlib(state, computerlib);
    lua_setglobal(state, "computer");
}



int ComputerApi::lrealTime(lua_State *state)
{
    lua_pushnumber(state, (lua_Number) (std::chrono::system_clock::now().time_since_epoch().count()
                                        * std::chrono::system_clock::period::num /
                                        std::chrono::system_clock::period::den) / 1000);
    return 1;
}



int ComputerApi::luptime(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    lua_pushnumber(state, instance->cpuTime());

    return 1;
}



int ComputerApi::laddress(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    lua_pushstring(state, instance->address().data());

    return 1;
}



int ComputerApi::lfreeMemory(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    lua_pushnumber(state, instance->memoryMax() - instance->memoryUsed());

    return 1;
}



int ComputerApi::ltotalMemory(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    lua_pushnumber(state, instance->memoryMax());

    return 1;
}



int ComputerApi::lpushSignal(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    lua_pushboolean(state, instance->signal(luaL_checkstring(state, 1), ArgList(state, 2)));

    return 1;
}



int ComputerApi::ltmpAddress(lua_State *state)
{
    // unimplemented
    lua_pushnil(state);
    return 1;
}



int ComputerApi::lusers(lua_State *state)
{
    // unimplemented. In the future, an array of names could be kept to simulate addUser and removeUser
    lua_newtable(state);
    return 1;
}



int ComputerApi::laddUser(lua_State *state)
{
    // unimplemented
    lua_pushboolean(state, true);
    return 1;
}



int ComputerApi::lremoveUser(lua_State *state)
{
    // unimplemented
    lua_pushnumber(state, true);
    return 1;
}



int ComputerApi::lenergy(lua_State *state)
{
    // unimplemented
    lua_pushnumber(state, HUGE_VAL);
    return 1;
}



int ComputerApi::lmaxEnergy(lua_State *state)
{
    // unimplemented
    lua_pushnumber(state, HUGE_VAL);
    return 1;
}



int ComputerApi::lgetArchitectures(lua_State *state)
{
    // unimplemented
    lua_newtable(state);
    return 1;
}



int ComputerApi::lgetArchitecture(lua_State *state)
{
    lua_pushstring(state, "Lua 5.2");
    return 1;
}



int ComputerApi::lsetArchitecture(lua_State *state)
{
    // unimplemented
    lua_pushnil(state);
    lua_pushstring(state, "unknown architecture");
    return 2;
}
