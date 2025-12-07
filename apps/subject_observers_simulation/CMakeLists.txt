set(GENERATOR_BINARY subject_observers_simulation)

add_executable(${GENERATOR_BINARY} subject_data_generator.cpp)
target_compile_options(${GENERATOR_BINARY} PRIVATE 
    -Wall -Wextra -Wpedantic
    $<$<CONFIG:Debug>:-g;-O0;-fsanitize=address,undefined;-Wfloat-equal;-fno-omit-frame-pointer>
    $<$<CONFIG:Release>:-O3;-march=native;-DNDEBUG>)

target_link_libraries(${GENERATOR_BINARY} PRIVATE subject_observers)

add_executable(receiver receiver.cpp)
target_compile_options(receiver PRIVATE 
    -Wall -Wextra -Wpedantic
    $<$<CONFIG:Debug>:-g;-O0;-fsanitize=address,undefined;-Wfloat-equal;-fno-omit-frame-pointer>
    $<$<CONFIG:Release>:-O3;-march=native;-DNDEBUG>)
target_link_libraries(receiver PRIVATE subject_observers)