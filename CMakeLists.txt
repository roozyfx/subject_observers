cmake_minimum_required(VERSION 3.26...4.2)

project(object_subjects VERSION 0.1.0
                        DESCRIPTION "Subject-Observers Simulation"
                        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(BUILD_TESTING "Build tests" ON)

add_subdirectory(src)
add_subdirectory(apps)

include_directories(${CMAKE_BINARY_DIR}/protobuf)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests) 
endif()