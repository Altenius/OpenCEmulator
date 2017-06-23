#ifndef OPENCEMULATOR_COMPONENTAPI_H
#define OPENCEMULATOR_COMPONENTAPI_H


#include "Api.h"
#include <QString>
#include <memory>

class Component;
typedef std::shared_ptr<Component> ComponentPtr;
typedef std::weak_ptr<Component> ComponentWeakPtr;

class ComponentApi : public Api {
public:
    ComponentApi(Instance *instance);

    virtual void load() override;

public:
    static int llist(lua_State *state);
    static int linvoke(lua_State *state);
    static int ldoc(lua_State *state);
    static int lmethods(lua_State *state);
    static int ltype(lua_State *state);
    static int lslot(lua_State *state);
    
private:
    static ComponentPtr getComponent(Instance * instance, const std::string &address);
};


#endif //OPENCEMULATOR_COMPONENTAPI_H
