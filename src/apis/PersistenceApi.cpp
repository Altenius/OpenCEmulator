#include <QDebug>
#include <assert.h>
#include "PersistenceApi.h"
#include "Instance.h"

extern "C" {
#include "eris.h"
}



PersistenceApi::PersistenceApi(Instance *instance) : Api(instance)
{

}



void PersistenceApi::load()
{
    lua_State *state = m_instance->state();

    lua_pushcfunction(state, PersistenceApi::lpersistKey);
    lua_setglobal(state, "persistKey");

    lua_newtable(state); // perms
    lua_newtable(state); // uperms

    int id_uperms = lua_gettop(state);
    int id_perms = lua_gettop(state) - 1;

    lua_pushstring(state, "_G");
    lua_getglobal(state, "_G");

    std::function<void()> storeValue = [&]() {
        // k v
        if (lua_istable(state, -1) || lua_isfunction(state, -1)) {
            lua_pushvalue(state, -2); // k v k
            lua_gettable(state, id_uperms); // k v uperms[k]
            if (!lua_isnil(state, -1)) {
                qWarning() << "Duplicate permanent value";
            }

            lua_pop(state, 1); // k v

            lua_pushvalue(state, -1); // k v v
            lua_gettable(state, id_perms); // k v perms[v]
            bool isNew = lua_isnil(state, -1);
            lua_pop(state, 1);
            if (isNew) {
                lua_pushvalue(state, -1); // k v v
                lua_pushvalue(state, -3); // k v v k
                lua_rawset(state, id_perms); // k v ; perms[v] = k
                lua_pushvalue(state, -2); // k v k
                lua_pushvalue(state, -2); // k v k v
                lua_rawset(state, id_uperms); // k v ; uperms[k] = v

                if (lua_istable(state, -1)) {
                    std::string key = lua_tostring(state, -2);
                    std::vector<std::string> childKeys;
                    lua_pushnil(state); // k v nil
                    while (lua_next(state, -2) != 0) {
                        // k v ck cv
                        lua_pop(state, 1); // k v ck

                        bool fix = false;
                        if (!lua_isstring(state, -1) || lua_isnumber(state, -1)) {
                            // lua_tostring converts the key to a string which messes with lua_next
                            // so we copy the key
                            lua_pushvalue(state, -1);
                            fix = true;
                        }

                        childKeys.push_back(lua_tostring(state, -1));
                        if (fix) {
                            lua_pop(state, 1);
                        }
                    }

                    std::sort(childKeys.begin(), childKeys.end(), [](std::string &a, std::string &b) { return a.compare(b) < 0; });

                    for (std::string &child : childKeys) {
                        lua_pushstring(state, (key + "." + child).data()); // k v ck
                        lua_getfield(state, -2, child.data());
                        storeValue();
                    }
                }
            }
        }

        lua_pop(state, 2);
    };

    storeValue();

    lua_setfield(state, LUA_REGISTRYINDEX, "uperms");
    lua_setfield(state, LUA_REGISTRYINDEX, "perms");
}



int PersistenceApi::lpersistKey(lua_State *state)
{
    lua_pushstring(state, "__persistthisistotallyauuid");
    return 1;
}



void PersistenceApi::persist(std::vector<char> &vector)
{
    lua_State *state = m_instance->state();
    lua_gc(state, LUA_GCSTOP, 0);

    lua_getfield(state, LUA_REGISTRYINDEX, "perms");
    assert(lua_type(state, 1) == LUA_TTHREAD);
    lua_pushvalue(state, 1); // index 1 is always the kernel thread

    eris_persist(state, -2, -1);
    size_t len;
    const char *data = lua_tolstring(state, -1, &len);
    lua_pop(state, 3);
    vector.resize(len);
    std::copy(data, data + len, vector.begin());
    
    lua_gc(state, LUA_GCRESTART, 0);
}



void PersistenceApi::unpersist(const std::vector<char> &vector)
{
    configure();
    lua_State *state = m_instance->state();
    lua_gc(state, LUA_GCSTOP, 0);


    lua_getfield(state, LUA_REGISTRYINDEX, "uperms");
    lua_pushlstring(state, vector.data(), vector.size());
    eris_unpersist(state, -2, -1);
    lua_insert(state, -3);
    lua_pop(state, 2);

    lua_gc(state, LUA_GCRESTART, 0);
}



void PersistenceApi::configure()
{
    lua_State *state = m_instance->state();

    lua_getglobal(state, "eris");

    lua_getfield(state, -1, "settings");
    lua_pushstring(state, "spkey");
    lua_pushstring(state, "__persistthisistotallyauuid");
    lua_call(state, 2, 0);

    lua_getfield(state, -1, "settings");
    lua_pushstring(state, "path");
    lua_pushboolean(state, false);
    lua_call(state, 2, 0);

    lua_pop(state, 1);
}
