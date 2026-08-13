cmake_minimum_required(VERSION 3.26...4.4)

project(object_subjects VERSION 0.2.0
                        DESCRIPTION "Subject-Observers Simulation"
                        LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(apps)

include_directories(${CMAKE_BINARY_DIR}/protobuf)

include(CTest)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests) 
endif()