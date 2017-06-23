#include <QPainter>
#include <QBitmap>
#include <FontLoader.h>
#include <iostream>
#include <assert.h>
#include "ComponentScreen.h"
#include "ui/ScreensWidget.h"

using namespace std::placeholders;



ComponentScreen::ComponentScreen(const std::string &uuid, const std::string &label) : Component("screen", uuid, label),
                                                                                      m_foregroundColor(0xFFFFFF),
                                                                                      m_backgroundColor(0x000000),
                                                                                      m_colorPalette(
                                                                                              new EightBitPalette),
                                                                                      m_width(100), m_height(30),
                                                                                      m_buffer(m_width * PIXEL_WIDTH,
                                                                                               m_height * PIXEL_HEIGHT),
                                                                                      m_viewportX(m_width),
                                                                                      m_viewportY(m_height)
{
    registerMethod("setTouchModeInverted", std::bind(&ComponentScreen::luaSetTouchModeInverted, this, _1, _2),
                   "function(value:boolean):boolean -- Sets whether to invert touch mode (sneak-activate opens GUI, instead of normal activate).",
                   false);
    registerMethod("isOn", std::bind(&ComponentScreen::luaIsOn, this, _1, _2),
                   "function():boolean -- Returns whether the screen is currently on.", true);
    registerMethod("setPrecise", std::bind(&ComponentScreen::luaSetPrecise, this, _1, _2),
                   "function(enabled:boolean):boolean -- Set whether to use high precision mode (sub-pixel mouse event positions).",
                   false);
    registerMethod("turnOn", std::bind(&ComponentScreen::luaTurnOn, this, _1, _2),
                   "function():boolean -- Turns the screen on. Returns true if it was off.", false);
    registerMethod("getKeyboards", std::bind(&ComponentScreen::luaGetKeyboards, this, _1, _2),
                   "function():table -- The list of keyboards attached to the screen.", false);
    registerMethod("turnOff", std::bind(&ComponentScreen::luaTurnOff, this, _1, _2),
                   "function():boolean -- Turns off the screen. Returns true if it was on.", false);
    registerMethod("isPrecise", std::bind(&ComponentScreen::luaIsPrecise, this, _1, _2),
                   "function():boolean -- Returns whether the screen is in high precision mode (sub-pixel mouse event positions).",
                   true);
    registerMethod("getAspectRatio", std::bind(&ComponentScreen::luaGetAspectRatio, this, _1, _2),
                   "function():number, number -- The aspect ratio of the screen. For multi-block screens this is the number of blocks, horizontal and vertical.",
                   true);
    registerMethod("isTouchModeInverted", std::bind(&ComponentScreen::luaIsTouchModeInverted, this, _1, _2),
                   "function():boolean -- Whether touch mode is inverted (sneak-activate opens GUI, instead of normal activate).",
                   true);


    m_packed = m_colorPalette->pack(m_foregroundColor, m_backgroundColor);
    std::vector<Pixel> line(m_height, Pixel{' ', m_packed});
    m_pixels.resize(m_width, line);

    QPainter painter(&m_buffer);
    painter.fillRect(QRect(QPoint(0, 0), m_buffer.size()), m_backgroundColor.toQColor());
}



bool ComponentScreen::luaSetTouchModeInverted(const ArgList &args, ArgList &out)
{
    return true;
}



bool ComponentScreen::luaSetPrecise(const ArgList &args, ArgList &out)
{
    return true;
}



bool ComponentScreen::luaTurnOn(const ArgList &args, ArgList &out)
{
    return true;
}



bool ComponentScreen::luaGetAspectRatio(const ArgList &args, ArgList &out)
{
    out.add(static_cast<long long>(m_width));
    out.add(static_cast<long long>(m_height));
    return true;
}



bool ComponentScreen::luaTurnOff(const ArgList &args, ArgList &out)
{
    return true;
}



bool ComponentScreen::luaIsOn(const ArgList &args, ArgList &out)
{
    return true;
}



bool ComponentScreen::luaIsPrecise(const ArgList &args, ArgList &out)
{
    return true;
}



bool ComponentScreen::luaIsTouchModeInverted(const ArgList &args, ArgList &out)
{
    return true;
}



bool ComponentScreen::luaGetKeyboards(const ArgList &args, ArgList &out)
{
    std::vector<Argument> keyboards;
    for (ComponentWeakPtr &component: m_keyboards) {
        if (auto keyboard = std::static_pointer_cast<ComponentKeyboard>(component.lock())) {
            keyboards.push_back(Argument(keyboard->uuid()));
        }
    }
    out.add(Argument(keyboards));
    return true;
}



void ComponentScreen::setPaletteColor(unsigned char idx, unsigned int color)
{
    m_colorPalette->set(idx, color);
}



void ComponentScreen::set(int x, int y, const QString &value, bool vertical)
{
    if (!vertical && (y < 0 || y >= m_height)) {
        return;
    }
    if (vertical && (x < 0 || x >= m_width)) {
        return;
    }
    if (x < m_width && y < m_height) {
        QPainter painter(&m_buffer);

        int sum = 0;
        for (const QChar &c : value) {
            sum += FontLoader::get().width(static_cast<const int>(c.unicode()));
        }

        if (vertical) {
            painter.fillRect(x * PIXEL_WIDTH, y * PIXEL_HEIGHT, PIXEL_WIDTH, value.length() * PIXEL_HEIGHT,
                             m_backgroundColor.toQColor());
        } else {
            painter.fillRect(x * PIXEL_WIDTH, y * PIXEL_HEIGHT, sum * PIXEL_WIDTH, PIXEL_HEIGHT,
                             m_backgroundColor.toQColor());
        }
        painter.setPen(m_foregroundColor.toQColor());

        for (const QChar &c : value) {
            char width = FontLoader::get().width(c.unicode());
            if (width <= 0) {
                continue;
            }

            painter.drawPixmap(QPoint(x * PIXEL_WIDTH, y * PIXEL_HEIGHT), FontLoader::get().getGlyph(c.unicode()));

            if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
                assert(x >= 0);
                assert(y >= 0);
                assert(x < m_width);
                assert(y < m_height);
                m_pixels[x][y] = Pixel{c.unicode(), m_packed};

                for (int i = x + 1; i < x + width; ++i) {
                    if (i < m_width) {
                        assert(i >= 0);
                        assert(y >= 0);
                        assert(i < m_width);
                        assert(y < m_height);
                        m_pixels[i][y] = Pixel{' ', m_packed};
                    }
                }
            }

            if (vertical) {
                y++;
                if (y >= m_height) {
                    break;
                }
            } else {
                x += width;
                if (x >= m_width) {
                    break;
                }
            }
        }
    }

    if (m_widget) {
        m_widget->update();
    }
}



void ComponentScreen::fill(int x, int y, int w, int h, QChar character)
{
    ushort unicode = character.unicode();
    char gWidth = FontLoader::get().width(unicode);
    if (gWidth <= 0) {
        return;
    }

    QPixmap glyph = FontLoader::get().getGlyph(unicode);

    if (x > m_width || y > m_height) {
        return;
    }

    if (x < 0) {
        w += x;
        x = 0;
    }
    if (y < 0) {
        h += y;
        y = 0;
    }

    w = (std::min<int>(x + w * gWidth, m_width) - x) / gWidth;
    h = (std::min<int>(y + h, m_height) - y);

    if (w <= 0 || h <= 0) {
        return;
    }

    QPainter painter(&m_buffer);
    painter.fillRect(x * PIXEL_WIDTH, y * PIXEL_HEIGHT, w * gWidth * PIXEL_WIDTH, h * PIXEL_HEIGHT,
                     m_backgroundColor.toQColor());

    painter.setPen(m_foregroundColor.toQColor());
    for (int ix = 0; ix < w; ix += gWidth) {
        for (int iy = 0; iy < h; ++iy) {
            painter.drawPixmap((x + ix) * PIXEL_WIDTH, (y + iy) * PIXEL_HEIGHT, glyph);

            assert((x + ix) >= 0);
            assert((y + iy) >= 0);
            assert((x + ix) < m_width);
            assert((y + iy) < m_height);
            m_pixels[x + ix][y + iy] = Pixel{unicode, m_packed};

            for (int tx = ix + 1; tx < ix + gWidth; tx++) {
                if (x + tx < m_width) {
                    assert((x + tx) >= 0);
                    assert((y + iy) >= 0);
                    assert((x + tx) < m_width);
                    assert((y + iy) < m_height);
                    m_pixels[x + tx][y + iy] = Pixel{' ', m_packed};
                }
            }
        }
    }

    if (m_widget) {
        m_widget->update();
    }
}



void ComponentScreen::copy(int x, int y, unsigned int w, unsigned int h, int tx,
                           int ty)
{
    if (w == 0 || h == 0) {
        return;
    }
    QPainter painter(&m_buffer);
    QPixmap temp(w * PIXEL_WIDTH, h * PIXEL_HEIGHT);
    {
        QPainter tPainter(&temp);
        tPainter.drawPixmap(0, 0, w * PIXEL_WIDTH, h * PIXEL_HEIGHT, m_buffer, x * PIXEL_WIDTH, y * PIXEL_HEIGHT,
                            w * PIXEL_WIDTH, h * PIXEL_HEIGHT);
    }
    painter.drawPixmap((x + tx) * PIXEL_WIDTH, (y + ty) * PIXEL_HEIGHT, temp, 0, 0, w * PIXEL_WIDTH, h * PIXEL_HEIGHT);

    if (x < 0) {
        tx += x;
        w += x;
    }
    if (y < 0) {
        ty += y;
        h += y;
    }


    int8_t dx, dy;
    int sx, sy, ex, ey;
    if (tx < 0) {
        dx = -1;
        sx = std::min<int>(m_width - 1, x + w);
        if (sx + ty < 0) {
            return;
        }
        ex = std::max<int>(0, std::min(m_width - 1, std::max<int>(0, x + tx) - tx)) - 1;
    } else {
        dx = 1;
        sx = std::max<int>(0, x);
        ex = std::max<int>(0, std::min(m_width - 1, std::min<int>(x + w + tx, m_width - 1) - tx)) + 1;
    }

    if (ty < 0) {
        dy = -1;
        sy = std::min<int>(m_height - 1, y + h);
        if (sy + ty < 0) {
            return;
        }
        ey = std::max<int>(0, std::min(m_height - 1, std::max<int>(0, y + ty) - ty)) - 1;
    } else {
        dy = 1;
        sy = std::max<int>(0, y);
        ey = std::max<int>(0, std::min(m_height - 1, std::min<int>(y + h + ty, m_height - 1) - ty)) + 1;
    }

    for (int ix = sx; ix != ex; ix += dx) {
        if (ix >= m_width) {
            break;
        }

        for (int iy = sy; iy != ey; iy += dy) {
            assert((ix + tx) >= 0);
            assert((iy + ty) >= 0);
            assert((ix + tx) < m_width);
            assert((iy + ty) < m_height);
            assert(ix >= 0);
            assert(iy >= 0);
            assert(ix < m_width);
            assert(iy < m_height);
            m_pixels[ix + tx][iy + ty] = m_pixels[ix][iy];
        }
    }

    if (m_widget) {
        m_widget->update();
    }
}



void ComponentScreen::setResolution(int x, int y)
{
    m_width = m_viewportX = x;
    m_height = m_viewportY = y;


    QPixmap newMap(x * PIXEL_WIDTH, y * PIXEL_HEIGHT);
    QPainter painter(&newMap);

    painter.drawPixmap(0, 0, m_buffer);
    m_buffer = newMap;

    for (auto &line : m_pixels) {
        line.resize(m_height, Pixel{' ', m_packed});
    }
    std::vector<Pixel> line(m_height, Pixel{' ', m_packed});
    m_pixels.resize(m_width, line);
}



bool ComponentScreen::setViewport(int w, int h)
{
    if (w < 1 || h < 1 || w > m_width || h > m_height) {
        return false; // "unsupported viewport resolution"
    }

    m_viewportX = w;
    m_viewportY = h;

    return true;
}



void ComponentScreen::cleanup()
{
    for (ComponentWeakPtr c : m_keyboards) {
        if (ComponentPtr component = c.lock()) {
            auto keyboard = std::static_pointer_cast<ComponentKeyboard>(component);
            keyboard->setScreen(nullptr);
        }
    }
    Component::cleanup();
}



ComponentScreen::~ComponentScreen()
{

}



void ComponentScreen::attachKeyboard(const ComponentPtr &keyboard)
{
    m_keyboards.push_back(ComponentWeakPtr(keyboard));
    if (m_widget) {
        m_widget->setKeyboard(keyboard);
    }
}



void ComponentScreen::detachKeyboard(const ComponentPtr &keyboard)
{
    std::remove_if(m_keyboards.begin(), m_keyboards.end(),
                   [keyboard](ComponentWeakPtr &c) { return c.lock() == keyboard; });
    if (m_widget) {
        if (!m_keyboards.empty()) {
            m_widget->setKeyboard(m_keyboards[0].lock());
        } else {
            m_widget->setKeyboard(nullptr);
        }
    }
}
