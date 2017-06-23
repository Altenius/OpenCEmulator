#include <iostream>
#include "ComponentApi.h"
#include "../Instance.h"



ComponentApi::ComponentApi(Instance *instance) : Api(instance)
{

}



static const luaL_Reg componentlib[] = {
        {"list",    ComponentApi::llist},
        {"type",    ComponentApi::ltype},
        {"slot",    ComponentApi::lslot},
        {"methods", ComponentApi::lmethods},
        {"invoke",  ComponentApi::linvoke},
        {NULL, NULL}
};



void ComponentApi::load()
{
    lua_State *state = m_instance->state();

    luaL_newlib(state, componentlib);
    lua_setglobal(state, "component");
}



int ComponentApi::llist(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    std::string filter = lua_isstring(state, 1) ? lua_tostring(state, 1) : "";
    bool exact = lua_isboolean(state, 1) ? lua_toboolean(state, 1) == 1 : true;

    std::vector<ComponentWeakPtr> &components = instance->components();

    lua_createtable(state, 0, components.size());
    for (ComponentWeakPtr &c : components) {
        if (auto component = c.lock()) {
            if (filter.empty() || (exact && component->type() == filter) || (!exact &&
                                                                             std::search(component->type().begin(),
                                                                                         component->type().end(),
                                                                                         filter.begin(),
                                                                                         filter.end()) !=
                                                                             component->type().end())) {
                lua_pushstring(state, component->uuid().data());
                lua_pushstring(state, component->type().data());
                lua_rawset(state, -3);
            }
        }
    }

    return 1;
}



int ComponentApi::ltype(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    std::string address = luaL_checkstring(state, 1);

    ComponentPtr component = getComponent(instance, address);
    if (component == nullptr) {
        lua_pushnil(state);
        lua_pushstring(state, "no such component");
        return 2;
    }

    lua_pushstring(state, component->type().data());
    return 1;
}



int ComponentApi::lslot(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    std::string address = luaL_checkstring(state, 1);

    ComponentPtr component = getComponent(instance, address);
    if (component == nullptr) {
        lua_pushnil(state);
        lua_pushstring(state, "no such component");
        return 2;
    }

    lua_pushnumber(state, -1);

    return 1;
}



int ComponentApi::lmethods(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    std::string address = luaL_checkstring(state, 1);

    ComponentPtr component = getComponent(instance, address);
    if (component == nullptr) {
        lua_pushnil(state);
        lua_pushstring(state, "no such component");
        return 2;
    }

    auto &info = component->methodsinfo();

    lua_createtable(state, 0, info.size());
    for (auto it = info.begin(); it != info.end(); ++it) {
        lua_pushstring(state, it->first.c_str());
        lua_newtable(state);
        lua_pushboolean(state, it->second.direct);
        lua_setfield(state, -2, "direct");
        lua_pushboolean(state, it->second.getter);
        lua_setfield(state, -2, "getter");
        lua_pushboolean(state, it->second.setter);
        lua_setfield(state, -2, "setter");
        lua_rawset(state, -3);
    }

    return 1;
}



int ComponentApi::linvoke(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    std::string address = luaL_checkstring(state, 1);
    std::string method = luaL_checkstring(state, 2);
    ArgList args(state, 3);

    ComponentPtr component = getComponent(instance, address);
    if (component == nullptr) {
        lua_pushnil(state);
        lua_pushstring(state, "no such component");
        return 2;
    }

    auto &methods = component->methods();
    auto it = methods.find(method);
    if (it == methods.end()) {
        lua_pushnil(state);
        lua_pushstring(state, "no such method");
        return 2;
    }

    ArgList out;

    bool res = it->second(args, out);
    lua_pushboolean(state, res);

    out.dump(state);

    return out.size() + 1;
}



int ComponentApi::ldoc(lua_State *state)
{
    Instance *instance = Instance::fromState(state);
    std::string address = luaL_checkstring(state, 1);
    std::string method = luaL_checkstring(state, 2);

    ComponentPtr component = getComponent(instance, address);
    if (component == nullptr) {
        lua_pushnil(state);
        lua_pushstring(state, "no such component");
        return 2;
    }

    auto &info = component->methodsinfo();
    auto it = info.find(method);
    if (it == info.end()) {
        lua_pushnil(state);
        lua_pushstring(state, "no such method");
        return 2;
    }

    lua_pushstring(state, it->second.doc.c_str());
    return 1;
}



ComponentPtr ComponentApi::getComponent(Instance *instance, const std::string &address)
{
    std::vector<ComponentWeakPtr> &components = instance->components();
    auto it = std::find_if(components.begin(), components.end(), [address](ComponentWeakPtr &c) -> bool {
        if (auto comp = c.lock()) {
            return comp->uuid() == address;
        }
        return false;
    });
    if (it == components.end()) {
        return nullptr;
    }

    return it->lock();
}