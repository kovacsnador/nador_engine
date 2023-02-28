# Install script for directory: E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "c:/atlas_maker-gcc64")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/MinGW/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/out/Debug/lib/libturbojpeg.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM RENAME "tjbench.exe" FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/tjbench-static.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/turbojpeg.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/out/Debug/lib/libjpeg.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM RENAME "cjpeg.exe" FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/cjpeg-static.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM RENAME "djpeg.exe" FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/djpeg-static.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM RENAME "jpegtran.exe" FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/jpegtran-static.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/out/Debug/bin/rdjpgcom.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/rdjpgcom.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/rdjpgcom.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/MinGW/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/rdjpgcom.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/out/Debug/bin/wrjpgcom.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/wrjpgcom.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/wrjpgcom.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/MinGW/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/wrjpgcom.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "E:/nador_engine/nador_test_app/libnador/atlas_maker/doc/README.ijg;E:/nador_engine/nador_test_app/libnador/atlas_maker/doc/README.md;E:/nador_engine/nador_test_app/libnador/atlas_maker/doc/example.c;E:/nador_engine/nador_test_app/libnador/atlas_maker/doc/tjexample.c;E:/nador_engine/nador_test_app/libnador/atlas_maker/doc/libjpeg.txt;E:/nador_engine/nador_test_app/libnador/atlas_maker/doc/structure.txt;E:/nador_engine/nador_test_app/libnador/atlas_maker/doc/usage.txt;E:/nador_engine/nador_test_app/libnador/atlas_maker/doc/wizard.txt;E:/nador_engine/nador_test_app/libnador/atlas_maker/doc/LICENSE.md")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "E:/nador_engine/nador_test_app/libnador/atlas_maker/doc" TYPE FILE FILES
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/README.ijg"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/README.md"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/example.c"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/tjexample.c"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/libjpeg.txt"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/structure.txt"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/usage.txt"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/wizard.txt"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/LICENSE.md"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/man1/cjpeg.1;/man1/djpeg.1;/man1/jpegtran.1;/man1/rdjpgcom.1;/man1/wrjpgcom.1")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/man1" TYPE FILE FILES
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/cjpeg.1"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/djpeg.1"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/jpegtran.1"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/rdjpgcom.1"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/wrjpgcom.1"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/pkgscripts/libjpeg.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/pkgscripts/libturbojpeg.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/atlas_maker" TYPE FILE FILES
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/pkgscripts/atlas_makerConfig.cmake"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/pkgscripts/atlas_makerConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/atlas_maker/atlas_makerTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/atlas_maker/atlas_makerTargets.cmake"
         "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/CMakeFiles/Export/d21c9cbc98b210fc0e647a2198a0526d/atlas_makerTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/atlas_maker/atlas_makerTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/atlas_maker/atlas_makerTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/atlas_maker" TYPE FILE FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/CMakeFiles/Export/d21c9cbc98b210fc0e647a2198a0526d/atlas_makerTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/atlas_maker" TYPE FILE FILES "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/CMakeFiles/Export/d21c9cbc98b210fc0e647a2198a0526d/atlas_makerTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/jconfig.h"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/jerror.h"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/jmorecfg.h"
    "E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/jpeglib.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/simd/cmake_install.cmake")
  include("E:/nador_engine/nador_test_app/libnador/atlas_maker/dep/libjpeg/md5/cmake_install.cmake")

endif()

