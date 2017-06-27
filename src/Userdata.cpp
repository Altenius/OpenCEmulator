#include "Userdata.h"
#include <cstring>



void Userdata::allocate(lua_State *state)
{
    void *mem = lua_newuserdata(state, size());
    init(mem);
}



Userdata::Userdata(const std::string type) : m_type(type)
{

}
