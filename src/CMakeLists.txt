set(PROJECT_LIBRARY_NAME subject_observers)

set(src
    utils/config.cpp
    sim/player.cpp
    sim/sensor.cpp
    sim/simulation.cpp
)

add_library(${PROJECT_LIBRARY_NAME} STATIC ${src})

target_include_directories(${PROJECT_LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Linux and macOS
if(CMAKE_SYSTEM_NAME MATCHES Linux OR CMAKE_SYSTEM_NAME MATCHES Darwin)
    target_compile_options(${PROJECT_LIBRARY_NAME} PRIVATE
        $<$<CONFIG:Debug>:-g;-O0;-fsanitize=address,undefined;-Wfloat-equal;-fno-omit-frame-pointer>
        $<$<CONFIG:Release>:-O3;-march=native>
    )
    # GCC
    if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
        target_compile_options(${PROJECT_LIBRARY_NAME} PRIVATE
            -Wall -Wextra -Wpedantic
        )
        # Clang
    elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang OR CMAKE_CXX_COMPILER_ID MATCHES AppleClang)
        target_compile_options(${PROJECT_LIBRARY_NAME} PRIVATE
            -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-weak-vtables -Wno-c++20-extensions -Wno-padded
        )
    endif()

    target_link_options(${PROJECT_LIBRARY_NAME} PUBLIC
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
    )

    # Windows branch
elseif(CMAKE_SYSTEM_NAME MATCHES Windows)
    target_compile_options(${PROJECT_LIBRARY_NAME} PRIVATE
        /W4 /permissive-
        $<$<CONFIG:Debug>:/Od;/Zi;/fsanitize=address,undefined>
        $<$<CONFIG:Release>:/O2;/DNDEBUG>
    )

    target_link_options(${PROJECT_LIBRARY_NAME} PUBLIC
        # MSVC AddressSanitizer requires incremental linking to be disabled
        $<$<CONFIG:Debug>:/INCREMENTAL:NO>
    )
endif()

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
