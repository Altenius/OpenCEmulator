#ifndef OPENCEMULATOR_SCHEDULER_H
#define OPENCEMULATOR_SCHEDULER_H

#include <thread>

class OpenCEmulator;

class Scheduler {
public:
    Scheduler(OpenCEmulator *emulator);
    void start();
    void stop();
    
private:
    void tick();
    
    std::thread m_tickThread;
    
    bool m_stop;
    
    OpenCEmulator *m_emulator;
};


#endif //OPENCEMULATOR_SCHEDULER_H
