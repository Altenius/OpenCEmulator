#ifndef OPENCEMULATOR_COMPONENTFILESYSTEM_H
#define OPENCEMULATOR_COMPONENTFILESYSTEM_H

#include "Component.h"

class ComponentFilesystem : public Component
{
public:
    ComponentFilesystem(const std::string &uuid = std::string(), const std::string &label = std::string());

private:
    std::string m_label;

public:
    virtual bool luaLastModified(const ArgList &args, ArgList &out) =0;

    virtual bool luaWrite(const ArgList &args, ArgList &out) =0;

    virtual bool luaRead(const ArgList &args, ArgList &out) =0;

    virtual bool luaExists(const ArgList &args, ArgList &out) =0;

    virtual bool luaIsReadOnly(const ArgList &args, ArgList &out) =0;

    bool luaSetLabel(const ArgList &args, ArgList &out);

    virtual bool luaRename(const ArgList &args, ArgList &out) =0;

    virtual bool luaRemove(const ArgList &args, ArgList &out) =0;

    virtual bool luaMakeDirectory(const ArgList &args, ArgList &out) =0;

    virtual bool luaSeek(const ArgList &args, ArgList &out) =0;

    virtual bool luaClose(const ArgList &args, ArgList &out) =0;

    virtual bool luaIsDirectory(const ArgList &args, ArgList &out) =0;

    virtual bool luaList(const ArgList &args, ArgList &out) =0;

    virtual bool luaSize(const ArgList &args, ArgList &out) =0;

    virtual bool luaSpaceTotal(const ArgList &args, ArgList &out) =0;

    virtual bool luaOpen(const ArgList &args, ArgList &out) =0;

    bool luaGetLabel(const ArgList &args, ArgList &out);

    virtual bool luaSpaceUsed(const ArgList &args, ArgList &out) =0;
};


#endif //OPENCEMULATOR_COMPONENTFILESYSTEM_H
