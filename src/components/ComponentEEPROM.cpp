#include <iostream>
#include "ComponentEEPROM.h"
#include <QFile>
#include <QDebug>

using namespace std::placeholders;



ComponentEEPROM::ComponentEEPROM(const std::string &uuid, const std::string &label) : Component("eeprom", uuid, label)
{
    QFile defaultRom(":/lua/bios.lua");
    if (!defaultRom.open(QIODevice::ReadOnly)) {
        std::cerr << "Could not open bios.lua" << std::endl;
    }

    QByteArray data = defaultRom.readAll();
    m_codeData.assign(data.begin(), data.end());

    registerMethod("get", std::bind(&ComponentEEPROM::luaGet, this, _1, _2),
                   "function():string -- Get the currently stored byte array.", true);
    registerMethod("getChecksum", std::bind(&ComponentEEPROM::luaGetChecksum, this, _1, _2),
                   "function():string -- Get the checksum of the data on this EEPROM.", true);
    registerMethod("makeReadonly", std::bind(&ComponentEEPROM::luaMakeReadonly, this, _1, _2),
                   "function(checksum:string):boolean -- Make this EEPROM readonly if it isn't already. This process cannot be reversed!",
                   true);
    registerMethod("getDataSize", std::bind(&ComponentEEPROM::luaGetDataSize, this, _1, _2),
                   "function():string -- Get the storage capacity of this EEPROM.", true);
    registerMethod("setLabel", std::bind(&ComponentEEPROM::luaSetLabel, this, _1, _2),
                   "function(data:string):string -- Set the label of the EEPROM.", false);
    registerMethod("getLabel", std::bind(&ComponentEEPROM::luaGetLabel, this, _1, _2),
                   "function():string -- Get the label of the EEPROM.", true);
    registerMethod("getData", std::bind(&ComponentEEPROM::luaGetData, this, _1, _2),
                   "function():string -- Get the currently stored byte array.", true);
    registerMethod("getSize", std::bind(&ComponentEEPROM::luaGetSize, this, _1, _2),
                   "function():string -- Get the storage capacity of this EEPROM.", true);
    registerMethod("set", std::bind(&ComponentEEPROM::luaSet, this, _1, _2),
                   "function(data:string) -- Overwrite the currently stored byte array.", false);
    registerMethod("setData", std::bind(&ComponentEEPROM::luaSetData, this, _1, _2),
                   "function(data:string) -- Overwrite the currently stored byte array.", false);
}



bool ComponentEEPROM::luaGet(const ArgList &args, ArgList &out)
{
    if (!m_codeData.empty()) {
        out.add(Argument(m_codeData.data(), m_codeData.size()));
    }
    return true;
}



bool ComponentEEPROM::luaGetChecksum(const ArgList &args, ArgList &out)
{
    out.add("checksum"); // Fuck this, I'll implement it later
    return true;
}



bool ComponentEEPROM::luaMakeReadonly(const ArgList &args, ArgList &out)
{
    // unimplemented
    return true;
}



bool ComponentEEPROM::luaGetDataSize(const ArgList &args, ArgList &out)
{
    out.add((lua_Number) 65565); // I'm just guessing here
    return true;
}



bool ComponentEEPROM::luaSetLabel(const ArgList &args, ArgList &out)
{
    if (args.size() != 0) {
        if (!args.checkString(0, m_label, out)) {
            return false;
        }
        if (m_label.empty()) {
            m_label = "EEPROM";
        }
    } else {
        m_label = "EEPROM";
    }

    out.add(m_label);

    return true;
}



bool ComponentEEPROM::luaGetLabel(const ArgList &args, ArgList &out)
{
    out.add(m_label);
    return true;
}



bool ComponentEEPROM::luaGetData(const ArgList &args, ArgList &out)
{
    if (!m_volatileData.empty()) {
        out.add(Argument(m_volatileData.data(), m_volatileData.size()));
    }
    return true;
}



bool ComponentEEPROM::luaGetSize(const ArgList &args, ArgList &out)
{
    out.add((lua_Number) 65565); // I'm just guessing here
    return true;
}



bool ComponentEEPROM::luaSet(const ArgList &args, ArgList &out)
{
    if (args.size() != 0) {
        std::string data;
        if (!args.checkString(0, data, out)) {
            return false;
        }
        m_codeData.assign(data.begin(), data.end());
    } else {
        m_codeData.clear();
    }

    return true;
}



bool ComponentEEPROM::luaSetData(const ArgList &args, ArgList &out)
{
    if (args.size() != 0) {
        std::string data;
        if (!args.checkString(0, data, out)) {
            return false;
        }
        m_volatileData.assign(data.begin(), data.end());
    } else {
        m_volatileData.clear();
    }

    return true;
}
