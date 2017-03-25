#include "ScreensWidget.h"
#include "FontLoader.h"
#include <QPainter>
#include <QBitmap>

ScreensWidget::ScreensWidget(QWidget *parent) : QWidget(parent) {

}

void ScreensWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 30));
    painter.drawText(rect(), Qt::AlignCenter, "Test");
    
    
    uchar bits[8];
    for (int i = 0; i < 8; i++ ){
        bits[i] = 0xC3;
    }
    
    // QBitmap bitmap = QBitmap::fromData(QSize(8, 8), bits);
    painter.drawPixmap(QPoint(20, 20), FontLoader::get().getGlyph(0x03a3));
}
