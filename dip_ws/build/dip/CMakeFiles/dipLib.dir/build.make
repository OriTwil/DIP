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
CMAKE_SOURCE_DIR = /home/szf/DIP/dip_ws/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/szf/DIP/dip_ws/build

# Include any dependencies generated for this target.
include dip/CMakeFiles/dipLib.dir/depend.make

# Include the progress variables for this target.
include dip/CMakeFiles/dipLib.dir/progress.make

# Include the compile flags for this target's objects.
include dip/CMakeFiles/dipLib.dir/flags.make

dip/CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.o: dip/CMakeFiles/dipLib.dir/flags.make
dip/CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.o: /home/szf/DIP/dip_ws/src/dip/include/dipLib/dipLib.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/szf/DIP/dip_ws/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object dip/CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.o"
	cd /home/szf/DIP/dip_ws/build/dip && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.o -c /home/szf/DIP/dip_ws/src/dip/include/dipLib/dipLib.cpp

dip/CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.i"
	cd /home/szf/DIP/dip_ws/build/dip && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/szf/DIP/dip_ws/src/dip/include/dipLib/dipLib.cpp > CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.i

dip/CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.s"
	cd /home/szf/DIP/dip_ws/build/dip && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/szf/DIP/dip_ws/src/dip/include/dipLib/dipLib.cpp -o CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.s

# Object files for target dipLib
dipLib_OBJECTS = \
"CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.o"

# External object files for target dipLib
dipLib_EXTERNAL_OBJECTS =

/home/szf/DIP/dip_ws/devel/lib/libdipLib.so: dip/CMakeFiles/dipLib.dir/include/dipLib/dipLib.cpp.o
/home/szf/DIP/dip_ws/devel/lib/libdipLib.so: dip/CMakeFiles/dipLib.dir/build.make
/home/szf/DIP/dip_ws/devel/lib/libdipLib.so: dip/CMakeFiles/dipLib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/szf/DIP/dip_ws/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library /home/szf/DIP/dip_ws/devel/lib/libdipLib.so"
	cd /home/szf/DIP/dip_ws/build/dip && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dipLib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
dip/CMakeFiles/dipLib.dir/build: /home/szf/DIP/dip_ws/devel/lib/libdipLib.so

.PHONY : dip/CMakeFiles/dipLib.dir/build

dip/CMakeFiles/dipLib.dir/clean:
	cd /home/szf/DIP/dip_ws/build/dip && $(CMAKE_COMMAND) -P CMakeFiles/dipLib.dir/cmake_clean.cmake
.PHONY : dip/CMakeFiles/dipLib.dir/clean

dip/CMakeFiles/dipLib.dir/depend:
	cd /home/szf/DIP/dip_ws/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/szf/DIP/dip_ws/src /home/szf/DIP/dip_ws/src/dip /home/szf/DIP/dip_ws/build /home/szf/DIP/dip_ws/build/dip /home/szf/DIP/dip_ws/build/dip/CMakeFiles/dipLib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : dip/CMakeFiles/dipLib.dir/depend

