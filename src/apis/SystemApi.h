#ifndef OPENCEMULATOR_SYSTEMAPI_H
#define OPENCEMULATOR_SYSTEMAPI_H


#include "Api.h"

class SystemApi : public Api {
public:
    SystemApi(Instance *instance);

    
    virtual void load() override;
    
public:
    static int lprint(lua_State *state);
    static int lallowBytecode(lua_State *state);
    static int lallowGC(lua_State *state);
    static int ltimeout(lua_State *state);
};


#endif //OPENCEMULATOR_SYSTEMAPI_H
