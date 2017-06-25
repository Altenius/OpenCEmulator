#ifndef OPENCEMULATOR_COMPONENTGPU_H
#define OPENCEMULATOR_COMPONENTGPU_H


#include "Component.h"

class ComponentScreen;

class ComponentGPU : public Component
{
public:
    ComponentGPU(const std::string &uuid = std::string(), const std::string &label = std::string());

    virtual bool serialize(WriteBuffer &buffer) override;
    
    virtual bool unserialize(ReadBuffer &buffer) override;

private:
    unsigned char m_maxDepth = 6;
    unsigned char m_maxWidth = 100, m_maxHeight = 30;
    ComponentWeakPtr m_bound;

    bool checkScreen(ArgList &out, std::shared_ptr<ComponentScreen> &screen);

public:
    bool luaGetDepth(const ArgList &args, ArgList &out);

    bool luaGet(const ArgList &args, ArgList &out);

    bool luaFill(const ArgList &args, ArgList &out);

    bool luaGetScreen(const ArgList &args, ArgList &out);

    bool luaSetDepth(const ArgList &args, ArgList &out);

    bool luaMaxResolution(const ArgList &args, ArgList &out);

    bool luaGetForeground(const ArgList &args, ArgList &out);

    bool luaSetViewport(const ArgList &args, ArgList &out);

    bool luaSetPaletteColor(const ArgList &args, ArgList &out);

    bool luaMaxDepth(const ArgList &args, ArgList &out);

    bool luaBind(const ArgList &args, ArgList &out);

    bool luaGetViewport(const ArgList &args, ArgList &out);

    bool luaSetResolution(const ArgList &args, ArgList &out);

    bool luaGetResolution(const ArgList &args, ArgList &out);

    bool luaSetForeground(const ArgList &args, ArgList &out);

    bool luaGetBackground(const ArgList &args, ArgList &out);

    bool luaCopy(const ArgList &args, ArgList &out);

    bool luaSetBackground(const ArgList &args, ArgList &out);

    bool luaGetPaletteColor(const ArgList &args, ArgList &out);

    bool luaSet(const ArgList &args, ArgList &out);
};


#endif //OPENCEMULATOR_COMPONENTGPU_H
