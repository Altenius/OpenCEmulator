#include <OpenCEmulator.h>
#include <fstream>
#include <iostream>
#include <ReadStringBuffer.h>
#include "ComponentSerializer.h"
#include "components/Component.h"
#include "WriteStringBuffer.h"


ComponentSerializer::ComponentSerializer()
{

}



void ComponentSerializer::serialize(const std::vector<ComponentPtr> &components)
{
    WriteStringBuffer buffer;
    buffer.writeBE<uint32_t>(components.size());

    for (const ComponentPtr &component : components) {
        buffer.writeString(component->uuid());
        WriteStringBuffer data;
        component->serialize(data);
        buffer.writeBE<uint32_t>(data.buffer().size());
        buffer.writeBuffer(data.buffer().data(), data.buffer().size());
    }

    std::ofstream file(OpenCEmulator::get().stateFile(), std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "could not open the state file. Component states will not be saved" << std::endl;
        return;
    }

    file.write(buffer.buffer().data(), buffer.buffer().size());

    file.close();
}



void ComponentSerializer::unserialize(std::vector<ComponentPtr> &components)
{
    std::ifstream file(OpenCEmulator::get().stateFile(), std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "could not open the state file. Component states will not be loaded" << std::endl;
        return;
    }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    char data[size];
    file.read(data, size);
    
    ReadStringBuffer buffer(data, size);
    
    uint32_t componentCount;
    if (!buffer.readBE<uint32_t>(componentCount)) {
        goto fail;
    }
    
    for (; componentCount > 0; componentCount--) {
        std::string address;
        if (!buffer.readString(address)) {
            goto fail;
        }
        
        std::cout << "Unserializing component with address " << address << std::endl;
        
        uint32_t dataSize;
        if (!buffer.readBE<uint32_t>(dataSize)) {
            goto fail;
        }
        
        auto componentIt = std::find_if(components.begin(), components.end(), [address](ComponentPtr &c) {return c->uuid() == address; });
        if (componentIt == components.end()) {
            std::cerr << "component with address " << address << " does not exist; cannot unserialize";
            buffer.seek(dataSize, ReadBuffer::SK_CUR);
            continue;
        }
        
        if (!(*componentIt)->unserialize(buffer)) {
            goto fail;
        }
    }
    
    return;
    
    fail:
    std::cerr << "Could not unserialize components: end of file" << std::endl;
    return;
}
