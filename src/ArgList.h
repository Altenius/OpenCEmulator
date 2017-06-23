#ifndef OPENCEMULATOR_ARGLIST_H
#define OPENCEMULATOR_ARGLIST_H

#include "lua.hpp"
#include <string>
#include <vector>
#include <Qt>
#include <cmath>

class Argument
{
public:
    Argument(const char *str);

    Argument(const char *str, size_t length);

    Argument(const std::string &string);

    Argument(double number);

    Argument(long long number);

    Argument(bool boolean);

    Argument(std::vector<Argument> &&array);

    Argument(const std::vector<Argument> &array);

    Argument();

    enum Type
    {
        STRING,
        NUMBER,
        BOOLEAN,
        INTEGER,
        ARRAY,
        NIL
    };



    inline Type type() const
    {
        return m_type;
    }



    inline bool isstring() const
    {
        return m_type == Argument::STRING || m_type == Argument::NUMBER;
    }



    inline bool isnumber() const
    {
        return m_type == Argument::NUMBER || m_type == Argument::INTEGER;
    }



    inline bool isboolean() const
    {
        return m_type == Argument::BOOLEAN;
    }



    std::string string() const;



    inline double number() const
    {
        if (m_type == INTEGER) {
            return static_cast<double>(m_integer);
        }
        return m_number;
    }



    inline long long integer() const
    {
        if (m_type == NUMBER) {
            if (std::isinf(m_number)) {
                return std::numeric_limits<long long>::max();
            }
            return static_cast<long long>(m_number);
        }
        return m_integer;
    }



    inline bool boolean() const
    {
        return m_boolean;
    }



    void dump(lua_State *state) const;

    std::string typeName() const;

private:
    Type m_type;

    std::string m_string;
    std::vector<Argument> m_array;
    union
    {
        double m_number;
        long long m_integer;
        bool m_boolean;
    };
};


class ArgList
{
public:
    ArgList(lua_State *state, int stackPos);

    ArgList();

    ArgList(const Argument &arg);



    inline Argument::Type type(int idx) const
    {
        if (idx < 0 || idx > m_arguments.size()) {
            return Argument::NIL;
        }
        return m_arguments[idx].type();
    }



    void dump(lua_State *state) const;



    inline bool isstring(int idx) const
    {
        return type(idx) == Argument::STRING || type(idx) == Argument::NUMBER;
    }



    inline bool isnumber(int idx) const
    {
        return type(idx) == Argument::NUMBER || type(idx) == Argument::INTEGER;
    }



    inline bool isboolean(int idx) const
    {
        return type(idx) == Argument::BOOLEAN;
    }



    inline void add(const Argument &arg)
    {
        m_arguments.push_back(arg);
    }



    inline size_t size() const
    {
        return m_arguments.size();
    }
    /* 
     bool tostring(int idx, std::string &out) const;
     bool tonumber(int idx, double &out) const;
     bool toint(int idx, int &out);
     bool tointeger(int idx, long &out);
     bool toboolean(int idx, bool &out) const;*/

    /* These methods will push an error string to aout upon failure.
     * They are assume to be called from a component */
    bool checkString(int idx, std::string &out, ArgList &aout) const;

    bool checkNumber(int idx, double &out, ArgList &aout) const;

    bool checkInt(int idx, int &out, ArgList &aout) const;

    bool checkLong(int idx, long long &out, ArgList &aout) const;

    bool checkBoolean(int idx, bool &out, ArgList &aout) const;

private:
    void typeError(int index, const std::string &have, const std::string &want, ArgList &aout) const;

    std::vector<Argument> m_arguments;
};


#endif //OPENCEMULATOR_ARGLIST_H
