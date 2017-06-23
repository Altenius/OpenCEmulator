#include "ComponentFilesystem.h"

using namespace std::placeholders;



ComponentFilesystem::ComponentFilesystem(const std::string &uuid, const std::string &label) : Component("filesystem",
                                                                                                        uuid, label)
{
    registerMethod("lastModified", std::bind(&ComponentFilesystem::luaLastModified, this, _1, _2),
                   "function(path:string):number -- Returns the ( world) timestamp of when the object at the specified absolute path in the file system was modified.",
                   true);
    registerMethod("write", std::bind(&ComponentFilesystem::luaWrite, this, _1, _2),
                   "function(handle:userdata, value:string):boolean -- Writes the specified data to an open file descriptor with the specified handle.",
                   true);
    registerMethod("read", std::bind(&ComponentFilesystem::luaRead, this, _1, _2),
                   "function(handle:userdata, count:number):string or nil -- Reads up to the specified amount of data from an open file descriptor with the specified handle. Returns nil when EOF is reached.",
                   true);
    registerMethod("exists", std::bind(&ComponentFilesystem::luaExists, this, _1, _2),
                   "function(path:string):boolean -- Returns whether an object exists at the specified absolute path in the file system.",
                   true);
    registerMethod("isReadOnly", std::bind(&ComponentFilesystem::luaIsReadOnly, this, _1, _2),
                   "function():boolean -- Returns whether the file system is read-only.", true);
    registerMethod("setLabel", std::bind(&ComponentFilesystem::luaSetLabel, this, _1, _2),
                   "function(value:string):string -- Sets the label of the drive. Returns the new value, which may be truncated.",
                   false);
    registerMethod("rename", std::bind(&ComponentFilesystem::luaRename, this, _1, _2),
                   "function(from:string, to:string):boolean -- Renames/moves an object from the first specified absolute path in the file system to the second.",
                   false);
    registerMethod("remove", std::bind(&ComponentFilesystem::luaRemove, this, _1, _2),
                   "function(path:string):boolean -- Removes the object at the specified absolute path in the file system.",
                   false);
    registerMethod("makeDirectory", std::bind(&ComponentFilesystem::luaMakeDirectory, this, _1, _2),
                   "function(path:string):boolean -- Creates a directory at the specified absolute path in the file system. Creates parent directories, if necessary.",
                   false);
    registerMethod("seek", std::bind(&ComponentFilesystem::luaSeek, this, _1, _2),
                   "function(handle:userdata, whence:string, offset:number):number -- Seeks in an open file descriptor with the specified handle. Returns the new pointer position.",
                   true);
    registerMethod("close", std::bind(&ComponentFilesystem::luaClose, this, _1, _2),
                   "function(handle:userdata) -- Closes an open file descriptor with the specified handle.", true);
    registerMethod("isDirectory", std::bind(&ComponentFilesystem::luaIsDirectory, this, _1, _2),
                   "function(path:string):boolean -- Returns whether the object at the specified absolute path in the file system is a directory.",
                   true);
    registerMethod("list", std::bind(&ComponentFilesystem::luaList, this, _1, _2),
                   "function(path:string):table -- Returns a list of names of objects in the directory at the specified absolute path in the file system.",
                   false);
    registerMethod("size", std::bind(&ComponentFilesystem::luaSize, this, _1, _2),
                   "function(path:string):number -- Returns the size of the object at the specified absolute path in the file system.",
                   true);
    registerMethod("spaceTotal", std::bind(&ComponentFilesystem::luaSpaceTotal, this, _1, _2),
                   "function():number -- The overall capacity of the file system, in bytes.", true);
    registerMethod("open", std::bind(&ComponentFilesystem::luaOpen, this, _1, _2),
                   "function(path:string[, mode:string='r']):userdata -- Opens a new file descriptor and returns its handle.",
                   true);
    registerMethod("getLabel", std::bind(&ComponentFilesystem::luaGetLabel, this, _1, _2),
                   "function():string -- Get the current label of the drive.", true);
    registerMethod("spaceUsed", std::bind(&ComponentFilesystem::luaSpaceUsed, this, _1, _2),
                   "function():number -- The currently used capacity of the file system, in bytes.", true);
}



bool ComponentFilesystem::luaSetLabel(const ArgList &args, ArgList &out)
{
    return args.checkString(0, m_label, out);
}



bool ComponentFilesystem::luaGetLabel(const ArgList &args, ArgList &out)
{
    out.add(m_label);
    return true;
}
