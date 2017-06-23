#ifndef OPENCEMULATOR_UNICODEAPI_H
#define OPENCEMULATOR_UNICODEAPI_H


#include "Api.h"
#include <QString>

class UnicodeApi : public Api {
public:
    UnicodeApi(Instance *instance);

    virtual void load() override;
    
    static inline QString checkQString(lua_State *state, int idx) {
        size_t len;
        const char *s = lua_tolstring(state, 1, &len);
        return QString::fromLocal8Bit(s, len);
    }
    
    static inline void pushQString(lua_State *state, const QString &string) {
        const QByteArray &data = string.toUtf8();
        lua_pushlstring(state, data.data(), data.size());
    }

public:
    static int lchar(lua_State *state);
    static int llen(lua_State *state);
    static int llower(lua_State *state);
    static int lreverse(lua_State *state);
    static int lsub(lua_State *state);
    static int lupper(lua_State *state);
    static int lisWide(lua_State *state);
    static int lcharWidth(lua_State *state);
    static int lwlen(lua_State *state);
    static int lwtrunc(lua_State *state);
};


#endif //OPENCEMULATOR_UNICODEAPI_H
