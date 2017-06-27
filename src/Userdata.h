#ifndef OPENCEMULATOR_USERDATA_H
#define OPENCEMULATOR_USERDATA_H

#include "lua.hpp"
#include <string>

class Userdata
{
public:
    Userdata(const std::string type);

    void allocate(lua_State *state);

    virtual size_t size() =0;

    virtual void init(void *memory) =0;

    virtual std::string &type() =0;

private:
    std::string m_type;
};


#endif //OPENCEMULATOR_USERDATA_H
