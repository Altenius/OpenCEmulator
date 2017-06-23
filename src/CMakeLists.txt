cmake_minimum_required(VERSION 3.1)

project(OpenCEmulator)

set(OCE_SOURCES
        main.cpp
        OpenCEmulator.cpp
        OpenCEmulator.h
        
        ui/MainWindow.cpp
        ui/MainWindow.h
        ui/ScreensWidget.cpp
        ui/ScreensWidget.h
        ui/ManageComponents.cpp
        ui/ManageComponents.h
        ui/ComponentModifier.cpp
        ui/ComponentModifier.h
        ui/KeyboardModifier.cpp
        ui/KeyboardModifier.h
        ui/CreateComponent.cpp
        ui/CreateComponent.h
        ui/ManageInstances.cpp
        ui/ManageInstances.h
        
        FontLoader.cpp
        FontLoader.h
        
        Instance.cpp
        Instance.h

        ArgList.cpp
        ArgList.h
        
        apis/ComponentApi.cpp
        apis/ComponentApi.h
        apis/Api.cpp
        apis/Api.h
        apis/ComputerApi.cpp
        apis/ComputerApi.h
        apis/OsApi.cpp
        apis/OsApi.h
        apis/PersistenceApi.cpp
        apis/PersistenceApi.h
        apis/SystemApi.cpp
        apis/SystemApi.h
        apis/UnicodeApi.cpp
        apis/UnicodeApi.h
        apis/UserdataApi.cpp
        apis/UserdataApi.h

        Scheduler.cpp
        Scheduler.h

        components/Component.cpp
        components/Component.h
        components/ComponentEEPROM.cpp
        components/ComponentEEPROM.h
        components/ComponentFilesystem.cpp
        components/ComponentFilesystem.h
        components/ComponentRealFilesystem.cpp
        components/ComponentRealFilesystem.h
        components/ComponentComputer.cpp
        components/ComponentComputer.h
        components/ComponentScreen.cpp
        components/ComponentScreen.h
        components/ComponentKeyboard.cpp
        components/ComponentKeyboard.h
        components/ComponentGPU.cpp
        components/ComponentGPU.h
        components/ColorPalette.cpp
        components/ColorPalette.h
        
        Color.cpp
        Color.h
        
        Configuration.cpp
        Configuration.h
        InstancesConfiguration.cpp
        InstancesConfiguration.h
        ComponentsConfiguration.cpp
        ComponentsConfiguration.h
        )

set(OCE_UIS
        ui/mainwindow.ui
        ui/managecomponents.ui
        ui/keyboardmodifier.ui
        ui/createcomponent.ui
        ui/manageinstances.ui
        )

set(OCE_QRCS
        resources/font.qrc
        resources/lua/lua.qrc
        )


qt5_wrap_ui(OCE_UI ${OCE_UIS})
qt5_add_resources(OCE_RESOURCES ${OCE_QRCS})

set(CMAKE_AUTOMOC ON)

add_executable(OpenCEmulator ${OCE_SOURCES} ${OCE_UI} ${OCE_RESOURCES})
target_link_libraries(OpenCEmulator Eris Qt5::Widgets ${CMAKE_THREAD_LIBS_INIT})
include_directories(../lib/rapidjson/include)