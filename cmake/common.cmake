MACRO(set_platform_specific_options target)
    # Linux and macOS Branch
    if(CMAKE_SYSTEM_NAME MATCHES Linux OR CMAKE_SYSTEM_NAME MATCHES Darwin)
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Debug>:-g;-O0;-fsanitize=address,undefined;-Wfloat-equal;-fno-omit-frame-pointer>
            $<$<CONFIG:Release>:-O3;-march=native>
        )
        # GCC
        if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
            target_compile_options(${target} PRIVATE
                -Wall -Wextra -Wpedantic
            )
            # Clang
        elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang OR CMAKE_CXX_COMPILER_ID MATCHES AppleClang)
            target_compile_options(${target} PRIVATE
                -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-weak-vtables -Wno-c++20-extensions -Wno-padded
                # TODO remove and fix warnings
                # -Wno-unsafe-buffer-usage
            )
        endif()
        target_link_options(${target} PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address,undefined>
        )
        # Windows branch
    elseif(CMAKE_SYSTEM_NAME MATCHES Windows)
        target_compile_options(${target} PRIVATE
            /W4 /permissive-
            $<$<CONFIG:Debug>:/Od;/Zi;/fsanitize=address,undefined>
            $<$<CONFIG:Release>:/O2;/DNDEBUG>
        )

        target_link_options(${target} PRIVATE
            # MSVC AddressSanitizer requires incremental linking to be disabled
            $<$<CONFIG:Debug>:/INCREMENTAL:NO>
        )
    endif()

ENDMACRO()
