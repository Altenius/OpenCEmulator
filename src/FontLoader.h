#ifndef OPENCEMULATOR_FONTLOADER_H
#define OPENCEMULATOR_FONTLOADER_H


#include <QObject>
#include <QVector>

#define GLYPHS_SIZE 0x10000

class FontLoader {
public:
    void load();
    
    inline static int glyphWidth() {
        return 8;
    }
    
    inline static int glyphHeight() {
        return 16;
    }
    
    static FontLoader &get();
    
    QBitmap getGlyph(int charCode);
    
    inline char width(int charCode) {
        return m_widths[charCode];
    }
private:
    QVector<uchar> m_glyphs[GLYPHS_SIZE];
    char m_widths[GLYPHS_SIZE];
    
    FontLoader();
};


#endif //OPENCEMULATOR_FONTLOADER_H
