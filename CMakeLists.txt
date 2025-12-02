cmake_minimum_required(VERSION 3.26...4.2)

project(object_subjects)

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

set(src 
    src/main.cpp
    )

set(hdrs
    src/sim/ifc_observer.h
    src/utils/vectormath.h
  )

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
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE pthread protobuf::libprotobuf)

enable_testing() 
add_subdirectory(tests) 
