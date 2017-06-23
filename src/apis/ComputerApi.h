#ifndef OPENCEMULATOR_COMPUTERAPI_H
#define OPENCEMULATOR_COMPUTERAPI_H


#include "Api.h"

class ComputerApi : public Api
{
public:
    ComputerApi(Instance *instance);

    virtual void load() override;

public:
    static int lrealTime(lua_State *state);

    static int luptime(lua_State *state);

    static int laddress(lua_State *state);

    static int lfreeMemory(lua_State *state);

    static int ltotalMemory(lua_State *state);

    static int lpushSignal(lua_State *state);

    static int ltmpAddress(lua_State *state);

    static int lusers(lua_State *state);

    static int laddUser(lua_State *state);

    static int lremoveUser(lua_State *state);

    static int lenergy(lua_State *state);

    static int lmaxEnergy(lua_State *state);

    static int lgetArchitectures(lua_State *state);

    static int lgetArchitecture(lua_State *state);

    static int lsetArchitecture(lua_State *state);
};


#endif //OPENCEMULATOR_COMPUTERAPI_H
