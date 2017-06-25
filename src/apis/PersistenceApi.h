#ifndef OPENCEMULATOR_PERSISTENCEAPI_H
#define OPENCEMULATOR_PERSISTENCEAPI_H


#include "Api.h"
#include <QVector>

class PersistenceApi : public Api
{
public:
    PersistenceApi(Instance *instance);

    virtual void load() override;

    void persist(std::vector<char> &vector);

    void unpersist(const std::vector<char> &vector);

private:
    void configure();

public:
    static int lpersistKey(lua_State *state);
};


#endif //OPENCEMULATOR_PERSISTENCEAPI_H
