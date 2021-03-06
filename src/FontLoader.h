#ifndef OPENCEMULATOR_FONTLOADER_H
#define OPENCEMULATOR_FONTLOADER_H


#include <QObject>
#include <QVector>

#define GLYPHS_SIZE 0x10000

class FontLoader
{
public:
    void load();



    /* Returns the width in pixels of a glyph. */
    inline static int glyphWidth()
    {
        return 8;
    }



    /* Returns the height in pixels of a glyph. */
    inline static int glyphHeight()
    {
        return 16;
    }



    /* Returns the global FontLoader instance. */
    static FontLoader &get();

    /* Returns the gylph for a character */
    QBitmap getGlyph(int charCode);



    /* Returns the width in glyph-width-units for a character */
    inline char width(int charCode)
    {
        return m_widths[charCode];
    }



private:
    QVector<uchar> m_glyphs[GLYPHS_SIZE];
    char m_widths[GLYPHS_SIZE];

    FontLoader();
};


#endif //OPENCEMULATOR_FONTLOADER_H
