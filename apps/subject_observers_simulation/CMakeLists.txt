include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

set(GENERATOR_BINARY subject_observers_simulation)

add_executable(${GENERATOR_BINARY} subject_data_generator.cpp)

target_include_directories(${GENERATOR_BINARY} PUBLIC ${CMAKE_SOURCE_DIR}/include)
# Copy configuration file to build directory
# To ensure copy, first create a dedicated target JUST for copying the config file
add_custom_target(CopyConfig
    # Ensure the destination folder exists BEFORE copying (crucial for clean builds)
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${GENERATOR_BINARY}>"
    
    # Copy the file.
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/configuration.toml"
        "$<TARGET_FILE_DIR:${GENERATOR_BINARY}>/configuration.toml"
    COMMENT "Synchronizing configuration.toml"
)
# make the main executable depend on CopyConfig
add_dependencies(${GENERATOR_BINARY} CopyConfig)

set_platform_specific_options(${GENERATOR_BINARY})

target_link_libraries(${GENERATOR_BINARY} PRIVATE subject_observers)

##################################################################################
add_executable(receiver receiver.cpp)


set_platform_specific_options(receiver)

target_include_directories(receiver PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(receiver PRIVATE subject_observers)



