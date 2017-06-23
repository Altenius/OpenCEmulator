#ifndef OPENCEMULATOR_OSAPI_H
#define OPENCEMULATOR_OSAPI_H


#include "Api.h"

class OsApi : public Api {
public:
    OsApi(Instance *instance);

    virtual void load() override;
    
private:
    static int lclock(lua_State *state);
    // os.date and os.time should be overridden
};


#endif //OPENCEMULATOR_OSAPI_H
