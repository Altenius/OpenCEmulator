#ifndef OPENCEMULATOR_CONFIGURATION_H
#define OPENCEMULATOR_CONFIGURATION_H

#include <string>
#include <rapidjson/document.h>

class Configuration
{
public:
    Configuration();

    /* Loads a configuration file. Calls readConfig()
     * after parsing the document. Returns false if
     * the file could not be opened. */
    bool load(const std::string &filename);

    /* Called by load() after parsing the document. */
    virtual void readConfig(rapidjson::Document &d) =0;

    /* Saves a document to a file. Returns false if the
     * the file could not be opened. */
    bool save(const std::string &filename, const rapidjson::Document &d);
};


#endif //OPENCEMULATOR_CONFIGURATION_H
