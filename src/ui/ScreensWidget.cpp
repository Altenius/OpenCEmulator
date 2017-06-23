#include "ScreensWidget.h"
#include "FontLoader.h"
#include <QPainter>
#include <QBitmap>
#include <iostream>
#include "components/ComponentScreen.h"


static const std::unordered_map<int, int> keyMap = {
        {Qt::Key_Escape, 1},
        {Qt::Key_Tab, 15},
        {Qt::Key_Backspace, 14},
        {Qt::Key_Return, 28},
        {Qt::Key_Enter, 28},
        {Qt::Key_Insert, 210},
        {Qt::Key_Delete, 211},
        {Qt::Key_Pause, 197},
        //{Qt::Key_Print, }
        {Qt::Key_SysReq, 183},
        {Qt::Key_Home, 199},
        {Qt::Key_End, 207},
        {Qt::Key_Left, 203},
        {Qt::Key_Up, 200},
        {Qt::Key_Right, 205},
        {Qt::Key_Down, 208},
        {Qt::Key_PageUp, 201},
        {Qt::Key_PageDown, 209},
        {Qt::Key_Shift, 42}, // 42 is left shift
        {Qt::Key_Control, 29}, // command on macos
        {Qt::Key_Meta, 219},
        {Qt::Key_Alt, 56}, // 56 is left alt
        {Qt::Key_CapsLock, 58},
        {Qt::Key_NumLock, 69},
        {Qt::Key_ScrollLock, 70},
        {Qt::Key_F1, 59},
        {Qt::Key_F2, 60},
        {Qt::Key_F3, 61},
        {Qt::Key_F4, 62},
        {Qt::Key_F5, 63},
        {Qt::Key_F6, 64},
        {Qt::Key_F7, 65},
        {Qt::Key_F8, 66},
        {Qt::Key_F9, 67},
        {Qt::Key_F10, 68},
        {Qt::Key_F11, 87},
        {Qt::Key_F12, 88},
        {Qt::Key_Super_L, 219},
        {Qt::Key_Super_R, 220},
        {Qt::Key_Menu, 184},
        {Qt::Key_Space, 57},
        {Qt::Key_Comma, 51},
        {Qt::Key_Period, 52},
        {Qt::Key_Slash, 53},
        {Qt::Key_Minus, 12},
        {Qt::Key_Underscore, 147},
        {Qt::Key_Dead_Grave, 41},
        {Qt::Key_1, 2},
        {Qt::Key_2, 3},
        {Qt::Key_At, 145},
        {Qt::Key_3, 4},
        {Qt::Key_4, 5},
        {Qt::Key_5, 6},
        {Qt::Key_6, 7},
        {Qt::Key_7, 8},
        {Qt::Key_8, 9},
        {Qt::Key_9, 10},
        {Qt::Key_0, 11},
        {Qt::Key_Q, 16},
        {Qt::Key_W, 17},
        {Qt::Key_E, 18},
        {Qt::Key_R, 19},
        {Qt::Key_T, 20},
        {Qt::Key_Y, 21},
        {Qt::Key_U, 22},
        {Qt::Key_I, 23},
        {Qt::Key_O, 24},
        {Qt::Key_P, 25},
        {Qt::Key_BracketLeft, 26},
        {Qt::Key_BracketRight, 27},
        {Qt::Key_Backslash, 43},
        {Qt::Key_A, 30},
        {Qt::Key_S, 31},
        {Qt::Key_D, 32},
        {Qt::Key_F, 33},
        {Qt::Key_G, 34},
        {Qt::Key_H, 35},
        {Qt::Key_J, 36},
        {Qt::Key_K, 37},
        {Qt::Key_L, 38},
        {Qt::Key_Semicolon, 39},
        {Qt::Key_Colon, 146},
        {Qt::Key_Apostrophe, 40},
        {Qt::Key_Z, 44},
        {Qt::Key_X, 45},
        {Qt::Key_C, 46},
        {Qt::Key_V, 47},
        {Qt::Key_B, 48},
        {Qt::Key_N, 49},
        {Qt::Key_M, 50},
        {Qt::Key_NumLock, 69}
        /*{Qt::Key_division, 181}, // NumLock stuff, fix this later
        {Qt::Key_multiply, 55},
        {Qt::Key_Plus, 78},
        {Qt::Key_}*/
};

ScreensWidget::ScreensWidget(QWidget *parent) : QWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);
}



void ScreensWidget::paintEvent(QPaintEvent *event) {
    if (auto component = m_screen.lock()) {
        auto screen = std::static_pointer_cast<ComponentScreen>(component);
        QPainter painter(this);

        /*painter.setPen(Qt::blue);
        painter.setFont(QFont("Arial", 30));
        painter.drawText(rect(), Qt::AlignCenter, "Test");
        
        
        uchar bits[8];
        for (int i = 0; i < 8; i++ ){
            bits[i] = 0xC3;
        }
        
        // QBitmap bitmap = QBitmap::fromData(QSize(8, 8), bits);
        painter.drawPixmap(QPoint(20, 20), FontLoader::get().getGlyph(0x03a3));*/
        
        const QPixmap &buffer = screen->buffer();
        
        if (buffer.width() > width() || buffer.height() > height()) {
            painter.drawPixmap(0, 0, buffer.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            int x = (width() - buffer.width()) / 2;
            int y = (height() - buffer.height()) / 2;
            painter.drawPixmap(x, y, buffer.width(), buffer.height(), buffer);
        }
    }
}



void ScreensWidget::keyPressEvent(QKeyEvent *event) {
    if (auto component = m_keyboard.lock()) {
        auto keyboard = std::static_pointer_cast<ComponentKeyboard>(component);
        if (event->text().isEmpty()) {
            keyboard->keyPressed(0, translateKey(event->key()));
        } else {
            keyboard->keyPressed(translateChar(event->text()[0].unicode()), translateKey(event->key()));
        }
    }
}



void ScreensWidget::keyReleaseEvent(QKeyEvent *event) {
    if (auto component = m_keyboard.lock()) {
        auto keyboard = std::static_pointer_cast<ComponentKeyboard>(component);
        if (event->text().isEmpty()) {
            keyboard->keyReleased(0, translateKey(event->key()));
        } else {
            keyboard->keyReleased(translateChar(event->text()[0].unicode()), translateKey(event->key()));
        }
    }
}



int ScreensWidget::translateKey(int key) {
    auto it = keyMap.find(key);
    if (it != keyMap.end()) {
        return it->second;
    }
    return key;
}



ushort ScreensWidget::translateChar(ushort c) {
    return c;
}
