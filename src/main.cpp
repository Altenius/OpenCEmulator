#include <iostream>
#include "OpenCEmulator.h"

int main(int argc, char *argv[]) {
    OpenCEmulator emulator(argc, argv);
    
    return emulator.exec();
}