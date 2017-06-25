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

    /* Sets the resolution of the screen to (x, y). */
    void setResolution(int x, int y);



    /* Sets the color depth of the screen to depth.
     * Valid depths: 1, 4, 8. */
    inline void setColorDepth(int depth)
    {
        m_colorDepth = depth;
    }



    /* Sets the foreground color to color. */
    inline void setForegroundColor(quint32 color)
    {
        m_foregroundColor = color;
        m_packed = m_colorPalette->pack(m_foregroundColor, m_backgroundColor);
    }



    /* Sets the background color to color. */
    inline void setBackgroundColor(quint32 color)
    {
        m_backgroundColor = color;
        m_packed = m_colorPalette->pack(m_foregroundColor, m_backgroundColor);
    }



    /* Returns the color depth. This will be either
     * 1, 4, or 8. */
    inline int depth()
    {
        return m_colorDepth;
    }



    /* Returns the foreground color. */
    inline Color foregroundColor()
    {
        return m_foregroundColor;
    }



    /* Returns the background color. */
    inline Color backgroundColor()
    {
        return m_backgroundColor;
    }



    /* Returns the color of a Color representing
     * a palette index. */
    inline uint32_t getPaletteColor(const Color &color)
    {
        return m_colorPalette->at(color.value());
    }



    /* Sets (x, y) to the current resolution. */
    inline void getResolution(int &x, int &y)
    {
        x = m_width;
        y = m_height;
    }



    /* Returns the foreground color of a character at (x, y). */
    inline Color getForegroundColor(int x, int y)
    {
        if (x < 0 || y < 0 || x > m_width || y >= m_height) {
            return Color(0);
        }
        return m_colorPalette->unpackForeground(m_pixels[x][y].color);
    }



    /* Returns the background color of a character at (x, y). */
    inline Color getBackgroundColor(int x, int y)
    {
        if (x < 0 || y < 0 || x > m_width || y >= m_height) {
            return Color(0);
        }
        return m_colorPalette->unpackBackground(m_pixels[x][y].color);
    }



    /* Returns the pixmap used for drawing to a widget. */
    inline const QPixmap &buffer()
    {
        return m_buffer;
    }



    /* Sets the color of a palette index. */
    void setPaletteColor(unsigned char idx, unsigned int color);

    /* Sets a string starting at (x, y) to value. If vertical is true,
     * value is written from the top to bottom. */
    void set(int x, int y, const QString &value, bool vertical);

    /* Fills an area with a character. */
    void fill(int x, int y, int w, int h, QChar character);

    /* Copies an area to another area. */
    void copy(int x, int y, unsigned int w, unsigned int h, int tx, int ty);



    /* Returns the character at (x, y). */
    ushort get(int x, int y)
    {
        if (x >= 0 && y >= 0 && x < m_width && y < m_height) {
            return m_pixels[x][y].text;
        }
        return 0;
    }



    /* Sets the Qt widget to widget. */
    inline void setWidget(ScreensWidgetPtr widget)
    {
        m_widget = widget;
    }



    /* Returns the widget. Returns a blank shared_ptr if
     * no widget is attached. */
    inline ScreensWidgetPtr &widget()
    {
        return m_widget;
    }



    /* Sets the viewport size to (w, h). Must be smaller than
     * the screen. */
    bool setViewport(int w, int h);



    /* Returns the width of the viewport. */
    inline int viewportWidth()
    {
        return m_viewportX;
    }



    /* Returns the height of the viewport. */
    inline int viewportHeight()
    {
        return m_viewportY;
    }



    /* Attaches a keyboard. */
    void attachKeyboard(const ComponentPtr &keyboard);


    /* Detaches a keyboard. */
    void detachKeyboard(const ComponentPtr &keyboard);



    inline void detachKeyboard(ComponentKeyboard *keyboard)
    {
        std::remove_if(m_keyboards.begin(), m_keyboards.end(),
                       [keyboard](ComponentWeakPtr &c) { return c.lock().get() == keyboard; });
    }



    /* Returns the list of attached keyboard. */
    inline std::vector<ComponentWeakPtr> &keyboards()
    {
        return m_keyboards;
    }



    virtual bool serialize(WriteBuffer &buffer) override;

    virtual bool unserialize(ReadBuffer &buffer) override;

private:
    uint32_t m_width, m_height;
    int m_colorDepth = 8;

    Color m_foregroundColor;
    Color m_backgroundColor;
    uint16_t m_packed;

    uint32_t m_viewportX, m_viewportY;
    
    
    ColorPalettePtr m_colorPalette;

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