#ifndef OPENCEMULATOR_COMPONENTEEPROM_H
#define OPENCEMULATOR_COMPONENTEEPROM_H


#include "Component.h"

class ComponentEEPROM : public Component {
public:
    ComponentEEPROM(const std::string &uuid = std::string(), const std::string &label = std::string());

protected:
    bool luaGet(const ArgList &args, ArgList &out);
    bool luaGetChecksum(const ArgList &args, ArgList &out);
    bool luaMakeReadonly(const ArgList &args, ArgList &out);
    bool luaGetDataSize(const ArgList &args, ArgList &out);
    bool luaSetLabel(const ArgList &args, ArgList &out);
    bool luaGetLabel(const ArgList &args, ArgList &out);
    bool luaGetData(const ArgList &args, ArgList &out);
    bool luaGetSize(const ArgList &args, ArgList &out);
    bool luaSet(const ArgList &args, ArgList &out);
    bool luaSetData(const ArgList &args, ArgList &out);
    
private:
    std::vector<char> m_codeData, m_volatileData;
    std::string m_label;
};


#endif //OPENCEMULATOR_COMPONENTEEPROM_H
