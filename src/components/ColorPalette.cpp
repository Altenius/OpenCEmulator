#include "ColorPalette.h"



ColorPalette::ColorPalette()
{

}



uint16_t ColorPalette::pack(const Color &foreground, const Color &background)
{
    return static_cast<uint16_t>(((deflate(foreground) << 8) | deflate(background)));
}



uint32_t ColorPalette::unpackForeground(uint16_t color)
{
    return inflate(color >> 8);
}



uint32_t ColorPalette::unpackBackground(uint16_t color)
{
    return inflate(color & 0xFF);
}



FourBitPalette::FourBitPalette()
{

}



float FourBitPalette::delta(int colorA, int colorB)
{ // https://github.com/MightyPirates/OpenComputers/blob/master-MC1.7.10/src/main/scala/li/cil/oc/util/PackedColor.scala#L84
    unsigned char rA = (colorA & 0xFF0000) >> 16, gA = (colorA & 0xFF00) >> 8, bA = colorA & 0xFF;
    unsigned char rB = (colorA & 0xFF0000) >> 16, gB = (colorA & 0xFF00) >> 8, bB = colorA & 0xFF;

    unsigned char rD = (rA - rB);
    unsigned char gD = (gA - gB);
    unsigned char bD = (bA - bB);
    return 0.2126 * rD * rD * 0.7152 * gD * gD * 0.0722 * bD *
           bD; // This calculates the distance between colorA and colorB.
}



uint32_t FourBitPalette::inflate(uint32_t index)
{
    return colors[std::min<uint32_t>(sizeof(colors) - 1, index)];
}



uint8_t FourBitPalette::deflate(const Color &color)
{
    if (color.isPalette()) {
        return color.value() % sizeof(colors);
    }
    float closest = delta(color.value(), colors[0]);
    unsigned char index = 0;

    for (unsigned char i = 1; i < sizeof(colors); i++) {
        float d = delta(color.value(), colors[i]);
        if (d < closest) {
            closest = d;
            index = i;
        }
    }

    return index;
}



unsigned int FourBitPalette::colors[] = {0xFFFFFF, 0xFFCC33, 0xCC66CC, 0x6699FF,
                                         0xFFFF33, 0x33CC33, 0xFF6699, 0x333333,
                                         0xCCCCCC, 0x336699, 0x9933CC, 0x333399,
                                         0x663300, 0x336600, 0xFF3333, 0x000000};



OneBitPalette::OneBitPalette()
{

}



uint32_t OneBitPalette::inflate(uint32_t index)
{
    return index == 0 ? 0 : ONEBIT_COLOR;
}



uint8_t OneBitPalette::deflate(const Color &color)
{
    return (color.value() == 0) ? 0 : 1;
}



unsigned int EightBitPalette::colors[] = {};



EightBitPalette::EightBitPalette()
{
    // initialize palette to grayscale

    for (int i = 0; i < sizeof(colors); ++i) {
        uint32_t shade = 0xFF * (i + 1) / (sizeof(colors) + 1);
        colors[i] = (shade << 16) | (shade << 8) | (shade);
    }
}



#define REDS 6
#define GREENS 8
#define BLUES 5



uint32_t EightBitPalette::inflate(uint32_t value)
{
    if (isFromPalette(value)) {
        return FourBitPalette::inflate(value);
    }

    uint32_t index = value - sizeof(colors);
    uint8_t idxB = index % BLUES;
    uint8_t idxG = (index / BLUES) % GREENS;
    uint8_t idxR = (index / BLUES / GREENS) % REDS;
    uint32_t r = static_cast<uint32_t>((static_cast<float>(idxR) * 0xFF / (REDS - 1) + 0.5f));
    uint32_t g = static_cast<uint32_t>((static_cast<float>(idxG) * 0xFF / (GREENS - 1) + 0.5f));
    uint32_t b = static_cast<uint32_t>((static_cast<float>(idxB) * 0xFF / (BLUES - 1) + 0.5f));

    return (r << 16) | (g << 8) | (b);
}



bool EightBitPalette::isFromPalette(uint32_t color)
{
    return color < sizeof(colors);
}



uint8_t EightBitPalette::deflate(const Color &color)
{
    uint8_t index = FourBitPalette::deflate(color);
    if (color.isPalette()) {
        return index;
    }

    uint8_t r, g, b;
    color.extract(r, g, b);

    uint8_t idxR = static_cast<uint8_t>((static_cast<float>(r) * (REDS - 1) / 0xFF + 0.5));
    uint8_t idxG = static_cast<uint8_t>((static_cast<float>(g) * (GREENS - 1) / 0xFF + 0.5));
    uint8_t idxB = static_cast<uint8_t>((static_cast<float>(b) * (BLUES - 1) / 0xFF + 0.5));
    uint8_t deflated = sizeof(colors) + idxR * GREENS * BLUES + idxG * BLUES + idxB;
    if (delta(inflate(deflated & 0xFF), color.value()) < delta(inflate(index & 0xFF), color.value())) {
        return deflated;
    }
    return index;
}



unsigned int OneBitPalette::at(unsigned char index)
{
    return index == 0 ? 0 : ONEBIT_COLOR;
}



void OneBitPalette::set(unsigned char index, unsigned int color)
{

}



unsigned int FourBitPalette::at(unsigned char index)
{
    return colors[std::min<uint8_t>(sizeof(colors) - 1, index)];
}



void FourBitPalette::set(unsigned char index, unsigned int color)
{
    colors[std::min<uint8_t>(sizeof(colors) - 1, index)] = color;
}
