# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/valentin/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/182.4892.24/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/valentin/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/182.4892.24/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/valentin/Documents/Projets/Pan/Pan_UV

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/valentin/Documents/Projets/Pan/Pan_UV/cmake-build-debug

# Utility rule file for PLATFORMIO_PROGRAM.

# Include the progress variables for this target.
include CMakeFiles/PLATFORMIO_PROGRAM.dir/progress.make

CMakeFiles/PLATFORMIO_PROGRAM:
	cd /home/valentin/Documents/Projets/Pan/Pan_UV && /usr/bin/platformio -f -c clion run --target program

PLATFORMIO_PROGRAM: CMakeFiles/PLATFORMIO_PROGRAM
PLATFORMIO_PROGRAM: CMakeFiles/PLATFORMIO_PROGRAM.dir/build.make

.PHONY : PLATFORMIO_PROGRAM

# Rule to build all files generated by this target.
CMakeFiles/PLATFORMIO_PROGRAM.dir/build: PLATFORMIO_PROGRAM

.PHONY : CMakeFiles/PLATFORMIO_PROGRAM.dir/build

CMakeFiles/PLATFORMIO_PROGRAM.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PLATFORMIO_PROGRAM.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PLATFORMIO_PROGRAM.dir/clean

CMakeFiles/PLATFORMIO_PROGRAM.dir/depend:
	cd /home/valentin/Documents/Projets/Pan/Pan_UV/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/valentin/Documents/Projets/Pan/Pan_UV /home/valentin/Documents/Projets/Pan/Pan_UV /home/valentin/Documents/Projets/Pan/Pan_UV/cmake-build-debug /home/valentin/Documents/Projets/Pan/Pan_UV/cmake-build-debug /home/valentin/Documents/Projets/Pan/Pan_UV/cmake-build-debug/CMakeFiles/PLATFORMIO_PROGRAM.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PLATFORMIO_PROGRAM.dir/depend

