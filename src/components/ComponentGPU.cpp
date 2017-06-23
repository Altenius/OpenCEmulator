#include <iostream>
#include "OpenCEmulator.h"
#include "ComponentGPU.h"
#include "ComponentScreen.h"

using namespace std::placeholders;



ComponentGPU::ComponentGPU(const std::string &uuid, const std::string &label) : Component("gpu", uuid, label)
{
    registerMethod("getDepth", std::bind(&ComponentGPU::luaGetDepth, this, _1, _2),
                   "function():number -- Returns the currently set color depth.", true);
    registerMethod("get", std::bind(&ComponentGPU::luaGet, this, _1, _2),
                   "function(x:number, y:number):string, number, number, number or nil, number or nil -- Get the value displayed on the screen at the specified index, as well as the foreground and background color. If the foreground or background is from the palette, returns the palette indices as fourth and fifth results, else nil, respectively.",
                   true);
    registerMethod("fill", std::bind(&ComponentGPU::luaFill, this, _1, _2),
                   "function(x:number, y:number, width:number, height:number, char:string):boolean -- Fills a portion of the screen at the specified position with the specified size with the specified character.",
                   true);
    registerMethod("getScreen", std::bind(&ComponentGPU::luaGetScreen, this, _1, _2),
                   "function():string -- Get the address of the screen the GPU is currently bound to.", true);
    registerMethod("setDepth", std::bind(&ComponentGPU::luaSetDepth, this, _1, _2),
                   "function(depth:number):number -- Set the color depth. Returns the previous value.", false);
    registerMethod("maxResolution", std::bind(&ComponentGPU::luaMaxResolution, this, _1, _2),
                   "function():number, number -- Get the maximum screen resolution.", true);
    registerMethod("getForeground", std::bind(&ComponentGPU::luaGetForeground, this, _1, _2),
                   "function():number, boolean -- Get the current foreground color and whether it's from the palette or not.",
                   true);
    registerMethod("setViewport", std::bind(&ComponentGPU::luaSetViewport, this, _1, _2),
                   "function(width:number, height:number):boolean -- Set the viewport resolution. Cannot exceed the screen resolution. Returns true if the resolution changed.",
                   false);
    registerMethod("setPaletteColor", std::bind(&ComponentGPU::luaSetPaletteColor, this, _1, _2),
                   "function(index:number, color:number):number -- Set the palette color at the specified palette index. Returns the previous value.",
                   true);
    registerMethod("maxDepth", std::bind(&ComponentGPU::luaMaxDepth, this, _1, _2),
                   "function():number -- Get the maximum supported color depth.", true);
    registerMethod("bind", std::bind(&ComponentGPU::luaBind, this, _1, _2),
                   "function(address:string[, reset:boolean=true]):boolean -- Binds the GPU to the screen with the specified address and resets screen settings if `reset` is true.",
                   false);
    registerMethod("getViewport", std::bind(&ComponentGPU::luaGetViewport, this, _1, _2),
                   "function():number, number -- Get the current viewport resolution.", true);
    registerMethod("setResolution", std::bind(&ComponentGPU::luaSetResolution, this, _1, _2),
                   "function(width:number, height:number):boolean -- Set the screen resolution. Returns true if the resolution changed.",
                   false);
    registerMethod("getResolution", std::bind(&ComponentGPU::luaGetResolution, this, _1, _2),
                   "function():number, number -- Get the current screen resolution.", true);
    registerMethod("setForeground", std::bind(&ComponentGPU::luaSetForeground, this, _1, _2),
                   "function(value:number[, palette:boolean]):number, number or nil -- Sets the foreground color to the specified value. Optionally takes an explicit palette index. Returns the old value and if it was from the palette its palette index.",
                   true);
    registerMethod("getBackground", std::bind(&ComponentGPU::luaGetBackground, this, _1, _2),
                   "function():number, boolean -- Get the current background color and whether it's from the palette or not.",
                   true);
    registerMethod("copy", std::bind(&ComponentGPU::luaCopy, this, _1, _2),
                   "function(x:number, y:number, width:number, height:number, tx:number, ty:number):boolean -- Copies a portion of the screen from the specified location with the specified size by the specified translation.",
                   true);
    registerMethod("setBackground", std::bind(&ComponentGPU::luaSetBackground, this, _1, _2),
                   "function(value:number[, palette:boolean]):number, number or nil -- Sets the background color to the specified value. Optionally takes an explicit palette index. Returns the old value and if it was from the palette its palette index.",
                   true);
    registerMethod("getPaletteColor", std::bind(&ComponentGPU::luaGetPaletteColor, this, _1, _2),
                   "function(index:number):number -- Get the palette color at the specified palette index.", true);
    registerMethod("set", std::bind(&ComponentGPU::luaSet, this, _1, _2),
                   "function(x:number, y:number, value:string[, vertical:boolean]):boolean -- Plots a string value to the screen at the specified position. Optionally writes the string vertically.",
                   true);
}



bool ComponentGPU::luaSetResolution(const ArgList &args, ArgList &out)
{
    int w, h;
    if (!checkScreen(out) || !args.checkInt(0, w, out) || !args.checkInt(1, h, out)) {
        return false;
    }

    m_bound->setResolution(w, h);

    return true;
}



bool ComponentGPU::luaBind(const ArgList &args, ArgList &out)
{
    std::string address;
    if (!args.checkString(0, address, out)) {
        return false;
    }
    bool reset = false;
    if (args.size() > 1) {
        if (!args.checkBoolean(1, reset, out)) {
            return false;
        }
    }

    ComponentPtr component = OpenCEmulator::get().findComponent(address);

    if (!component) {
        out.add("invalid address");
        return false;
    }
    if (component->type() != "screen") {
        out.add("not a screen");
        return false;
    }

    if (reset) {
        // TODO: reset stuff
    }

    m_bound = static_cast<ComponentScreen *>(component.get());


    return true;
}



bool ComponentGPU::luaSetBackground(const ArgList &args, ArgList &out)
{
    if (!checkScreen(out)) {
        return false;
    }
    int color;
    if (!args.checkInt(0, color, out)) {
        return false;
    }

    Color oldColor = m_bound->backgroundColor();
    if (oldColor.isPalette()) {
        out.add(Argument(static_cast<long long>(m_bound->getPaletteColor(oldColor))));
        out.add(Argument(static_cast<long long>(oldColor.value())));
    } else {
        out.add(Argument(static_cast<long long>(oldColor.value())));
    }

    m_bound->setBackgroundColor(color);

    return true;
}



bool ComponentGPU::checkScreen(ArgList &out)
{
    if (!m_bound) {
        out.add("no screen");
        return false;
    }
    return true;
}



bool ComponentGPU::luaSetForeground(const ArgList &args, ArgList &out)
{
    if (!checkScreen(out)) {
        return false;
    }
    int color;
    if (!args.checkInt(0, color, out)) {
        return false;
    }

    Color oldColor = m_bound->foregroundColor();
    if (oldColor.isPalette()) {
        out.add(Argument(static_cast<long long>(m_bound->getPaletteColor(oldColor))));
        out.add(Argument(static_cast<long long>(oldColor.value())));
    } else {
        out.add(Argument(static_cast<long long>(oldColor.value())));
    }

    m_bound->setForegroundColor(color);

    return true;
}



bool ComponentGPU::luaGetResolution(const ArgList &args, ArgList &out)
{
    if (!checkScreen(out)) {
        return false;
    }

    int x, y;
    m_bound->getResolution(x, y);

    out.add(Argument(static_cast<long long>(x)));
    out.add(Argument(static_cast<long long>(y)));
    return true;
}



bool ComponentGPU::luaMaxResolution(const ArgList &args, ArgList &out)
{


    out.add(static_cast<long long>(m_maxWidth));
    out.add(static_cast<long long>(m_maxHeight));
    return true;
}



bool ComponentGPU::luaGetScreen(const ArgList &args, ArgList &out)
{
    if (m_bound) {
        out.add(m_bound->uuid());
    }
    return true;
}



bool ComponentGPU::luaSetDepth(const ArgList &args, ArgList &out)
{
    if (!checkScreen(out)) {
        return false;
    }

    int depth;
    if (!args.checkInt(0, depth, out)) {
        return false;
    }

    m_bound->setColorDepth(depth);
    return true;
}



bool ComponentGPU::luaMaxDepth(const ArgList &args, ArgList &out)
{


    out.add(static_cast<long long>(m_maxDepth));
    return true;
}



bool ComponentGPU::luaGetForeground(const ArgList &args, ArgList &out)
{
    if (!checkScreen(out)) {
        return false;
    }

    out.add(static_cast<long long>(m_bound->foregroundColor().value()));
    return true;
}



bool ComponentGPU::luaGetBackground(const ArgList &args, ArgList &out)
{
    if (!checkScreen(out)) {
        return false;
    }

    out.add(static_cast<long long>(m_bound->backgroundColor().value()));
    return true;
}



bool ComponentGPU::luaGetDepth(const ArgList &args, ArgList &out)
{
    if (!checkScreen(out)) {
        return false;
    }

    out.add(static_cast<long long>(m_bound->depth()));
    return true;
}



bool ComponentGPU::luaGetPaletteColor(const ArgList &args, ArgList &out)
{
    if (!checkScreen(out)) {
        return false;
    }

    int idx;
    if (!args.checkInt(0, idx, out)) {
        return false;
    }

    out.add(static_cast<long long>(m_bound->getPaletteColor(idx)));
    return true;
}



bool ComponentGPU::luaSetPaletteColor(const ArgList &args, ArgList &out)
{
    if (!checkScreen(out)) {
        return false;
    }

    int idx, color;
    if (!args.checkInt(0, idx, out) || !args.checkInt(1, color, out)) {
        return false;
    }

    out.add(static_cast<long long>(m_bound->getPaletteColor(idx)));
    m_bound->setPaletteColor(idx, color);
    return true;
}



bool ComponentGPU::luaSet(const ArgList &args, ArgList &out)
{
    int x, y;
    std::string value;
    bool vertical = false;
    if (!checkScreen(out) || !args.checkInt(0, x, out) || !args.checkInt(1, y, out) ||
        !args.checkString(2, value, out)) {
        return false;
    }
    if (args.size() > 3) {
        if (!args.checkBoolean(3, vertical, out)) {
            return false;
        }
    }

    m_bound->set(x - 1, y - 1, QString::fromStdString(value), vertical);

    return true;
}



bool ComponentGPU::luaFill(const ArgList &args, ArgList &out)
{
    int x, y, w, h;
    std::string value;
    if (!checkScreen(out) || !args.checkInt(0, x, out) || !args.checkInt(1, y, out) ||
        !args.checkInt(2, w, out) || !args.checkInt(3, h, out) || !args.checkString(4, value, out)) {
        return false;
    }

    QString uvalue = QString::fromStdString(value);

    if (uvalue.length() == 1) {
        m_bound->fill(x - 1, y - 1, w, h, uvalue.at(0));
        return true;
    }
    out.add("invalid fill value");
    return false;
}



bool ComponentGPU::luaCopy(const ArgList &args, ArgList &out)
{
    int x, y, w, h, tx, ty;
    if (!checkScreen(out) || !args.checkInt(0, x, out) || !args.checkInt(1, y, out) ||
        !args.checkInt(2, w, out) || !args.checkInt(3, h, out) ||
        !args.checkInt(4, tx, out) || !args.checkInt(5, ty, out)) {
        return false;
    }

    m_bound->copy(x - 1, y - 1, w, h, tx, ty);

    return true;
}



bool ComponentGPU::luaGet(const ArgList &args, ArgList &out)
{
    int x, y;
    if (!checkScreen(out) || !args.checkInt(0, x, out) || !args.checkInt(1, y, out)) {
        return false;
    }

    x--;
    y--;

    out.add(QString(m_bound->get(x, y)).toStdString());

    Color fg = m_bound->getForegroundColor(x, y);
    Color bg = m_bound->getBackgroundColor(x, y);
    int fgIndex = -1, bgIndex = -1;

    if (fg.isPalette()) {
        fgIndex = fg.value();
        out.add(static_cast<long long>(m_bound->getPaletteColor(fg)));
    } else {
        out.add(static_cast<long long>(fg.value()));
    }
    if (bg.isPalette()) {
        bgIndex = bg.value();
        out.add(static_cast<long long>(m_bound->getPaletteColor(bg)));
    } else {
        out.add(static_cast<long long>(bg.value()));
    };

    if (fgIndex != -1) {
        out.add(static_cast<long long>(fgIndex));
    } else if (bgIndex != -1) {
        out.add(Argument());
    }
    if (bgIndex != -1) {
        out.add(static_cast<long long>(bgIndex));
    }

    return true;
}



bool ComponentGPU::luaSetViewport(const ArgList &args, ArgList &out)
{
    int w, h;
    if (!checkScreen(out) || !args.checkInt(0, w, out) || !args.checkInt(1, h, out)) {
        return false;
    }

    if (!m_bound->setViewport(w, h)) {
        out.add("unsupported viewport resolution");
        return false;
    }

    return true;
}



bool ComponentGPU::luaGetViewport(const ArgList &args, ArgList &out)
{
    if (!checkScreen(out)) {
        return false;
    }

    out.add(Argument(static_cast<long long>(m_bound->viewportWidth())));
    out.add(Argument(static_cast<long long>(m_bound->viewportHeight())));
    return true;
}
