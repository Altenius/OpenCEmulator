#include "OpenCEmulator.h"
#include "FontLoader.h"
#include "Instance.h"
#include <QDebug>
#include <iostream>
#include "components/ComponentEEPROM.h"
#include "components/ComponentRealFilesystem.h"
#include "components/ComponentComputer.h"
#include "components/ComponentGPU.h"
#include "components/ComponentScreen.h"

OpenCEmulator *global;

OpenCEmulator::OpenCEmulator(int argc, char **argv) : QApplication(argc, argv), m_scheduler(this) {
    global = this;
    setOrganizationName("OpenCEmulator");
    setApplicationName("OpenCEmulator");
    setApplicationDisplayName("OpenCEmulator");
    
    FontLoader::get().load();
    
    showMainWindow();
    
    m_scheduler.start();
    
    m_baseDirectory = QDir::homePath().toStdString().append("/.ocemulator");
    m_filesystemDirectory = m_baseDirectory + "/files";
    
    QDir(QString::fromStdString(m_baseDirectory)).mkpath(".");

    m_instancesConfig.initialize();
    m_componentsConfig.initialize();
    
    bool defaultKeyboard = false;
    
    for (const ComponentConfig &componentConfig : m_componentsConfig.components()) {
        const std::string &type = componentConfig.type();
        const std::string &uuid = componentConfig.name();
        const std::string &label = componentConfig.label();
        
        if (type == "eeprom") {
            addComponent(ComponentPtr(new ComponentEEPROM(uuid, label)));
        } else if (type == "filesystem") {
            addComponent(ComponentPtr(new ComponentRealFilesystem(uuid, label)));
        } else if (type == "screen") {
            ComponentScreen *screen = new ComponentScreen(uuid, label);
            addComponent(ComponentPtr(screen));
        } else if (type == "keyboard") {
            ComponentPtr component = findComponent(componentConfig.extra());
            if (!component) {
                std::cerr << "Could not attach keyboard: screen not found with uuid '" << componentConfig.extra() << "'" << std::endl;
                continue;
            }
            ComponentPtr keyboard(new ComponentKeyboard(component, uuid, label));
            addComponent(keyboard);
            
            std::static_pointer_cast<ComponentScreen>(component)->attachKeyboard(keyboard);
        } else if (type == "gpu") {
            // gpu should be rebound to the screen in the future
            addComponent(ComponentPtr(new ComponentGPU(uuid, label)));
        } else {
            std::cerr << "unknown component type: " << type << std::endl;
            continue;
        }
        
        std::cout << "added component " << type << " with uuid " << uuid << std::endl;
    }
    
    for (const InstanceConfig &config : m_instancesConfig.instances()) {
        InstancePtr instance(new Instance(4 * 1024 * 1024, config.label(), config.uuid())); // 4MiB
        ComponentPtr computer(new ComponentComputer(instance.get()));
        addComponent(computer);
        computer->attach(instance);
        instance->attachComponent(computer);
        
        for (const std::string &uuid : config.components()) {
            ComponentPtr component = findComponent(uuid);
            if (!component) {
                std::cerr << "Could not find component '" << uuid << "' for instance '" << config.uuid() << "'" << std::endl;
                continue;
            }
            instance->attachComponent(component);
            component->attach(instance);
            
            std::cout << "attached component " << component->uuid() << " (" << component->type() << ") to instance " << instance->address() << std::endl;
        }

        if (!instance->initialize()) {
            std::cerr << "Failed to initialize instance" << std::endl;
        } else {
            addInstance(instance);
        }
    }


    /*InstancePtr instance(new Instance(4 * 1024 * 1024));
    ComponentPtr eeprom(new ComponentEEPROM);
    ComponentPtr filesystem(new ComponentRealFilesystem("5958eb4d-d905-7998-a6db-9d42c5f5f005"));
    ComponentPtr computer(new ComponentComputer(instance.get()));
    ComponentPtr gpu(new ComponentGPU);
    ComponentPtr screen(new ComponentScreen);
    ComponentPtr keyboard(new ComponentKeyboard(static_cast<ComponentScreen*>(screen.get())));
    m_components.push_back(eeprom);
    m_components.push_back(filesystem);
    m_components.push_back(computer);
    m_components.push_back(gpu);
    m_components.push_back(screen);
    m_components.push_back(keyboard);
    eeprom->attach(instance.get());
    instance->attachComponent(eeprom.get());
    filesystem->attach(instance.get());
    instance->attachComponent(filesystem.get());
    computer->attach(instance.get());
    instance->attachComponent(computer.get());
    gpu->attach(instance.get());
    instance->attachComponent(gpu.get());
    screen->attach(instance.get());
    instance->attachComponent(screen.get());
    keyboard->attach(instance.get());
    instance->attachComponent(keyboard.get());
    
    m_mainWindow->screens()->setScreen(static_cast<ComponentScreen*>(screen.get()));
    m_mainWindow->screens()->setKeyboard(static_cast<ComponentKeyboard*>(keyboard.get()));
    static_cast<ComponentScreen*>((screen.get()))->setWidget(m_mainWindow->screens());

    instance->initialize();
    m_instances.push_back(instance);*/

    // m_componentsConfig.saveConfig(m_components);
    // m_instancesConfig.saveConfig(m_instances)
}



OpenCEmulator::~OpenCEmulator() {
    for (ComponentPtr &component : m_components) {
        component->cleanup();
    }
    m_scheduler.stop();
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



OpenCEmulator &OpenCEmulator::get() {
    return *global;
}



ComponentPtr OpenCEmulator::findComponent(const std::string &address) {
    auto it = std::find_if(m_components.begin(), m_components.end(), [address](ComponentPtr c) {
        return c->uuid() == address;
    });
    
    if (it != m_components.end()) {
        return *it;
    }
    
    return nullptr;
}

void OpenCEmulator::addComponent(const ComponentPtr &component) {
    m_components.push_back(component);
    emit componentsChanged(m_components);
}



void OpenCEmulator::addInstance(const InstancePtr &instance) {
    m_instances.push_back(instance);
    emit instancesChanged(m_instances);
}



void OpenCEmulator::removeInstance(const InstancePtr &instance) {
    m_instances.erase(std::find(m_instances.begin(), m_instances.end(), instance));
    emit instancesChanged(m_instances);
}



void OpenCEmulator::removeComponent(const ComponentPtr &component) {
    component->cleanup();
    m_components.erase(std::find(m_components.begin(), m_components.end(), component));
    emit componentsChanged(m_components);
}



void OpenCEmulator::save() {
    m_componentsConfig.saveConfig(m_components);
    m_instancesConfig.saveConfig(m_instances);
}
