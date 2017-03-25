cmake_minimum_required(VERSION 3.1)

project(OpenCEmulator)

set(OCE_SOURCES
        main.cpp
        OpenCEmulator.cpp
        OpenCEmulator.h
        
        MainWindow.cpp
        MainWindow.h
        ScreensWidget.cpp
        ScreensWidget.h
        
        FontLoader.cpp
        FontLoader.h)

set(OCE_UIS
        )

set(OCE_QRCS
        resources/font.qrc
        )


qt5_wrap_ui(OCE_UI ${OCE_UIS})
qt5_add_resources(OCE_RESOURCES ${OCE_QRCS})


add_executable(OpenCEmulator ${OCE_SOURCES} ${OCE_UI} ${OCE_RESOURCES})
target_link_libraries(OpenCEmulator Qt5::Widgets)