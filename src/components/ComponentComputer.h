#ifndef OPENCEMULATOR_COMPONENTCOMPUTER_H
#define OPENCEMULATOR_COMPONENTCOMPUTER_H

#include "Component.h"

class ComponentComputer : public Component {
public:
    ComponentComputer(Instance *instance);
    
private:
    Instance *m_instance;
    
public:
    bool luaBeep(const ArgList &args, ArgList &out);
    bool luaStart(const ArgList &args, ArgList &out);
    bool luaStop(const ArgList &args, ArgList &out);
    bool luaGetDeviceInfo(const ArgList &args, ArgList &out);
    bool luaIsRunning(const ArgList &args, ArgList &out);
    bool luaGetProgramLocations(const ArgList &args, ArgList &out);

};


#endif //OPENCEMULATOR_COMPONENTCOMPUTER_H
