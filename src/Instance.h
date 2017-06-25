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
#include <memory>
#include <thread>
#include <mutex>
#include "Scheduler.h"

class Instance;

typedef std::shared_ptr<Instance> InstancePtr;
typedef std::weak_ptr<Instance> InstanceWeakPtr;

class Instance
{
    friend Scheduler;
public:
    static InstancePtr
    create(size_t maxMemory, const std::string &label = std::string(), const std::string &uuid = std::string());

    ~Instance();

    /* Resumes the instance. Called internally from the scheduler */
    bool run();



    /* Returns the Lua thread */
    inline lua_State *thread()
    {
        return m_thread;
    }



    /* Returns the Lua state */
    inline lua_State *state()
    {
        return m_state;
    }



    /* Returns the list of components for iterating. lock() should
     * be called on each entry to get a ComponentPtr and checked. */
    inline std::vector<ComponentWeakPtr> &components()
    {
        return m_components;
    }



    /* Returns the address of the instance */
    inline const std::string &address()
    {
        return m_uuid;
    }



    /* Returns the instance's ComponentComputer component */
    inline const ComponentPtr &computerComponent()
    {
        return m_computerComponent;
    }



    /* Returns the amount of time in milliseconds that the
     * instance has been running */
    inline lua_Number cpuTime()
    {
        return (std::chrono::duration_cast<std::chrono::duration<double>>(
                std::chrono::steady_clock::now() - m_startTime).count());
    }



    /* Starts the instance */
    void start();

    /* Stops the instance */
    void stop();

    /* Attaches a component to the instance.
     * component->attachInstance should also be called */
    void attachComponent(ComponentPtr component);

    /* Detaches a component from the instance.
     * component->detachInstance should also be called */
    void detachComponent(ComponentPtr component);

    void detachComponent(Component *component);



    /* Returns the amount of memory used by the lua state */
    inline size_t memoryUsed()
    {
        return m_memoryUsed;
    }



    /* Returns the amount of memory allowed to be allocated for
     * the lua state */
    inline size_t memoryMax()
    {
        return m_memoryMax;
    }



    /* Returns true if the instance is sleeping. The instance
     * sleeps when waiting a specific amount of m_ticks or for
     * an event */
    inline bool sleeping()
    {
        return m_sleeping;
    }



    /* Returns the label of the instance or the address if it
     * is not labeled */
    inline const std::string &label()
    {
        if (m_label.empty()) {
            return m_uuid;
        }
        return m_label;
    }



    /* Sets the label of the instance */
    inline void setLabel(const std::string &label)
    {
        m_label = label;
        m_computerComponent->setLabel(label);
    }



    /* Returns true if the instance is waiting for a synchronized
     * call. This is where OpenComputers calls a lua function from
     * the main thread. */
    inline bool synchronized()
    {
        return m_synchronized;
    }



    /* Returns true if there are signals ready to be sent to the
     * instance */
    inline bool queued()
    {
        return !m_signalQueue.empty();
    }



    /* Returns the last error that occurred */
    std::string lastError()
    {
        return m_errorStream.str();
    }



    /* Queues a signal to be transmitted to the instance */
    bool signal(const std::string &name, ArgList args);



    /* Returns true if the instance is ready to run. */
    inline bool shouldRun()
    {
        if (m_state == nullptr) {
            return false;
        }
        if (m_sleeping && m_ticks <= 0) {
            return true;
        }
        return m_synchronized || queued();
    }



    /* Save the state of the instance to a file */
    void persist();

    /* Load the state of the instance from a file */
    void unpersist();

protected:
    int m_ticks;

    Instance(size_t maxMemory, const std::string &label, const std::string &uuid);

private:
    std::mutex m_mutex;
    lua_State *m_state, *m_thread;
    size_t m_memoryUsed, m_memoryMax;
    std::vector<ComponentWeakPtr> m_components;
    bool m_initialized;
    std::chrono::steady_clock::time_point m_startTime;
    std::string m_uuid, m_label;
    ComponentPtr m_computerComponent;

    std::stringstream m_errorStream;

    bool m_sleeping = false;
    bool m_synchronized = false;

    PersistenceApi m_persistenceApi;

    static void *allocator(void *ud, void *ptr, size_t osize, size_t nsize);

    static std::unordered_map<global_State *, Instance *> g_stateMap;

    std::queue<std::pair<std::string, ArgList> > m_signalQueue;

    /* Resumes the instance until the signal queue is empty. */
    bool runLoop();

    /* Initializes the instance. Called internally from start(). */
    bool initialize();

    /* Resumes the instance for a synchronized call. Called
     * internally from run(). */
    bool runSynchronized();

    /* Resumes the instance. If synchronizedReturn is true, pushes
     * the result of the synchronized call. */
    bool runThreaded(bool synchronizedReturn = false);


public:
    /* Returns the instance from the state-to-instance map */
    static inline Instance *fromState(lua_State *state)
    {
        auto it = g_stateMap.find(state->l_G);
        if (it == g_stateMap.end()) {
            return nullptr;
        }
        return it->second;
    }
};


#endif //OPENCEMULATOR_INSTANCE_H
