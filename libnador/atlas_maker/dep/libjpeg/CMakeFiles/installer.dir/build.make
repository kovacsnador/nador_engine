# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\nador_engine\nador_test_app\libnador\atlas_maker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\nador_engine\nador_test_app\libnador\atlas_maker

# Utility rule file for installer.

# Include any custom commands dependencies for this target.
include dep/libjpeg/CMakeFiles/installer.dir/compiler_depend.make

# Include the progress variables for this target.
include dep/libjpeg/CMakeFiles/installer.dir/progress.make

dep/libjpeg/CMakeFiles/installer: dep/libjpeg/jpeg
dep/libjpeg/CMakeFiles/installer: out/Debug/lib/libjpeg.a
dep/libjpeg/CMakeFiles/installer: out/Debug/bin/rdjpgcom.exe
dep/libjpeg/CMakeFiles/installer: out/Debug/bin/wrjpgcom.exe
dep/libjpeg/CMakeFiles/installer: dep/libjpeg/cjpeg.c
dep/libjpeg/CMakeFiles/installer: dep/libjpeg/djpeg.c
dep/libjpeg/CMakeFiles/installer: dep/libjpeg/jpegtran.c
dep/libjpeg/CMakeFiles/installer: dep/libjpeg/turbojpeg.c
dep/libjpeg/CMakeFiles/installer: out/Debug/lib/libturbojpeg.a
dep/libjpeg/CMakeFiles/installer: dep/libjpeg/tjbench.c
	cd /d E:\nador_engine\nador_test_app\libnador\atlas_maker\dep\libjpeg && makensis -nocd -DGCC -DWIN64 -DBUILDDIR= installer.nsi

installer: dep/libjpeg/CMakeFiles/installer
installer: dep/libjpeg/CMakeFiles/installer.dir/build.make
.PHONY : installer

# Rule to build all files generated by this target.
dep/libjpeg/CMakeFiles/installer.dir/build: installer
.PHONY : dep/libjpeg/CMakeFiles/installer.dir/build

dep/libjpeg/CMakeFiles/installer.dir/clean:
	cd /d E:\nador_engine\nador_test_app\libnador\atlas_maker\dep\libjpeg && $(CMAKE_COMMAND) -P CMakeFiles\installer.dir\cmake_clean.cmake
.PHONY : dep/libjpeg/CMakeFiles/installer.dir/clean

dep/libjpeg/CMakeFiles/installer.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\nador_engine\nador_test_app\libnador\atlas_maker E:\nador_engine\nador_test_app\libnador\atlas_maker\dep\libjpeg E:\nador_engine\nador_test_app\libnador\atlas_maker E:\nador_engine\nador_test_app\libnador\atlas_maker\dep\libjpeg E:\nador_engine\nador_test_app\libnador\atlas_maker\dep\libjpeg\CMakeFiles\installer.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : dep/libjpeg/CMakeFiles/installer.dir/depend

