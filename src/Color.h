#ifndef OPENCEMULATOR_COLOR_H
#define OPENCEMULATOR_COLOR_H


#include <cstdint>
#include <QColor>

class Color
{
public:
    Color(uint8_t r, uint8_t g, uint8_t b);

    Color(uint32_t color, bool palette = false);



    inline uint32_t value() const
    {
        return m_color;
    }



    inline bool isPalette() const
    {
        return m_palette;
    }



    inline void extract(uint8_t &r, uint8_t &g, uint8_t &b) const
    {
        r = (m_color >> 16) & 0xFF;
        g = (m_color >> 8) & 0xFF;
        b = (m_color) & 0xFF;
    }



    inline QColor toQColor() const
    {
        return QColor::fromRgb(m_color);
    }



private:
    uint32_t m_color;
    bool m_palette;
};


#endif //OPENCEMULATOR_COLOR_H
