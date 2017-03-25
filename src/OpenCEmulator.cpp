#include "OpenCEmulator.h"
#include "FontLoader.h"

OpenCEmulator::OpenCEmulator(int argc, char **argv) : QApplication(argc, argv) {
    setOrganizationName("OpenCEmulator");
    setApplicationName("OpenCEmulator");
    setApplicationDisplayName("OpenCEmulator");
    
    FontLoader::get().load();
    
    showMainWindow();
}

OpenCEmulator::~OpenCEmulator() {
    
}

void OpenCEmulator::showMainWindow(bool minimized) {
    if (m_mainWindow) {
        
    } else {
        m_mainWindow = new MainWindow();
        
        if (minimized) {
            m_mainWindow->showMinimized();
        } else {
            m_mainWindow->show();
        }
    }
}
