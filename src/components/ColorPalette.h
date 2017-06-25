#ifndef OPENCEMULATOR_PALETTE_H
#define OPENCEMULATOR_PALETTE_H


#include <array>
#include <memory>
#include <ReadBuffer.h>
#include "Color.h"
#include "WriteBuffer.h"

#define ONEBIT_COLOR 0xFFFFFF


class ColorPalette;

typedef std::shared_ptr<ColorPalette> ColorPalettePtr;


class ColorPalette
{
public:
    ColorPalette();

    static ColorPalettePtr create(ReadBuffer &buffer);

    static ColorPalettePtr create(uint8_t depth);

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

    virtual bool serialize(WriteBuffer &buffer) =0;

    virtual bool unserialize(ReadBuffer &buffer) =0;
};


class OneBitPalette : public ColorPalette
{
public:
    OneBitPalette();

    uint32_t inflate(uint32_t index) override;

    uint8_t deflate(const Color &color) override;

    unsigned int at(unsigned char index) override;

    void set(unsigned char index, unsigned int color) override;

    virtual bool serialize(WriteBuffer &buffer);

    virtual bool unserialize(ReadBuffer &buffer) override;
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



    virtual bool serialize(WriteBuffer &buffer);

    virtual bool unserialize(ReadBuffer &buffer) override;


protected:
    float delta(int colorA, int colorB);

    unsigned int colors[16];
};


class EightBitPalette : public FourBitPalette
{
public:
    EightBitPalette();

    uint32_t inflate(uint32_t index) override;

    uint8_t deflate(const Color &color) override;

    bool isFromPalette(uint32_t color) override;

    virtual bool serialize(WriteBuffer &buffer) override;

    virtual bool unserialize(ReadBuffer &buffer) override;
};


#endif //OPENCEMULATOR_PALETTE_H
