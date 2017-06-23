#ifndef OPENCEMULATOR_COMPONENTSCREEN_H
#define OPENCEMULATOR_COMPONENTSCREEN_H

#include "Component.h"
#include "ColorPalette.h"
#include "ComponentKeyboard.h"
#include <QPixmap>

struct Pixel
{
    ushort text;
    uint16_t color;
};

#define PIXEL_HEIGHT 16
#define PIXEL_WIDTH 8

class ScreensWidget;

typedef std::weak_ptr<ScreensWidget> ScreensWidgetWeakPtr;
typedef std::shared_ptr<ScreensWidget> ScreensWidgetPtr;

class ComponentScreen : public Component
{
public:
    ComponentScreen(const std::string &uuid = std::string(), const std::string &label = std::string());

    ~ComponentScreen();

    void cleanup() override;

    void setResolution(int x, int y);



    inline void setColorDepth(int depth)
    {
        m_colorDepth = depth;
    }



    inline void setForegroundColor(quint32 color)
    {
        m_foregroundColor = color;
        m_packed = m_colorPalette->pack(m_foregroundColor, m_backgroundColor);
    }



    inline void setBackgroundColor(quint32 color)
    {
        m_backgroundColor = color;
        m_packed = m_colorPalette->pack(m_foregroundColor, m_backgroundColor);
    }



    inline int depth()
    {
        return m_colorDepth;
    }



    inline Color foregroundColor()
    {
        return m_foregroundColor;
    }



    inline Color backgroundColor()
    {
        return m_backgroundColor;
    }



    inline uint32_t getPaletteColor(const Color &color)
    {
        return m_colorPalette->at(color.value());
    }



    inline void getResolution(int &x, int &y)
    {
        x = m_width;
        y = m_height;
    }



    inline Color getForegroundColor(int x, int y)
    {
        if (x < 0 || y < 0 || x > m_width || y >= m_height) {
            return Color(0);
        }
        return m_colorPalette->unpackForeground(m_pixels[x][y].color);
    }



    inline Color getBackgroundColor(int x, int y)
    {
        if (x < 0 || y < 0 || x > m_width || y >= m_height) {
            return Color(0);
        }
        return m_colorPalette->unpackBackground(m_pixels[x][y].color);
    }



    inline const QPixmap &buffer()
    {
        return m_buffer;
    }



    void setPaletteColor(unsigned char idx, unsigned int color);

    void set(int x, int y, const QString &value, bool vertical);

    void fill(int x, int y, int w, int h, QChar character);

    void copy(int x, int y, unsigned int w, unsigned int h, int tx, int ty);



    ushort get(int x, int y)
    {
        if (x >= 0 && y >= 0 && x < m_width && y < m_height) {
            return m_pixels[x][y].text;
        }
        return 0;
    }



    inline void setWidget(ScreensWidgetPtr widget)
    {
        m_widget = widget;
    }



    inline ScreensWidgetPtr &widget()
    {
        return m_widget;
    }



    bool setViewport(int w, int h);



    inline int viewportWidth()
    {
        return m_viewportX;
    }



    inline int viewportHeight()
    {
        return m_viewportY;
    }



    void attachKeyboard(const ComponentPtr &keyboard);


    void detachKeyboard(const ComponentPtr &keyboard);



    inline void detachKeyboard(ComponentKeyboard *keyboard)
    {
        std::remove_if(m_keyboards.begin(), m_keyboards.end(),
                       [keyboard](ComponentWeakPtr &c) { return c.lock().get() == keyboard; });
    }



    inline std::vector<ComponentWeakPtr> &keyboards()
    {
        return m_keyboards;
    }



private:
    int m_width, m_height;
    int m_colorDepth = 6;

    Color m_foregroundColor;
    Color m_backgroundColor;
    uint16_t m_packed;

    int m_viewportX, m_viewportY;


    std::unique_ptr<ColorPalette> m_colorPalette;

    QPixmap m_buffer;
    std::vector<std::vector<Pixel> > m_pixels;

    ScreensWidgetPtr m_widget;

    std::vector<ComponentWeakPtr> m_keyboards;

public:
    bool luaSetTouchModeInverted(const ArgList &args, ArgList &out);

    bool luaSetPrecise(const ArgList &args, ArgList &out);

    bool luaTurnOn(const ArgList &args, ArgList &out);

    bool luaGetAspectRatio(const ArgList &args, ArgList &out);

    bool luaTurnOff(const ArgList &args, ArgList &out);

    bool luaIsOn(const ArgList &args, ArgList &out);

    bool luaIsPrecise(const ArgList &args, ArgList &out);

    bool luaIsTouchModeInverted(const ArgList &args, ArgList &out);

    bool luaGetKeyboards(const ArgList &args, ArgList &out);
};


#endif //OPENCEMULATOR_COMPONENTSCREEN_H
