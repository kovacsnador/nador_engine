cmake_minimum_required(VERSION 3.14)

project(nador_unit_tests) #name of your project

if(NOT CMAKE_BUILD_TYPE)   
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type options: Debug; Release; RelWithDebInfo; MinSizeRel" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif(NOT CMAKE_BUILD_TYPE)

add_subdirectory(dep/googletest-main) # add googletest subdirectory

include_directories(dep/googletest-main/googletest/include) # this is so we can #include <gtest/gtest.h>
include_directories(dep/googletest-main/googlemock/include) # this is so we can #include <gmock>

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
    main.cpp
    hello_test.cc
    nador_app_test.cpp
)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE gtest
                      PRIVATE gmock) # link google test to this executable