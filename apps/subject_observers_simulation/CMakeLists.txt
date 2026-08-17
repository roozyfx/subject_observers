include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Find dependencies
# 0MQ
find_package(cppzmq REQUIRED)

foreach(_target     
        zeromq_server zeromq_receiver)
    add_executable(${_target} "${_target}.cpp")
    set_platform_specific_options(${_target})

    target_include_directories(${_target} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${_target} PRIVATE subject_observers cppzmq)
endforeach()

# Copy configuration file to build directory
# To ensure copy, first create a dedicated target JUST for copying the config file
add_custom_target(CopyConfig
    # Ensure the destination folder exists BEFORE copying (crucial for clean builds)
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:zeromq_server>"
    
    # Copy the file.
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/configuration.toml"
        "$<TARGET_FILE_DIR:zeromq_server>/configuration.toml"
    COMMENT "Synchronizing configuration.toml"
)
# make the main executable depend on CopyConfig
add_dependencies(zeromq_server CopyConfig)
