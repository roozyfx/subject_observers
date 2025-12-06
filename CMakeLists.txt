cmake_minimum_required(VERSION 3.26...4.2)

project(object_subjects VERSION 0.1.0
                        DESCRIPTION "Subject-Observers Simulation"
                        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TODO Add checks for Windows
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ ")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Weffc++")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions("DEBUG")
endif()

find_package(cppzmq REQUIRED)

# TOML++ 
include(FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

set(src 
    src/main.cpp
    src/utils/config.cpp
    src/sim/player.cpp
    src/sim/sensor.cpp
    src/sim/simulation.cpp
    )

set(hdrs
    src/sim/ifc_observer.h
    src/sim/player.h
    src/utils/config.cpp
    src/sim/sensor.h
    src/sim/simulation.h
    
    src/utils/config.h
    src/utils/rng.h
    src/utils/vectormath.h
  )

file(COPY ${CMAKE_SOURCE_DIR}/configuration.toml DESTINATION ${CMAKE_BINARY_DIR})

include_directories(src)

add_executable(${CMAKE_PROJECT_NAME} ${src} ${hdrs})

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE 
  -Wall -Wextra -Wpedantic
  $<$<CONFIG:Debug>:-g;-O0;-fsanitize=address,undefined;-Wfloat-equal;-fno-omit-frame-pointer>
  $<$<CONFIG:Release>:-O3;-march=native;-DNDEBUG>)

# First try to find user-compiled Protobuf, then system installation
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
find_package(Protobuf)
unset(CMAKE_FIND_PACKAGE_PREFER_CONFIG)

if(Protobuf_FOUND)
  set(PROTO_OUTPUT_DIR ${CMAKE_BINARY_DIR}/protobuf)
  file(MAKE_DIRECTORY ${PROTO_OUTPUT_DIR})
  protobuf_generate(
    TARGET ${CMAKE_PROJECT_NAME}
    LANGUAGE cpp
    PROTOC_OUT_DIR ${PROTO_OUTPUT_DIR}
    IMPORT_DIRS src/protobuf 
    PROTOS 
      src/protobuf/position.proto
  )
endif()

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROTO_OUTPUT_DIR})
if(CMAKE_BUILD_TYPE MATCHES "Debug")
  target_link_options(${CMAKE_PROJECT_NAME} BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address)
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE pthread protobuf::libprotobuf tomlplusplus::tomlplusplus cppzmq)

enable_testing() 
add_subdirectory(tests) 
