cmake_minimum_required(VERSION 3.20)

project(physics CXX)
file(GLOB _PHYSICS_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**/*.cc)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

message(${_PHYSICS_SOURCES})
add_library(physics ${_PHYSICS_SOURCES})

find_package(OpenBlas CONFIG)
find_package(OpenMP REQUIRED)

target_include_directories(physics PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(physics PRIVATE openblas)

# C++20 の有効化
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS}")

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    find_package(GTest)

    enable_testing()
    add_executable(
        physics_test
        test/Matrix_test.cc
        test/Vector_test.cc
        test/lapacke_test.cc
        test/openmp_test.cc
        test/Fraction_test.cc
        test/Quantity_test.cc
        )
    target_link_libraries(
        physics_test
        GTest::gtest_main
        OpenMP::OpenMP_CXX
        physics
        )

    include(GoogleTest)
    gtest_discover_tests(physics_test)
endif()
