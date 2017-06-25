#ifndef OPENCEMULATOR_OPENCEMULATOR_H
#define OPENCEMULATOR_OPENCEMULATOR_H


#include <QApplication>
#include "ui/MainWindow.h"
#include "FontLoader.h"
#include "components/Component.h"
#include "Instance.h"
#include "InstancesConfiguration.h"
#include "ComponentsConfiguration.h"
#include <vector>
#include <components/ComponentSerializer.h>

class OpenCEmulator : public QApplication
{
Q_OBJECT
public:
    OpenCEmulator(int argc, char *argv[]);

    ~OpenCEmulator();

    void showMainWindow(bool minimized = false);



    /* Returns the list of instances. */
    inline std::vector<InstancePtr> &instances()
    {
        return m_instances;
    }



    /* Returns the global emulator object. */
    static OpenCEmulator &get();



    /* Returns the base directory of the emulator. This
     * is ~/.ocemulator on linux and C:\[User]\.ocemulator
     * on Windows. */
    const std::string &baseDirectory()
    {
        return m_baseDirectory;
    }



    /* Returns the filesystem directory of the emulator. By
     * default, this is BASE_DIRECTORY/files. */
    const std::string &filesystemDirectory()
    {
        return m_filesystemDirectory;
    }



    /* Returns the persistence directory for storing instances
     * states. By default this is BASE_DIRECTORY/persist */
    const std::string &persistDirectory()
    {
        return m_persistDirectory;
    }



    /* Returns the name of the file storing information about the
     * state of components. By default this is BASE_DIRECTORY/state */
    const std::string &stateFile()
    {
        return m_stateFile;
    }



    /* Returns the list of components. */
    inline std::vector<ComponentPtr> &components()
    {
        return m_components;
    }



    /* Adds a new component. Triggers componentsChanged(). */
    void addComponent(const ComponentPtr &component);

    /* Adds a new instance. Triggers instancesChanged(). */
    void addInstance(const InstancePtr &instance);

    /* Removes a component. Triggers componentsChanged(). */
    void removeComponent(const ComponentPtr &component);

    /* Removes an instance. Triggers instancesChanged(). */
    void removeInstance(const InstancePtr &instance);

    /* Attempts to find the component with the specified
     * address. Returns the component or a blank
     * ComponentPtr if a component could not be found. */
    ComponentPtr findComponent(const std::string &address);

    /* Saves the emulator state. */
    void save();

    /* Loads the emulator state */
    void load();

signals:

    void componentsChanged(const std::vector<ComponentPtr> &components);

    void instancesChanged(const std::vector<InstancePtr> &instances);

private:
    MainWindow *m_mainWindow = nullptr;
    Scheduler m_scheduler;
    std::string m_baseDirectory;
    std::string m_filesystemDirectory;
    std::string m_persistDirectory;
    std::string m_stateFile;

    InstancesConfiguration m_instancesConfig;
    ComponentsConfiguration m_componentsConfig;

    std::vector<ComponentPtr> m_components;
    std::vector<InstancePtr> m_instances;
    
    ComponentSerializer m_componentSerializer;

    /* Loads the components from components.json. */
    void loadComponents();

    /* Loads the instances from instances.json. */
    void loadInstances();
};


#endif //OPENCEMULATOR_OPENCEMULATOR_H
