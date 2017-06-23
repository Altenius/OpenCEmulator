#ifndef OPENCEMULATOR_API_H
#define OPENCEMULATOR_API_H

#include "lua.hpp"

class Instance;
// The current api system is disgusting
class Api {
public:
    Api(Instance *instance);
    
    virtual void load() =0;

protected:
    Instance *m_instance;
};


#endif //OPENCEMULATOR_API_H
