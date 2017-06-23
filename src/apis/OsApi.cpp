#include "OsApi.h"
#include "../Instance.h"
#include "Instance.h"



OsApi::OsApi(Instance *instance) : Api(instance)
{

}



void OsApi::load()
{
    lua_State *state = m_instance->state();
    luaopen_os(state);

    lua_pushcfunction(state, OsApi::lclock);
    lua_setfield(state, -2, "clock");

    lua_setglobal(state, "os");
}



int OsApi::lclock(lua_State *state)
{
    Instance *instance = Instance::fromState(state);

    lua_pushnumber(state, instance->cpuTime());
    return 1;
}
