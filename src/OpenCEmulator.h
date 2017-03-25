#ifndef OPENCEMULATOR_OPENCEMULATOR_H
#define OPENCEMULATOR_OPENCEMULATOR_H


#include <QApplication>
#include "MainWindow.h"
#include "FontLoader.h"

class OpenCEmulator : public QApplication {
    Q_OBJECT
public:
    OpenCEmulator(int argc, char *argv[]);
    ~OpenCEmulator();
    
    void showMainWindow(bool minimized = false);
private:
    MainWindow *m_mainWindow = nullptr;
};


#endif //OPENCEMULATOR_OPENCEMULATOR_H
