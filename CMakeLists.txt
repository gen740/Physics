cmake_minimum_required(VERSION 3.20)

project(physics CXX)
file(GLOB _PHYSICS_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**/*.cc)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

message(${_PHYSICS_SOURCES})
add_library(physics ${_PHYSICS_SOURCES})

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

find_package(OpenMP REQUIRED)

target_include_directories(physics PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
set(USE_MKL true)

if(${USE_MKL})
    # Find MKL
    if(NOT $ENV{MKL_DIR} STREQUAL "")
        find_package(MKL REQUIRED)
        target_include_directories(physics PUBLIC ${MKL_INCLUDE})
        target_link_libraries(physics PRIVATE MKL::MKL)
    else()
        find_package(PkgConfig)
        pkg_check_modules(OpenBLAS REQUIRED openblas)
        target_include_directories(physics PUBLIC ${MKL_INCLUDE_DIRS})
        target_link_libraries(physics PRIVATE ${MKL_LINK_LIBRARIES})
    endif()
    add_compile_definitions(PHYSICS_USE_MKL)
else()
    # Find OpenBLAS
    if(NOT $ENV{OpenBLAS_DIR} STREQUAL "")
        find_package(OpenBLAS REQUIRED)
        target_include_directories(physics PUBLIC ${OpenBLAS_INCLUDE_DIRS})
        target_link_libraries(physics PRIVATE ${OpenBLAS_LIBRARIES})
        if(NOT APPLE)
            find_package(PkgConfig)
            pkg_check_modules(Lapacke REQUIRED lapacke)
            target_include_directories(physics PUBLIC ${Lapacke_INCLUDE_DIRS})
            target_link_libraries(physics PRIVATE ${Lapacke_LINK_LIBRARIES})
        endif()
    else()
        find_package(PkgConfig)
        pkg_check_modules(OpenBLAS REQUIRED openblas)
        target_include_directories(physics PUBLIC ${OpenBLAS_INCLUDE_DIRS})
        target_link_libraries(physics PRIVATE ${OpenBLAS_LINK_LIBRARIES})
        if(NOT APPLE)
            pkg_check_modules(Lapacke REQUIRED lapacke)
            target_include_directories(physics PUBLIC ${Lapacke_INCLUDE_DIRS})
            target_link_libraries(physics PRIVATE ${Lapacke_LINK_LIBRARIES})
        endif()
    endif()
endif()

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
        test/generator_test.cc
        )
    target_link_libraries(
        physics_test
        GTest::gtest_main
        OpenMP::OpenMP_CXX
        physics
        )

    include(GoogleTest)
    gtest_discover_tests(physics_test)

    ## examples
    add_custom_target(examples)

    add_executable(quantity EXCLUDE_FROM_ALL examples/quantity.cc)
    target_link_libraries(quantity physics)

    add_executable(matrix EXCLUDE_FROM_ALL examples/matrix.cc)
    target_link_libraries(matrix physics)
    add_dependencies(examples quantity matrix)
endif()
