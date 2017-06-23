#include "ComponentsConfiguration.h"
#include "OpenCEmulator.h"
#include <fstream>
#include <iostream>
#include "components/ComponentScreen.h"



void ComponentsConfiguration::initialize()
{
    if (!load(OpenCEmulator::get().baseDirectory() + "/components.json")) {
        std::ofstream file(OpenCEmulator::get().baseDirectory() + "/components.json");
        if (!file.is_open()) {
            std::cerr << "Failed to open components.json" << std::endl;
            return;
        }
        file << "{}";
        file.close();
    }
}



void ComponentsConfiguration::readConfig(rapidjson::Document &d)
{
    if (!d.IsObject()) {
        std::cerr << "Document is not an object; cannot load components.json" << std::endl;
        return;
    }
    for (auto &component : d.GetObject()) {
        if (!component.value.IsObject()) {
            std::cerr << "error: non-object located in root object of components.json" << std::endl;
            continue;
        }

        std::string labelString, typeString, dataString, nameString = component.name.GetString();

        for (auto &value : component.value.GetObject()) {
            std::string name = value.name.GetString();
            if (name == "type") {
                if (value.value.IsString()) {
                    typeString.assign(value.value.GetString());
                }
            } else if (name == "data") {
                if (value.value.IsString()) {
                    dataString = value.value.GetString();
                }
            } else if (name == "label") {
                if (value.value.IsString()) {
                    labelString.assign(value.value.GetString());
                }
            }
        }

        m_components.push_back(ComponentConfig(nameString, typeString, labelString, dataString));
    }
}



void ComponentsConfiguration::saveConfig(const std::vector<ComponentPtr> &components)
{
    rapidjson::Document d(rapidjson::kObjectType);

    for (const ComponentPtr &component : components) {
        rapidjson::Value componentData(rapidjson::kObjectType);

        rapidjson::Value type(rapidjson::kStringType);
        std::string sType = component->type();
        if (sType == "computer") {
            continue;
        }

        std::string extra;

        if (sType == "keyboard") {
            // save data about the connected screen

            if (ComponentPtr screen = std::static_pointer_cast<ComponentKeyboard>(component)->screen()) {
                extra.assign(screen->uuid());
            }
        }
        type.SetString(sType.c_str(), sType.length(), d.GetAllocator());
        componentData.AddMember("type", type, d.GetAllocator());

        if (!extra.empty()) {
            rapidjson::Value extraV(rapidjson::kStringType);
            extraV.SetString(extra.c_str(), extra.length(), d.GetAllocator());
            componentData.AddMember("data", extraV, d.GetAllocator());
        }

        const std::string &label = component->label();
        if (!label.empty()) {
            rapidjson::Value labelV(rapidjson::kStringType);
            labelV.SetString(label.c_str(), label.length(), d.GetAllocator());
            componentData.AddMember("label", labelV, d.GetAllocator());
        }

        rapidjson::Value uuid(rapidjson::kStringType);
        std::string sUuid = component->uuid();
        uuid.SetString(sUuid.c_str(), sUuid.length(), d.GetAllocator());
        d.AddMember(uuid, componentData, d.GetAllocator());
    }

    if (!save(OpenCEmulator::get().baseDirectory() + "/components.json", d)) {
        std::cerr << "failed to save components.json" << std::endl;
    }
}
