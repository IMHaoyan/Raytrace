# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/haoyan/Raytrace

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/haoyan/Raytrace/build

# Include any dependencies generated for this target.
include CMakeFiles/Raytrace.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Raytrace.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Raytrace.dir/flags.make

CMakeFiles/Raytrace.dir/main.cpp.o: CMakeFiles/Raytrace.dir/flags.make
CMakeFiles/Raytrace.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/haoyan/Raytrace/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Raytrace.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Raytrace.dir/main.cpp.o -c /home/haoyan/Raytrace/main.cpp

CMakeFiles/Raytrace.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Raytrace.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/haoyan/Raytrace/main.cpp > CMakeFiles/Raytrace.dir/main.cpp.i

CMakeFiles/Raytrace.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Raytrace.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/haoyan/Raytrace/main.cpp -o CMakeFiles/Raytrace.dir/main.cpp.s

# Object files for target Raytrace
Raytrace_OBJECTS = \
"CMakeFiles/Raytrace.dir/main.cpp.o"

# External object files for target Raytrace
Raytrace_EXTERNAL_OBJECTS =

Raytrace: CMakeFiles/Raytrace.dir/main.cpp.o
Raytrace: CMakeFiles/Raytrace.dir/build.make
Raytrace: CMakeFiles/Raytrace.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/haoyan/Raytrace/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Raytrace"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Raytrace.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Raytrace.dir/build: Raytrace

.PHONY : CMakeFiles/Raytrace.dir/build

CMakeFiles/Raytrace.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Raytrace.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Raytrace.dir/clean

CMakeFiles/Raytrace.dir/depend:
	cd /home/haoyan/Raytrace/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/haoyan/Raytrace /home/haoyan/Raytrace /home/haoyan/Raytrace/build /home/haoyan/Raytrace/build /home/haoyan/Raytrace/build/CMakeFiles/Raytrace.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Raytrace.dir/depend

