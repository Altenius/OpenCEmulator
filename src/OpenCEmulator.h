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

class OpenCEmulator : public QApplication {
    Q_OBJECT
public:
    OpenCEmulator(int argc, char *argv[]);
    ~OpenCEmulator();
    
    void showMainWindow(bool minimized = false);
    
    inline std::vector<InstancePtr> &instances() {
        return m_instances;
    }
    
    static OpenCEmulator &get();
    
    const std::string &baseDirectory() {
        return m_baseDirectory;
    }
    
    const std::string &filesystemDirectory() {
        return m_filesystemDirectory;
    }
    
    inline std::vector<ComponentPtr> &components() {
        return m_components;
    }
    
    void addComponent(const ComponentPtr &component);
    void addInstance(const InstancePtr &instance);
    
    void removeComponent(const ComponentPtr &component);
    
    void removeInstance(const InstancePtr &instance); 
    
    ComponentPtr findComponent(const std::string &address);
    
    void save();

signals:
    void componentsChanged(const std::vector<ComponentPtr> &components);
    void instancesChanged(const std::vector<InstancePtr> &instances);
    
private:
    MainWindow *m_mainWindow = nullptr;
    std::vector<InstancePtr> m_instances;
    std::vector<ComponentPtr> m_components;
    Scheduler m_scheduler;
    std::string m_baseDirectory;
    std::string m_filesystemDirectory;
    
    InstancesConfiguration m_instancesConfig;
    ComponentsConfiguration m_componentsConfig;
    
    void loadComponents();
    void loadInstances();
};


#endif //OPENCEMULATOR_OPENCEMULATOR_H
