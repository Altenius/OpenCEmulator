#ifndef OPENCEMULATOR_PALETTE_H
#define OPENCEMULATOR_PALETTE_H


#include <array>
#include <Color.h>

#define ONEBIT_COLOR 0xFFFFFF

class ColorPalette
{
public:
    ColorPalette();

    virtual uint32_t inflate(uint32_t index) =0;

    virtual uint8_t deflate(const Color &color) =0;

    virtual unsigned int at(unsigned char index) =0;

    virtual void set(unsigned char index, unsigned int color) =0;



    virtual inline bool isFromPalette(uint32_t color)
    {
        return false;
    }



    uint16_t pack(const Color &foreground, const Color &background);



    inline uint16_t extractForeground(uint16_t color)
    {
        return color >> 8;
    }



    inline uint16_t extractBackground(uint16_t color)
    {
        return color & 0xFF;
    }



    uint32_t unpackForeground(uint16_t color);

    uint32_t unpackBackground(uint16_t color);
};


class OneBitPalette : public ColorPalette
{
public:
    OneBitPalette();

    uint32_t inflate(uint32_t index) override;

    uint8_t deflate(const Color &color) override;

    unsigned int at(unsigned char index) override;

    void set(unsigned char index, unsigned int color) override;
};


class FourBitPalette : public ColorPalette
{
public:
    FourBitPalette();

    virtual uint32_t inflate(uint32_t index) override;

    virtual uint8_t deflate(const Color &color) override;

    unsigned int at(unsigned char index) override;

    void set(unsigned char index, unsigned int color) override;



    virtual inline bool isFromPalette(uint32_t color) override
    { return true; }



protected:
    float delta(int colorA, int colorB);

    static unsigned int colors[16];
};


class EightBitPalette : public FourBitPalette
{
public:
    EightBitPalette();

    uint32_t inflate(uint32_t index) override;

    uint8_t deflate(const Color &color) override;

    bool isFromPalette(uint32_t color) override;

protected:
    static unsigned int colors[16];
};


#endif //OPENCEMULATOR_PALETTE_H
