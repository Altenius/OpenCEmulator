#include "ComponentRealFilesystem.h"
#include <sstream>
#include <QDateTime>
#include <fstream>
#include <iostream>
#include <OpenCEmulator.h>
#include <QDir>



ComponentRealFilesystem::ComponentRealFilesystem(const std::string &uuid, const std::string &label)
        : ComponentFilesystem(uuid, label)
{
    m_path = OpenCEmulator::get().filesystemDirectory() + "/" + uuid;
    QDir(QString::fromStdString(OpenCEmulator::get().filesystemDirectory())).mkpath(QString::fromStdString(uuid));
}



bool ComponentRealFilesystem::luaLastModified(const ArgList &args, ArgList &out)
{
    std::string path;
    if (!args.checkString(0, path, out)) {
        return false;
    }

    out.add(QFileInfo(QString::fromStdString(realPath(path))).lastModified().toMSecsSinceEpoch());
    return true;
}



std::string ComponentRealFilesystem::realPath(std::string path)
{
    path += "/";
    std::vector<std::string> parts;

    size_t last = 0;
    size_t pos;
    while (last <= path.size() && ((pos = path.find_first_of("/\\", last)) != std::string::npos)) {
        if (pos != last) {
            std::string part = path.substr(last, pos - last);
            if (part != ".") {
                if (part == "..") {
                    if (parts.size() != 0) {
                        parts.pop_back();
                    }
                } else {
                    parts.push_back(part);
                }
            }
        }
        last = pos + 1;
    }

    std::ostringstream newPath;
    newPath << m_path;
    for (auto it = parts.begin(); it != parts.end(); it++) {
        newPath << "/" << (*it);
    }

    return newPath.str();
}



bool ComponentRealFilesystem::luaWrite(const ArgList &args, ArgList &out)
{
    long long handle;
    std::string data;
    if (!args.checkLong(0, handle, out) || !args.checkString(1, data, out)) {
        return false;
    }

    auto it = m_handles.find(handle);

    if (it == m_handles.end()) {
        out.add("bad file descriptor");
        return false;
    }

    auto stream = it->second;
    stream->write(data.data(), data.length());

    out.add(true);
    return true;
}



bool ComponentRealFilesystem::luaRead(const ArgList &args, ArgList &out)
{
    long long handle, count;
    if (!args.checkLong(0, handle, out) || !args.checkLong(1, count, out)) {
        return false;
    }

    auto it = m_handles.find(handle);

    if (it == m_handles.end()) {
        out.add("bad file descriptor");
        return false;
    }

    auto stream = it->second;
    if (stream->eof() || !stream->is_open()) {
        return true;
    }

    std::string buffer;
    char buf[1024];
    while (!stream->eof() && count > 0) {
        stream->read(buf, (count > 1024 ? 1024 : count));

        buffer.append(buf, stream->gcount());
        count -= stream->gcount();
    }

    out.add(buffer);
    return true;
}



bool ComponentRealFilesystem::luaExists(const ArgList &args, ArgList &out)
{
    std::string path;
    if (!args.checkString(0, path, out)) {
        return false;
    }

    out.add(QFileInfo(QString::fromStdString(realPath(path))).exists());
    return true;
}



bool ComponentRealFilesystem::luaIsReadOnly(const ArgList &args, ArgList &out)
{
    out.add(false);
    return true;
}



bool ComponentRealFilesystem::luaRename(const ArgList &args, ArgList &out)
{
    std::string from, to;
    if (!args.checkString(0, from, out) || !args.checkString(1, to, out)) {
        return false;
    }

    out.add(QFile::rename(QString::fromStdString(from), QString::fromStdString(to)));
    return true;
}



bool ComponentRealFilesystem::luaRemove(const ArgList &args, ArgList &out)
{
    std::string path;
    if (!args.checkString(0, path, out)) {
        return false;
    }

    out.add(QDir().remove(QString::fromStdString(realPath(path))));
    return true;
}



bool ComponentRealFilesystem::luaMakeDirectory(const ArgList &args, ArgList &out)
{
    std::string path;
    if (!args.checkString(0, path, out)) {
        return false;
    }

    out.add(QDir().mkpath(QString::fromStdString(realPath(path))));
    return true;
}



bool ComponentRealFilesystem::luaSeek(const ArgList &args, ArgList &out)
{
    long long handle, offset;
    std::string whence;

    if (!args.checkLong(0, handle, out) || !args.checkString(1, whence, out) || !args.checkLong(2, offset, out)) {
        return false;
    }

    auto it = m_handles.find(handle);

    if (it == m_handles.end()) {
        out.add("bad file descriptor");
        return false;
    }

    auto stream = it->second;

    std::ios::seekdir way;
    if (whence == "cur")
        way = std::ios::cur;
    else if (whence == "set")
        way = std::ios::beg;
    else if (whence == "end")
        way = std::ios::end;
    else {
        out.add("invalid mode");
        return false;
    }

    stream->seekg(offset, way);
    out.add(static_cast<lua_Number>(stream->tellg())); // TODO: fix this shit
    return true;
}



bool ComponentRealFilesystem::luaClose(const ArgList &args, ArgList &out)
{
    long long handle;
    if (!args.checkLong(0, handle, out)) {
        return false;
    }

    auto it = m_handles.find(handle);

    if (it == m_handles.end()) {
        out.add("bad file descriptor");
        return false;
    }

    it->second->close();
    m_handles.erase(it);
    m_freeHandles.push(handle);

    return true;
}



bool ComponentRealFilesystem::luaIsDirectory(const ArgList &args, ArgList &out)
{
    std::string path;
    if (!args.checkString(0, path, out)) {
        return false;
    }

    out.add(QFileInfo(QString::fromStdString(realPath(path))).isDir());
    return true;
}



bool ComponentRealFilesystem::luaList(const ArgList &args, ArgList &out)
{
    std::string path;
    if (!args.checkString(0, path, out)) {
        return false;
    }
    QDir dir = QDir(QString::fromStdString(realPath(path)));
    if (!dir.exists()) {
        return true;
    }

    std::vector<Argument> names;
    QStringList list = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (QFileInfo(dir, *it).isDir()) {
            names.push_back((*it + "/").toStdString());
        } else {
            names.push_back((*it).toStdString());
        }
    }
    out.add(Argument(std::move(names)));

    return true;
}



bool ComponentRealFilesystem::luaSize(const ArgList &args, ArgList &out)
{
    std::string path;
    if (!args.checkString(0, path, out)) {
        return false;
    }

    out.add(QFileInfo(QString::fromStdString(path)).size());
    return true;
}



bool ComponentRealFilesystem::luaSpaceTotal(const ArgList &args, ArgList &out)
{
    out.add(Argument(static_cast<long long>(1024 * 1024 * 1024))); // Who knows?
    return true;
}



bool ComponentRealFilesystem::luaOpen(const ArgList &args, ArgList &out)
{
    std::string path;
    if (!args.checkString(0, path, out)) {
        return false;
    }

    std::ios_base::openmode mode = std::ios_base::in;
    if (args.size() > 1) {
        std::string sMode;
        if (!args.checkString(1, sMode, out)) {
            return false;
        }
        if (sMode == "a") {
            mode = std::ios_base::app;
        } else if (sMode == "r") {
            mode = std::ios_base::in;
        } else if (sMode == "rb") {
            mode = std::ios_base::in | std::ios_base::binary;
        } else if (sMode == "w") {
            mode = std::ios_base::out;
        } else if (sMode == "wb") {
            mode = std::ios_base::out | std::ios_base::binary;
        } else {
            out.add("unsupported mode");
            return false;
        }
    }

    std::fstream *file = new std::fstream();
    file->open(realPath(path), mode);
    if (mode & std::ios_base::in && !file->is_open()) {
        return false;
    }
    long long handle; // todo: userdata
    if (!m_freeHandles.empty()) {
        handle = m_freeHandles.front();
        m_freeHandles.pop();
    } else {
        handle = m_topHandle++;
    }

    m_handles.insert(std::make_pair(handle, std::shared_ptr<std::fstream>(file)));
    out.add(handle);
    return true;
}



bool ComponentRealFilesystem::luaSpaceUsed(const ArgList &args, ArgList &out)
{
    out.add(Argument(static_cast<long long>(0))); // No space has been used ;)
    return true;
}
