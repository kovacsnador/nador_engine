cmake_minimum_required(VERSION 3.25)

project("atlas_maker" VERSION 0.1 DESCRIPTION "Atlas generator program.")

if(NOT CMAKE_BUILD_TYPE)   
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type options: Debug; Release; RelWithDebInfo; MinSizeRel" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif(NOT CMAKE_BUILD_TYPE)

# set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS                     "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG               "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE             "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO      "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_MINSIZEREL          "${CMAKE_CXX_FLAGS} -O3")

add_definitions(
    -Dcimg_use_png
    -Dcimg_use_jpeg
    -Dcimg_use_zlib
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out/${CMAKE_BUILD_TYPE}/bin)

set(ENABLE_SHARED OFF CACHE BOOL "")
set(PNG_SHARED OFF CACHE BOOL "" FORCE)
set(PNG_STATIC ON CACHE BOOL "" FORCE)
set(PNG_EXECUTABLES OFF CACHE BOOL "" FORCE) # we only want lib
set(PNG_TESTS OFF CACHE BOOL "" FORCE) # we only want lib
set(SKIP_INSTALL_ALL OFF CACHE BOOL "" FORCE) # we only want lib

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# add zlib
add_subdirectory(dep/zlib)
set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dep/zlib")
set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})

# add libpng
# This is the only way I could avoid libpng going crazy if it found awk.exe, seems benign otherwise
set(PNG_BUILD_ZLIB ${ZLIB_INCLUDE_DIR} CACHE STRING "" FORCE)
add_subdirectory(dep/libpng)
set(PNG_PNG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dep/libpng")

# add jpeg
set(CMAKE_INSTALL_DOCDIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
add_subdirectory(dep/libjpeg)

# Add executable
add_executable(${PROJECT_NAME})

# Add sources
target_sources(${PROJECT_NAME}
    PRIVATE
    Main.cpp
    AtlasImage.cpp
    FileUtil.cpp
    Image.cpp
    ImageUtils.cpp
    SimpleAtlasGenerator.cpp
    SimpleImage.cpp
    Test.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/dep/cimg
        ${CMAKE_CURRENT_SOURCE_DIR}/dep/libjpeg
        ${CMAKE_CURRENT_SOURCE_DIR}/dep/libpng
        ${CMAKE_CURRENT_SOURCE_DIR}/dep/rapidJson
        ${CMAKE_CURRENT_SOURCE_DIR}/dep/zlib
)

target_link_libraries(${PROJECT_NAME} png_static)
target_link_libraries(${PROJECT_NAME} jpeg-static)
target_link_libraries(${PROJECT_NAME} zlibstatic)