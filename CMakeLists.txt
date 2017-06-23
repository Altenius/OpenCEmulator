cmake_minimum_required(VERSION 3.1)


string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BUILD_DIR}" IS_IN_SOURCE_BUILD)
if(IS_IN_SOURCE_BUILD)
    message(AUTHOR_WARNING "Building OpenCEmulator in-source is unrecommended!")
endif()


if(WIN32)
    # In Qt 5.1+ we have our own main() function, don't autolink to qtmain on Windows
    cmake_policy(SET CMP0020 OLD)
endif()


project(OpenCEmulator)


## CMake options ##
set(CMAKE_INCLUDE_CURRENT_DIR ON)



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
if(UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
endif()


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)


find_package(Qt5Widgets)
find_package(Threads)

add_subdirectory(lib/eris)

set(RAPIDJSON_BUILD_TESTS off)

add_subdirectory(lib/rapidjson)
include_directories(lib/rapidjson/include)
add_subdirectory(src)