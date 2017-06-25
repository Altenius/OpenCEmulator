#ifndef OPENCEMULATOR_COMPONENTSERIALIZER_H
#define OPENCEMULATOR_COMPONENTSERIALIZER_H

#include <memory>
#include <vector>

class Component;

typedef std::shared_ptr<Component> ComponentPtr;

class ComponentSerializer
{
public:
    ComponentSerializer();

    void serialize(const std::vector<ComponentPtr> &components);
    
    void unserialize(std::vector<ComponentPtr> &components);
};


#endif //OPENCEMULATOR_COMPONENTSERIALIZER_H
