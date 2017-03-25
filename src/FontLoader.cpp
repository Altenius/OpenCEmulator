#include "FontLoader.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QBitmap>

void FontLoader::load() {
    QFile widthFile(":/font/wcwidth.bin");
    if (!widthFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open font width file: " << widthFile.errorString();
    }
    memset(m_widths, -1, GLYPHS_SIZE);
    if (widthFile.read(m_widths, GLYPHS_SIZE) == -1) {
        qWarning() << "Could not read from wcwidth.bin: " << widthFile.errorString();
    }
    widthFile.close();
    
    
    QFile file(":/font/font.hex");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not load font file: " << file.errorString();
        return;
    }
    
    QTextStream in(&file);
    
    int glyphCount = 0;
    
    while (!in.atEnd()) {
        QString line = in.readLine();
        
        QStringList info = line.split(':');
        const QString data = info[1];
        
        int charCode = info[0].toInt(nullptr, 16);
        if (charCode < 0 || charCode >= GLYPHS_SIZE) {
            continue;
        }
        
        // Two character represent one byte which represents one row of pixels (one glyph is 8 pixels wide)
        QVector<uchar> glyph(data.length() >> 1);
        int fWidth = glyph.length() / glyphHeight();
        int expectedWidth = width(charCode);
        if (expectedWidth != fWidth) {
            qWarning() << "Expected glyph width (" << expectedWidth << ") is not equal to real width (" << fWidth << ")";
            continue;
        }

        // TODO: Check expected width here
        for (int i = 0; i < glyph.length(); i++) {
            QStringRef part(&data, i * 2, 2);
            glyph[i] = (uchar)part.toInt(nullptr, 16);
            
            if (m_glyphs[charCode].empty()) {
                glyphCount++;
            }
            
            m_glyphs[charCode] = glyph;
        }
    }
    
    qInfo() << "Loaded" << glyphCount << "glyphs";
    file.close();
}

QBitmap FontLoader::getGlyph(int charCode) {
    if (charCode < 0 || charCode > GLYPHS_SIZE || m_glyphs[charCode].empty()) {
        return QBitmap();
    }
    
    return QBitmap::fromData(QSize(glyphWidth() * width(charCode), glyphHeight()), m_glyphs[charCode].data(), QImage::Format_Mono); // Format width
}

FontLoader &FontLoader::get() {
    static FontLoader loader;
    return loader;
}

FontLoader::FontLoader() {

}
