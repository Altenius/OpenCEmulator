#include <FontLoader.h>
#include <iostream>
#include "UnicodeApi.h"
#include "Instance.h"



UnicodeApi::UnicodeApi(Instance *instance) : Api(instance)
{

}



static const luaL_Reg unicodelib[] = {
        {"char",      UnicodeApi::lchar},
        {"len",       UnicodeApi::llen},
        {"lower",     UnicodeApi::llower},
        {"reverse",   UnicodeApi::lreverse},
        {"sub",       UnicodeApi::lsub},
        {"upper",     UnicodeApi::lupper},
        {"isWide",    UnicodeApi::lisWide},
        {"charWidth", UnicodeApi::lcharWidth},
        {"wlen",      UnicodeApi::lwlen},
        {"wtrunc",    UnicodeApi::lwtrunc},
        {NULL, NULL}
};



void UnicodeApi::load()
{
    lua_State *state = m_instance->state();

    luaL_newlib(state, unicodelib);
    lua_setglobal(state, "unicode");
}



int UnicodeApi::lchar(lua_State *state)
{
    QString string;
    for (int i = 1; i <= lua_gettop(state); i++) {
        string.append(QChar(luaL_checkint(state, i)));
    }

    pushQString(state, string);
    return 1;
}



int UnicodeApi::llen(lua_State *state)
{
    lua_pushinteger(state, checkQString(state, 1).length());

    return 1;
}



int UnicodeApi::llower(lua_State *state)
{
    QString string = checkQString(state, 1).toLower();

    pushQString(state, string);
    return 1;
}



int UnicodeApi::lreverse(lua_State *state)
{
    QString string = checkQString(state, 1);
    std::reverse(string.begin(), string.end());
    pushQString(state, string);
    return 1;
}



int UnicodeApi::lsub(lua_State *state)
{
    QString string = checkQString(state, 1);
    int start = luaL_checkint(state, 2);
    if (start < 0) {
        start = string.length() + start;
    } else {
        start--;
    }
    start = std::max(0, start);

    int end;
    if (lua_gettop(state) > 2) {
        end = luaL_checkint(state, 3);
        if (end < 0) {
            end = string.length() + end + 1;
        }
        end = std::min(string.length(), end);
    } else {
        end = string.length();
    }

    if (end <= start) {
        lua_pushstring(state, "");
    } else {
        QByteArray data = QStringRef(&string, start, end - start).toUtf8();
        lua_pushlstring(state, data.data(), data.size());
    }

    return 1;
}



int UnicodeApi::lupper(lua_State *state)
{
    QString string = checkQString(state, 1);
    pushQString(state, string.toUpper());
    return 1;
}



int UnicodeApi::lisWide(lua_State *state)
{
    QString string = checkQString(state, 1);

    lua_pushboolean(state, FontLoader::get().width(static_cast<const int>(string.at(0).unicode())) > 1);
    return 1;
}



int UnicodeApi::lcharWidth(lua_State *state)
{
    QString string = checkQString(state, 1);

    lua_pushinteger(state, FontLoader::get().width(static_cast<const int>(string.at(0).unicode())));
    return 1;
}



int UnicodeApi::lwlen(lua_State *state)
{
    QString string = checkQString(state, 1);

    int sum = 0;
    for (auto it = string.begin(); it != string.end(); it++) {
        sum += FontLoader::get().width(static_cast<const int>((*it).unicode()));
    }
    lua_pushinteger(state, sum);
    return 1;
}



int UnicodeApi::lwtrunc(lua_State *state)
{
    QString string = checkQString(state, 1);
    int count = luaL_checkint(state, 2);
    if (count <= 0) {
        lua_pushstring(state, "");
        return 1;
    }

    int width = 0, end = 0;
    while (width < count) {
        width += FontLoader::get().width(string.at(end++).unicode());
    }
    if (end > 1) {
        QByteArray data = QStringRef(&string, 0, end - 1).toUtf8();
        lua_pushlstring(state, data.data(), data.size());
    } else {
        lua_pushstring(state, "");
    }
    return 1;
}


