#ifndef OPENCEMULATOR_CONFIGURATION_H
#define OPENCEMULATOR_CONFIGURATION_H

#include <string>
#include <rapidjson/document.h>

class Configuration {
public:
    Configuration();
    
    bool load(const std::string &filename);
    
    virtual void readConfig(rapidjson::Document &d) =0;
    
    bool save(const std::string &filename, const rapidjson::Document &d);
};


#endif //OPENCEMULATOR_CONFIGURATION_H
