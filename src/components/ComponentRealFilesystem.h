#ifndef OPENCEMULATOR_COMPONENTREALFILESYSTEM_H
#define OPENCEMULATOR_COMPONENTREALFILESYSTEM_H


#include "ComponentFilesystem.h"
#include <QDir>
#include <queue>

class ComponentRealFilesystem : public ComponentFilesystem {
public:
    ComponentRealFilesystem(const std::string &uuid = std::string(), const std::string &label = std::string());
    
private:
    std::string realPath(std::string path);

    std::string m_path;
    std::unordered_map<long long, std::shared_ptr<std::fstream>> m_handles;
    std::queue<long long> m_freeHandles;
    int m_topHandle = 0;

public:
    bool luaLastModified(const ArgList &args, ArgList &out) override;
    bool luaWrite(const ArgList &args, ArgList &out) override;
    bool luaRead(const ArgList &args, ArgList &out) override;
    bool luaExists(const ArgList &args, ArgList &out) override;
    bool luaIsReadOnly(const ArgList &args, ArgList &out) override;
    bool luaRename(const ArgList &args, ArgList &out) override;
    bool luaRemove(const ArgList &args, ArgList &out) override;
    bool luaMakeDirectory(const ArgList &args, ArgList &out) override;
    bool luaSeek(const ArgList &args, ArgList &out) override;
    bool luaClose(const ArgList &args, ArgList &out) override;
    bool luaIsDirectory(const ArgList &args, ArgList &out) override;
    bool luaList(const ArgList &args, ArgList &out) override;
    bool luaSize(const ArgList &args, ArgList &out) override;
    bool luaSpaceTotal(const ArgList &args, ArgList &out) override;
    bool luaOpen(const ArgList &args, ArgList &out) override;
    bool luaSpaceUsed(const ArgList &args, ArgList &out) override;
};


#endif //OPENCEMULATOR_COMPONENTREALFILESYSTEM_H
