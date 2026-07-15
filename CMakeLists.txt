cmake_minimum_required(VERSION 3.25)

project(emerald_maze VERSION 1.0.0 LANGUAGES C)

include(CTest)

option(EMERALD_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(SDL2 2.0 REQUIRED)

add_library(emerald_core STATIC
    action.c
    base.c
    direction.c
    entity.c
    grid.c
    state.c
)

target_include_directories(emerald_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(emerald_core PRIVATE
    $<$<C_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic -Wconversion -Wshadow>
)

add_library(emerald_display STATIC display.c)
target_include_directories(emerald_display PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(emerald_display PUBLIC SDL2::SDL2)
target_compile_options(emerald_display PRIVATE
    $<$<C_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic -Wconversion -Wshadow>
)

add_executable(emerald-maze maze.c)
target_link_libraries(emerald-maze PRIVATE emerald_core emerald_display)

add_executable(emerald-fractal fractal.c)
target_link_libraries(emerald-fractal PRIVATE emerald_display)

add_executable(emerald-hello-display hellodisplay.c)
target_link_libraries(emerald-hello-display PRIVATE emerald_display)

add_executable(emerald-hello-sdl hellosdl.c)
target_link_libraries(emerald-hello-sdl PRIVATE SDL2::SDL2)

if(BUILD_TESTING)
    add_executable(emerald-core-tests tests/core_tests.c)
    target_link_libraries(emerald-core-tests PRIVATE emerald_core)
    add_test(NAME emerald-core-tests COMMAND emerald-core-tests)
endif()

if(EMERALD_ENABLE_SANITIZERS AND CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    foreach(target emerald_core emerald_display emerald-maze emerald-fractal emerald-hello-display emerald-hello-sdl emerald-core-tests)
        if(TARGET ${target})
            target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE -fsanitize=address,undefined)
        endif()
    endforeach()
endif()

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)
if(CLANG_FORMAT_EXECUTABLE)
    file(GLOB EMERALD_FORMAT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c"
    )
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${EMERALD_FORMAT_SOURCES}
        COMMENT "Formatting C sources"
    )
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror ${EMERALD_FORMAT_SOURCES}
        COMMENT "Checking C source formatting"
    )
endif()
