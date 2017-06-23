#include "OpenCEmulator.h"
#include "Scheduler.h"
#include "Instance.h"
#include <QDebug>
#include <iostream>



Scheduler::Scheduler(OpenCEmulator *emulator) : m_emulator(emulator), m_stop(false)
{

}



void Scheduler::start()
{
    m_tickThread = std::thread(std::bind(&Scheduler::tick, this));
}



void Scheduler::stop()
{
    m_stop = true;
    m_tickThread.join();
}



void Scheduler::tick()
{
    std::vector<InstancePtr> &instances = m_emulator->instances();
    while (!m_stop) {
        for (InstancePtr &instance : instances) {
            if (instance->sleeping()) {
                instance->m_ticks--;
            }
            if (instance->shouldRun()) {
                if (!instance->run()) {
                    std::cerr << "Instance failed to run:" << instance->lastError().c_str() << std::endl;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
