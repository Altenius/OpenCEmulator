#include <sstream>
#include <cassert>
#include <iostream>
#include "ArgList.h"



Argument::Argument(const char *str) : m_type(STRING), m_string(str)
{

}



Argument::Argument(double number) : m_type(NUMBER), m_number(number)
{

}



Argument::Argument(bool boolean) : m_type(BOOLEAN), m_boolean(boolean)
{

}



Argument::Argument() : m_type(NIL)
{

}



Argument::Argument(const std::string &string) : m_type(STRING), m_string(string)
{

}



Argument::Argument(const char *str, size_t length) : m_type(STRING), m_string(str, length)
{

}



Argument::Argument(long long number) : m_type(INTEGER), m_integer(number)
{

}



std::string Argument::string() const
{
    if (m_type == NUMBER) {
        return std::to_string(m_number);
    } else if (m_type == INTEGER) {
        return std::to_string(m_integer);
    }

    return m_string;
}



std::string Argument::typeName() const
{
    switch (m_type) {
        case STRING:
            return "string";
        case NUMBER:
        case INTEGER:
            return "number";
        case BOOLEAN:
            return "boolean";
        case ARRAY:
            return "array";
        default:
            return "nil";
    }
}



void Argument::dump(lua_State *state) const
{
    switch (m_type) {
        case Argument::STRING:
            lua_pushlstring(state, m_string.c_str(), m_string.size());
            break;
        case Argument::NUMBER:
            lua_pushnumber(state, m_number);
            break;
        case Argument::INTEGER:
            lua_pushinteger(state, m_integer);
            break;
        case Argument::BOOLEAN:
            lua_pushboolean(state, m_boolean);
            break;
        case Argument::ARRAY: {
            assert(lua_checkstack(state, m_array.size()));
            lua_createtable(state, m_array.size(), 0);

            int index = 1;
            for (auto it = m_array.begin(); it != m_array.end(); ++it) {
                lua_pushinteger(state, index++);
                (*it).dump(state);
                lua_rawset(state, -3);
            }
            break;
        }
        default:
            lua_pushnil(state);
    }
}



Argument::Argument(std::vector<Argument> &&array) : m_type(ARRAY), m_array(std::move(array))
{

}



Argument::Argument(const std::vector<Argument> &array) : m_type(ARRAY), m_array(array)
{

}



ArgList::ArgList(lua_State *state, int stackPos)
{
    int top = lua_gettop(state);
    for (int i = stackPos; i <= top; i++) {
        int type = lua_type(state, i);
        switch (type) {
            case LUA_TNUMBER:
                m_arguments.push_back(Argument(lua_tonumber(state, i)));
                break;
            case LUA_TSTRING: {
                size_t slen;
                const char *cstr = lua_tolstring(state, i, &slen);
                m_arguments.push_back(Argument(std::string(cstr, slen)));
                break;
            }
            case LUA_TBOOLEAN:
                m_arguments.push_back(Argument(static_cast<bool>(lua_toboolean(state, i))));
                break;
            default:
                m_arguments.push_back(Argument());
                break;
        }
    }
}



void ArgList::dump(lua_State *state) const
{
    for (auto it = m_arguments.begin(); it != m_arguments.end(); ++it) {
        (*it).dump(state);
    }
}



/*
bool ArgList::tostring(int idx, std::string &out) const {
    if (idx < 0 || idx > m_arguments.size()) {
        return false;
    }
    if (!isstring(idx)) {
        return false;
    }
    
    out = m_arguments[idx].string();
    return true;
}



bool ArgList::tonumber(int idx, double &out) const {
    if (idx < 0 || idx > m_arguments.size()) {
        return false;
    }
    if (!isnumber(idx)) {
        return false;
    }
    
    out = m_arguments[idx].number();
    return true;
}



bool ArgList::toboolean(int idx, bool &out) const {
    if (idx < 0 || idx > m_arguments.size()) {
        return false;
    }
    if (!isboolean(idx)) {
        return false;
    }

    out = m_arguments[idx].boolean();
    return true;
}


bool ArgList::toint(int idx, int &out) {
    return false;
}
*/



ArgList::ArgList()
{

}



ArgList::ArgList(const Argument &arg)
{
    m_arguments.push_back(arg);
}



void ArgList::typeError(int index, const std::string &have, const std::string &want, ArgList &aout) const
{
    std::stringstream err;
    err << "bad argument #" << (index + 3) << " (" << want << " expected, got " << have << ")";
    aout.add(err.str());
}



bool ArgList::checkString(int idx, std::string &out, ArgList &aout) const
{
    if (m_arguments.size() <= idx || idx < 0) {
        typeError(idx, "no value", "string", aout);
        return false;
    }

    const Argument &arg = m_arguments[idx];
    if (!arg.isstring()) {
        typeError(idx, arg.typeName(), "string", aout);
    }

    out = m_arguments[idx].string();

    return true;
}



bool ArgList::checkNumber(int idx, double &out, ArgList &aout) const
{
    if (m_arguments.size() <= idx || idx < 0) {
        typeError(idx, "no value", "number", aout);
        return false;
    }

    const Argument &arg = m_arguments[idx];
    if (!arg.isnumber()) {
        typeError(idx, arg.typeName(), "number", aout);
    }

    out = arg.number();

    return true;
}



bool ArgList::checkInt(int idx, int &out, ArgList &aout) const
{
    if (m_arguments.size() <= idx || idx < 0) {
        typeError(idx, "no value", "number", aout);
        return false;
    }

    const Argument &arg = m_arguments[idx];
    if (!arg.isnumber()) {
        typeError(idx, arg.typeName(), "number", aout);
    }

    out = static_cast<int>(arg.integer());

    return true;
}



bool ArgList::checkLong(int idx, long long &out, ArgList &aout) const
{
    if (m_arguments.size() <= idx || idx < 0) {
        typeError(idx, "no value", "number", aout);
        return false;
    }

    const Argument &arg = m_arguments[idx];
    if (!arg.isnumber()) {
        typeError(idx, arg.typeName(), "number", aout);
    }

    out = arg.integer();

    return true;
}



bool ArgList::checkBoolean(int idx, bool &out, ArgList &aout) const
{
    if (m_arguments.size() <= idx || idx < 0) {
        typeError(idx, "no value", "boolean", aout);
        return false;
    }

    const Argument &arg = m_arguments[idx];
    if (!arg.isboolean()) {
        typeError(idx, arg.typeName(), "boolean", aout);
    }

    out = arg.boolean();

    return true;
}

