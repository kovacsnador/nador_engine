# https://decovar.dev/blog/2021/03/08/cmake-cpp-library/#the-library

cmake_minimum_required(VERSION 3.25)

project("nador_test_app" VERSION 0.1 DESCRIPTION "The nador engine test program.")

if(NOT CMAKE_BUILD_TYPE)   
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type options: Debug; Release; RelWithDebInfo; MinSizeRel" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD                  20)

set(CMAKE_CXX_FLAGS                     "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG               "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE             "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO      "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS_MINSIZEREL          "${CMAKE_CXX_FLAGS} -O2") 

set(OUTPUT_DIR                          ${CMAKE_CURRENT_SOURCE_DIR}/out)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY      ${OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY      ${OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY      ${OUTPUT_DIR}/${CMAKE_BUILD_TYPE}/bin)

set(NADOR_EIMAGE_NAME_PATH              ${CMAKE_CURRENT_SOURCE_DIR}/res/atlas/__atlases)

set(NADOR_WINDOW_API                "SDL2" CACHE STRING "" FORCE)

add_subdirectory(libnador)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libnador
)

target_sources(${PROJECT_NAME}
    PRIVATE
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE nador)