OpenComputers Emulator
======================


Dependencies
------------
* Qt5
* CMake


Building
--------
Clone the repository
`git clone https://github.com/Altenius/OpenCEmulator.git`
cd into the directory
`cd OpenCEmulator`
Create a separate build directory and cd into it
`mkdir build && cd build`
Initialize cmake
`cmake ..`
Build
`make -j8`
The binary will be located in src/OpenCEmulator
`./src/OpenCEmulator`


Configuration
-------------
OpenCEmulator can be configured from the json configuration files or the GUI.

To configure directly using the files, locate components.json and instances.json in `~/.ocemulator` on linux or `C:\[User]\.ocemulator` on Windows


Contributing
------------


Usage
-----
