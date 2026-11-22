cmake_minimum_required(VERSION 3.24)
project(AtmosphereEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ATMOSPHERE_BUILD_TESTS "Build tests and benchmarks" ON)
option(ATMOSPHERE_ENABLE_SANITIZERS "Enable ASan + UBSan" OFF)

if(ATMOSPHERE_ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()

add_subdirectory(src/core)
add_subdirectory(src/app)

if(ATMOSPHERE_BUILD_TESTS)
    enable_testing()
    # add_subdirectory(tests)
    # add_subdirectory(bench)
endif()