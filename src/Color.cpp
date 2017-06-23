#include "Color.h"



Color::Color(uint8_t r, uint8_t g, uint8_t b) : m_palette(false)
{
    m_color = (r << 16) | (g << 8) | b;
}



Color::Color(uint32_t color, bool palette) : m_color(color), m_palette(palette)
{

}

