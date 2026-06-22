cmake_minimum_required(VERSION 3.24)

project(Marvin VERSION 2.0.0 LANGUAGES CXX)

include(CTest)
include(FetchContent)

option(MARVIN_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(MARVIN_ENABLE_CLANG_TIDY "Run clang-tidy while compiling Marvin targets" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

if(MARVIN_ENABLE_ASAN AND MSVC)
    add_compile_options(/fsanitize=address /Zi)
    add_link_options(/INCREMENTAL:NO)
endif()

if(MARVIN_ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy REQUIRED)
endif()

function(marvin_enable_clang_tidy target)
    if(MARVIN_ENABLE_CLANG_TIDY)
        set_target_properties(${target} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")
    endif()
endfunction()

function(marvin_enable_strict_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX /permissive- /sdl)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

function(marvin_enable_asan target)
    if(NOT MARVIN_ENABLE_ASAN)
        return()
    endif()

    if(NOT MSVC)
        target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address)
    endif()
endfunction()

add_library(marvin_core STATIC
    Command.cpp
    Command.h
    Marvin.cpp
    Marvin.h
    Menu.cpp
    Menu.h
    Robot.cpp
    Robot.h
    RobotAssembly.h
    RobotGrid.cpp
    RobotGrid.h
    RobotSimulator.cpp
    RobotSimulator.h
)
target_include_directories(marvin_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
marvin_enable_strict_warnings(marvin_core)
marvin_enable_asan(marvin_core)
marvin_enable_clang_tidy(marvin_core)

add_executable(Marvin main.cpp)
target_link_libraries(Marvin PRIVATE marvin_core)
marvin_enable_strict_warnings(Marvin)
marvin_enable_asan(Marvin)
marvin_enable_clang_tidy(Marvin)

if(BUILD_TESTING)
    set(gtest_force_shared_crt ON CACHE BOOL "Use the shared MSVC runtime" FORCE)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.tar.gz
        URL_HASH SHA256=745c55415660044610f7fcd3af7a6420d5de16a7dbb9ebfe2e131275676232be
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(RobotSimulatorTest
        RobotSimulatorTest/TestCommandParser.cpp
        RobotSimulatorTest/TestRobotGrid.cpp
        RobotSimulatorTest/TestRobotMarvin.cpp
        RobotSimulatorTest/TestRobotSimulator.cpp
    )
    target_link_libraries(RobotSimulatorTest PRIVATE marvin_core GTest::gtest_main)
    marvin_enable_strict_warnings(RobotSimulatorTest)
    marvin_enable_asan(RobotSimulatorTest)
    marvin_enable_clang_tidy(RobotSimulatorTest)

    include(GoogleTest)
    gtest_discover_tests(RobotSimulatorTest)
endif()
