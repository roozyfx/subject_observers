include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)
set(PROJECT_LIBRARY_NAME subject_observers)

set(src
    utils/config.cpp
    sim/player.cpp
    sim/sensor.cpp
    sim/simulation.cpp
)

add_library(${PROJECT_LIBRARY_NAME} STATIC ${src})

target_include_directories(${PROJECT_LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

set_platform_specific_options(${PROJECT_LIBRARY_NAME})

target_compile_definitions(${PROJECT_LIBRARY_NAME} PRIVATE
    $<$<CONFIG:Debug>:-DDEBUG>
    $<$<CONFIG:Release>:-DNDEBUG>
)

# Find dependencies
# 0MQ
find_package(cppzmq REQUIRED)

# TOML++ 
include(FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

# First try to find user-compiled Protobuf, then system installation
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
find_package(Protobuf)
unset(CMAKE_FIND_PACKAGE_PREFER_CONFIG)

if(Protobuf_FOUND)
    set(PROTO_OUTPUT_DIR ${CMAKE_BINARY_DIR}/protobuf)
    file(MAKE_DIRECTORY ${PROTO_OUTPUT_DIR})

    protobuf_generate(
        TARGET ${PROJECT_LIBRARY_NAME}
        LANGUAGE cpp
        PROTOC_OUT_DIR ${PROTO_OUTPUT_DIR}
        IMPORT_DIRS protobuf
        PROTOS
        protobuf/position.proto
    )
endif()

# Generated .pb.cc/.pb.h land in the binary dir, so make them discoverable
target_include_directories(${PROJECT_LIBRARY_NAME} PUBLIC ${PROTO_OUTPUT_DIR} ${CMAKE_SOURCE_DIR}/include)

target_link_options(${PROJECT_LIBRARY_NAME} BEFORE PUBLIC
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

target_link_libraries(${PROJECT_LIBRARY_NAME} PRIVATE Threads::Threads tomlplusplus::tomlplusplus cppzmq)
target_link_libraries(${PROJECT_LIBRARY_NAME} PUBLIC protobuf::libprotobuf)
