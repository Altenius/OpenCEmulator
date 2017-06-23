#include <QDebug>
#include <QFile>
#include <functional>
#include "Instance.h"
#include "OpenCEmulator.h"
#include <QUuid>
#include <apis/ComponentApi.h>
#include <apis/ComputerApi.h>
#include <apis/OsApi.h>
#include <apis/SystemApi.h>
#include <apis/UnicodeApi.h>
#include <iostream>
#include <cassert>
#include <components/ComponentComputer.h>



Instance::Instance(size_t maxMemory, const std::string &label, const std::string &uuid) : m_label(label),
                                                                                          m_memoryMax(maxMemory),
                                                                                          m_memoryUsed(0),
                                                                                          m_initialized(false),
                                                                                          m_persistenceApi(this),
                                                                                          m_uuid(uuid),
                                                                                          m_state(nullptr)
{
    if (m_uuid.empty()) {
        m_uuid = QUuid::createUuid().toString().toStdString();
        m_uuid.erase(m_uuid.begin());
        m_uuid.erase(m_uuid.end() - 1);
    }

    m_computerComponent.reset(new ComponentComputer(this));
    attachComponent(m_computerComponent);

    OpenCEmulator::get().addComponent(m_computerComponent);
}



InstancePtr Instance::create(size_t maxMemory, const std::string &label, const std::string &uuid)
{
    InstancePtr instance(new Instance(maxMemory, label, uuid));
    instance->computerComponent()->attach(instance);
    OpenCEmulator::get().addInstance(instance);

    return instance;
}



bool Instance::initialize()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    ComponentApi componentApi(this);
    ComputerApi computerApi(this);
    OsApi osApi(this);
    SystemApi systemApi(this);
    UnicodeApi unicodeApi(this);

    QFile machineFile(":/lua/machine.lua");
    if (!machineFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open machine.lua:" << machineFile.errorString();
        return false;
    }

    m_state = lua_newstate(allocator, this);
    if (m_state == nullptr) {
        std::cerr << "Out of memory" << std::endl;
        return false;
    }

    componentApi.load();
    computerApi.load();
    osApi.load();
    m_persistenceApi.load();
    systemApi.load();
    unicodeApi.load();

    m_thread = lua_newthread(m_state);

    int err;
    if ((err = luaL_loadstring(m_thread, machineFile.readAll().data())) != LUA_OK) {
        if (err == LUA_ERRSYNTAX) {
            std::cerr << "Could not load machine.lua: " << lua_tostring(m_thread, -1) << std::endl;
            m_state = nullptr;
            return false;
        } else {
            std::cerr << "Cold not load machine.lua" << std::endl;
            m_state = nullptr;
            return false;
        }
    }

    m_startTime = std::chrono::system_clock::now();

    m_sleeping = true;
    m_ticks = 0;

    g_stateMap.insert(std::make_pair(m_state->l_G, this));
    
    m_initialized = false;
    m_synchronized = false;

    return true;
}



void *Instance::allocator(void *ud, void *ptr, size_t osize, size_t nsize)
{
    Instance *instance = (Instance *) ud;

    if (nsize > osize) {
        instance->m_memoryUsed += nsize - osize;
        if (instance->m_memoryUsed > instance->m_memoryMax) {
            free(ptr);
            return NULL;
        }
    } else {
        size_t diff = osize - nsize;
        if (instance->m_memoryUsed < diff) {
            instance->m_memoryUsed = 0;
        } else {
            instance->m_memoryUsed -= diff;
        }
    }

    if (nsize == 0) {
        free(ptr);
        return NULL;
    } else {
        return realloc(ptr, nsize);
    }
}



bool Instance::runSynchronized()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_state == nullptr) {
        return false;
    }
    assert(lua_gettop(m_thread) == 1);
    assert(lua_isfunction(m_thread, 1));

    int res = lua_pcall(m_thread, 0, 1, 0);
    if (res != LUA_OK) {
        // TODO: set some error to return in some stringError() method
        m_errorStream.str("");
        m_errorStream << "Synchronized function failed";
        if (res == LUA_ERRRUN) {
            m_errorStream << ": " << lua_tostring(m_thread, -1);
        } else if (res == LUA_ERRMEM) {
            m_errorStream << ": ran out of memory";
        }
        return false;
    }

    if (!lua_istable(m_thread, 1)) {
        // TODO: set some error to return in some stringError() method
        m_errorStream.str("");
        m_errorStream << "Synchronized function failed: a table was not returned";
        return false;
    }

    return true;
}



bool Instance::runThreaded(bool synchronizedReturn)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_state == nullptr) {
        return false;
    }
    int res;
    if (synchronizedReturn) {
        assert(lua_istable(m_thread, 1));
        res = lua_resume(m_thread, nullptr, 1);
    } else if (!m_initialized) {
        // Initialization run
        res = lua_resume(m_thread, nullptr, 0);
        lua_gc(m_thread, LUA_GCCOLLECT, 0);
        m_memoryUsed = 0;
        m_initialized = true;
        lua_pushinteger(m_thread, 0); // push 0 to fake a sleep
    } else {
        if (!m_signalQueue.empty()) {
            auto &signal = m_signalQueue.front();
            assert(lua_checkstack(m_thread, 1 + signal.second.size()));
            lua_pushstring(m_thread, signal.first.c_str());
            signal.second.dump(m_thread);
            m_signalQueue.pop();

            res = lua_resume(m_thread, nullptr, 1 + signal.second.size());
        } else {
            res = lua_resume(m_thread, nullptr, 0);
        }
    }

    switch (res) {
        case LUA_YIELD: {
            int top = lua_gettop(m_thread);
            if (top == 1) {
                if (lua_isfunction(m_thread, 1)) {
                    // This is where OpenComputers calls the function in the main server thread
                    m_synchronized = true;
                } else if (lua_isnumber(m_thread, 1)) {
                    m_ticks = static_cast<int>(lua_tonumber(m_thread, 1) * 20);
                    m_sleeping = true;
                }
            }
            return true;
        }
        case LUA_OK:
            m_errorStream.str("");
            if (lua_toboolean(m_thread, -2) == 0) {
                m_errorStream << "runtime error: " << lua_tostring(m_thread, -1);
            } else {
                m_errorStream << "computer halted";
            }
            return false;
        case LUA_ERRRUN:
            m_errorStream.str("");
            m_errorStream << "kernel error: " << lua_tostring(m_thread, -1);
            return false;
        case LUA_ERRMEM:
            m_errorStream.str("");
            m_errorStream << "instance ran out of memory";
            return false;
        case LUA_ERRGCMM:
            m_errorStream.str("");
            qWarning() << "garbage collector failed";
            return false;
        default:
            m_errorStream.str("");
            m_errorStream << "unknown error";
            return false;
    }
}



bool Instance::runLoop()
{
    m_sleeping = false;
    do {
        if (!runThreaded()) {
            return false;
        }
    } while (queued() && !m_synchronized);

    return true;
}



bool Instance::run()
{
    if (m_synchronized) {
        m_synchronized = false;

        if (!runSynchronized()) {
            return false;
        }
        if (!runThreaded(true)) {
            return false;
        }
        if (!shouldRun() || m_synchronized) {
            return true;
        }
    }

    return runLoop();
}



void Instance::attachComponent(ComponentPtr component)
{
    for (ComponentWeakPtr i : m_components) {
        if (i.lock() == component) {
            return;
        }
    }

    m_components.push_back(ComponentWeakPtr(component));
}



void Instance::detachComponent(ComponentPtr component)
{
    for (auto it = m_components.begin(); it != m_components.end(); ++it) {
        if (it->lock() == component) {
            m_components.erase(it);
            return;
        }
    }
}



void Instance::detachComponent(Component *component)
{
    for (auto it = m_components.begin(); it != m_components.end(); ++it) {
        if (it->lock().get() == component) {
            m_components.erase(it);
            return;
        }
    }
}



Instance::~Instance()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    OpenCEmulator::get().removeComponent(m_computerComponent);
    for (ComponentWeakPtr i : m_components) {
        if (auto component = i.lock()) {
            component->detach(this);
        }
    }

    if (m_state != nullptr) {
        auto it = g_stateMap.find(m_state->l_G);
        assert(it != g_stateMap.end());
        if (it != g_stateMap.end()) {
            g_stateMap.erase(it);
        }
    }
}



bool Instance::signal(const std::string &name, ArgList args)
{
    m_signalQueue.push(std::make_pair(name, args));
    return true;
}



void Instance::start()
{
    if (!initialize()) {
        std::cerr << "failed to initialize instance" << std::endl;
    }
}



void Instance::stop()
{
    if (m_state != nullptr) {
        std::lock_guard<std::mutex> lock(m_mutex);
        lua_close(m_state);

        auto it = g_stateMap.find(m_state->l_G);
        if (it != g_stateMap.end()) {
            g_stateMap.erase(it);
        }

        m_state = nullptr;
    }
}



std::unordered_map<global_State *, Instance *> Instance::g_stateMap;