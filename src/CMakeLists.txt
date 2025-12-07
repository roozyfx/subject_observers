set(PROJECT_LIBRARY_NAME subject_observers)

# TODO Add checks for Windows
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ ")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Weffc++")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions("DEBUG")
endif()

set(src 
    utils/config.cpp
    sim/player.cpp
    sim/sensor.cpp
    sim/simulation.cpp
    )

# Copy configuration file to build directory
file(COPY ${CMAKE_SOURCE_DIR}/configuration.toml DESTINATION ${CMAKE_BINARY_DIR})

# Find dependencies
# 0MQ
find_package(cppzmq REQUIRED)

# TOML++ 
include(FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

add_library(${PROJECT_LIBRARY_NAME} STATIC ${src})

target_compile_options(${PROJECT_LIBRARY_NAME} PRIVATE 
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
    TARGET ${PROJECT_LIBRARY_NAME}
    LANGUAGE cpp
    PROTOC_OUT_DIR ${PROTO_OUTPUT_DIR}
    IMPORT_DIRS protobuf 
    PROTOS 
    protobuf/position.proto
    )
endif()
    
target_include_directories(${PROJECT_LIBRARY_NAME} PUBLIC ${PROTO_OUTPUT_DIR} ../include)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_options(${PROJECT_LIBRARY_NAME} BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address)
endif()

target_link_libraries(${PROJECT_LIBRARY_NAME} PUBLIC pthread protobuf::libprotobuf tomlplusplus::tomlplusplus cppzmq)