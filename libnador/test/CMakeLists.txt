cmake_minimum_required(VERSION 3.14)

project(nador_unit_tests) #name of your project

if(NOT CMAKE_BUILD_TYPE)   
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type options: Debug; Release; RelWithDebInfo; MinSizeRel" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif(NOT CMAKE_BUILD_TYPE)

add_subdirectory(dep/googletest-main) # add googletest subdirectory

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/googletest-main/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/dep/googletest-main/googlemock/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_sources(${PROJECT_NAME}
    PRIVATE
    main.cpp
    nador_app_test.cpp
)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE gtest
                      PRIVATE gmock
                      PRIVATE nador)