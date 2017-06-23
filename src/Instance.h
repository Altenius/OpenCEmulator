#ifndef OPENCEMULATOR_INSTANCE_H
#define OPENCEMULATOR_INSTANCE_H

#include <unordered_map>
#include "lua.hpp"
#include "lapi.h"
#include "components/Component.h"
#include <chrono>
#include <queue>
#include <apis/Api.h>
#include <apis/PersistenceApi.h>
#include <sstream>
#include "Scheduler.h"

class Instance;
typedef std::shared_ptr<Instance> InstancePtr;
typedef std::weak_ptr<Instance> InstanceWeakPtr;

class Instance {
    friend Scheduler;
public:
    Instance(size_t maxMemory, const std::string &label = std::string(), const std::string &uuid = std::string());
    ~Instance();
    
    bool initialize();
    bool run();
    bool runSynchronized();
    bool runThreaded(bool synchronizedReturn = false);
    
    inline lua_State *thread() {
        return m_thread;
    }
    
    inline lua_State *state() {
        return m_state;
    }
    
    inline std::vector<ComponentWeakPtr> &components() {
        return m_components;
    }
    
    inline const std::string &address() {
        return m_uuid;
    }
    
    inline lua_Number uptime() {
        // This function should count ticks, not actual seconds
        return static_cast<lua_Integer>(cpuTime() * 20);
    }
    
    inline lua_Number cpuTime() {
        return (static_cast<float>((std::chrono::system_clock::now() - m_startTime).count())
                * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den);
    }
    
    void start();
    void stop();
    
    void attachComponent(ComponentPtr component);
    void detachComponent(ComponentPtr component);
    void detachComponent(Component *component);
    
    
    inline size_t memoryUsed() {
        return m_memoryUsed;
    }
    
    inline size_t memoryMax() {
        return m_memoryMax;
    }
    
    inline bool sleeping() {
        return m_sleeping;
    }
    
    inline const std::string &label() {
        if (m_label.empty()) {
            return m_uuid;
        }
        return m_label;
    }
    
    inline void setLabel(const std::string &label) {
        m_label = label;
    }
    
    inline bool synchronized() {
        return m_synchronized;
    }

    inline bool queued() {
        return !m_signalQueue.empty();
    }
    
    std::string lastError() {
        return m_errorStream.str();
    }
    
    bool signal(const std::string &name, ArgList args);
    
    inline bool shouldRun() {
        if (m_state == nullptr) {
            return false;
        }
        if (m_sleeping && m_ticks <= 0) {
            return true;
        }
        return m_synchronized || queued();
    }

protected:
    int m_ticks;
    
private:
    lua_State *m_state, *m_thread;
    size_t m_memoryUsed, m_memoryMax;
    std::vector<ComponentWeakPtr> m_components;
    bool m_initialized;
    std::chrono::system_clock::time_point m_startTime;
    std::string m_uuid, m_label;
    
    std::stringstream m_errorStream;
    
    bool m_sleeping = false;
    bool m_synchronized = false;

    PersistenceApi m_persistenceApi;

    static void *allocator(void *ud, void *ptr, size_t osize, size_t nsize);
    
    static std::unordered_map<global_State*, Instance*> g_stateMap;
    
    std::queue<std::pair<std::string, ArgList> > m_signalQueue;
    
    bool runLoop();
    
public:
    static inline Instance* fromState(lua_State *state) {
        auto it = g_stateMap.find(state->l_G);
        if (it == g_stateMap.end()) {
            return nullptr;
        }
        return it->second;
    }
};


#endif //OPENCEMULATOR_INSTANCE_H
