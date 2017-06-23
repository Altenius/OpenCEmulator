#include "OpenCEmulator.h"
#include "FontLoader.h"
#include "Instance.h"
#include <QDebug>
#include <iostream>
#include <QtCore/QDir>
#include "components/ComponentEEPROM.h"
#include "components/ComponentRealFilesystem.h"
#include "components/ComponentComputer.h"
#include "components/ComponentGPU.h"
#include "components/ComponentScreen.h"

OpenCEmulator *global;



OpenCEmulator::OpenCEmulator(int argc, char **argv) : QApplication(argc, argv), m_scheduler(this)
{
    global = this;
    setOrganizationName("OpenCEmulator");
    setApplicationName("OpenCEmulator");
    setApplicationDisplayName("OpenCEmulator");

    FontLoader::get().load();

    showMainWindow();

    m_scheduler.start();

    m_baseDirectory = QDir::homePath().toStdString().append("/.ocemulator");
    m_filesystemDirectory = m_baseDirectory + "/files";

    QDir(QDir::homePath()).mkpath(".ocemulator");
    QDir(QString::fromStdString(m_baseDirectory)).mkpath("files");

    loadComponents();
    loadInstances();
}



OpenCEmulator::~OpenCEmulator()
{
    for (ComponentPtr &component : m_components) {
        component->cleanup();
    }
    m_scheduler.stop();
}



void OpenCEmulator::showMainWindow(bool minimized)
{
    if (!m_mainWindow) {
        m_mainWindow = new MainWindow();

        if (minimized) {
            m_mainWindow->showMinimized();
        } else {
            m_mainWindow->show();
        }
    }
}



OpenCEmulator &OpenCEmulator::get()
{
    return *global;
}



ComponentPtr OpenCEmulator::findComponent(const std::string &address)
{
    auto it = std::find_if(m_components.begin(), m_components.end(), [address](ComponentPtr c) {
        return c->uuid() == address;
    });

    if (it != m_components.end()) {
        return *it;
    }

    return nullptr;
}



void OpenCEmulator::addComponent(const ComponentPtr &component)
{
    m_components.push_back(component);
    emit componentsChanged(m_components);
}



void OpenCEmulator::addInstance(const InstancePtr &instance)
{
    m_instances.push_back(instance);
    emit instancesChanged(m_instances);
}



void OpenCEmulator::removeInstance(const InstancePtr &instance)
{
    m_instances.erase(std::find(m_instances.begin(), m_instances.end(), instance));
    emit instancesChanged(m_instances);
}



void OpenCEmulator::removeComponent(const ComponentPtr &component)
{
    component->cleanup();
    m_components.erase(std::find(m_components.begin(), m_components.end(), component));
    emit componentsChanged(m_components);
}



void OpenCEmulator::save()
{
    m_componentsConfig.saveConfig(m_components);
    m_instancesConfig.saveConfig(m_instances);
}



void OpenCEmulator::loadComponents()
{
    m_componentsConfig.initialize();

    for (const ComponentConfig &componentConfig : m_componentsConfig.components()) {
        Component::create(componentConfig.type(), componentConfig.name(), componentConfig.label(),
                          componentConfig.data());
    }
}



void OpenCEmulator::loadInstances()
{
    m_instancesConfig.initialize();

    for (const InstanceConfig &config : m_instancesConfig.instances()) {
        InstancePtr instance(new Instance(4 * 1024 * 1024, config.label(), config.uuid())); // 4MiB

        for (const std::string &uuid : config.components()) {
            ComponentPtr component = findComponent(uuid);
            if (!component) {
                std::cerr << "Could not find component '" << uuid << "' for instance '" << config.uuid() << "'"
                          << std::endl;
                continue;
            }
            instance->attachComponent(component);
            component->attach(instance);

            std::cout << "attached component " << component->uuid() << " (" << component->type() << ") to instance "
                      << instance->address() << std::endl;
        }

        if (!instance->initialize()) {
            std::cerr << "Failed to initialize instance" << std::endl;
        } else {
            addInstance(instance);
        }
    }
}
