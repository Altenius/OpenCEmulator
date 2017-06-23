#include "ComponentComputer.h"
#include "Instance.h"

using namespace std::placeholders;

ComponentComputer::ComponentComputer(Instance *instance) : m_instance(instance), Component("computer", instance->address(), instance->label()) {
    registerMethod("beep", std::bind(&ComponentComputer::luaBeep, this, _1, _2), "function([frequency:number[, duration:number]]) -- Plays a tone, useful to alert users via audible feedback.", false);
    registerMethod("start", std::bind(&ComponentComputer::luaStart, this, _1, _2), "function():boolean -- Starts the computer. Returns true if the state changed.", false);
    registerMethod("stop", std::bind(&ComponentComputer::luaStop, this, _1, _2), "function():boolean -- Stops the computer. Returns true if the state changed.", false);
    registerMethod("getDeviceInfo", std::bind(&ComponentComputer::luaGetDeviceInfo, this, _1, _2), "function():table -- Collect information on all connected devices.", true);
    registerMethod("isRunning", std::bind(&ComponentComputer::luaIsRunning, this, _1, _2), "function():boolean -- Returns whether the computer is running.", true);
    registerMethod("getProgramLocations", std::bind(&ComponentComputer::luaGetProgramLocations, this, _1, _2), "function():table -- Returns a map of program name to disk label for known programs.", false);
}



bool ComponentComputer::luaIsRunning(const ArgList &args, ArgList &out) {
    out.add(true);
    return true;
}



bool ComponentComputer::luaStart(const ArgList &args, ArgList &out) {
    out.add(true);
    return true;
}



bool ComponentComputer::luaStop(const ArgList &args, ArgList &out) {
    out.add(true);
    return true;
}



bool ComponentComputer::luaBeep(const ArgList &args, ArgList &out) {
    return true;
}



bool ComponentComputer::luaGetProgramLocations(const ArgList &args, ArgList &out) {
    out.add(std::vector<Argument>());
    return true;
}



bool ComponentComputer::luaGetDeviceInfo(const ArgList &args, ArgList &out) {
    out.add(std::vector<Argument>());
    return true;
}
