# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/163/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/163/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tardis3/InfLessons_3_sem/10_09_21Less

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tardis3/InfLessons_3_sem/10_09_21Less/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/10_09_21Less.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/10_09_21Less.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/10_09_21Less.dir/flags.make

CMakeFiles/10_09_21Less.dir/main.c.o: CMakeFiles/10_09_21Less.dir/flags.make
CMakeFiles/10_09_21Less.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tardis3/InfLessons_3_sem/10_09_21Less/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/10_09_21Less.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/10_09_21Less.dir/main.c.o -c /home/tardis3/InfLessons_3_sem/10_09_21Less/main.c

CMakeFiles/10_09_21Less.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/10_09_21Less.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tardis3/InfLessons_3_sem/10_09_21Less/main.c > CMakeFiles/10_09_21Less.dir/main.c.i

CMakeFiles/10_09_21Less.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/10_09_21Less.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tardis3/InfLessons_3_sem/10_09_21Less/main.c -o CMakeFiles/10_09_21Less.dir/main.c.s

# Object files for target 10_09_21Less
10_09_21Less_OBJECTS = \
"CMakeFiles/10_09_21Less.dir/main.c.o"

# External object files for target 10_09_21Less
10_09_21Less_EXTERNAL_OBJECTS =

10_09_21Less: CMakeFiles/10_09_21Less.dir/main.c.o
10_09_21Less: CMakeFiles/10_09_21Less.dir/build.make
10_09_21Less: CMakeFiles/10_09_21Less.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tardis3/InfLessons_3_sem/10_09_21Less/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable 10_09_21Less"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/10_09_21Less.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/10_09_21Less.dir/build: 10_09_21Less
.PHONY : CMakeFiles/10_09_21Less.dir/build

CMakeFiles/10_09_21Less.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/10_09_21Less.dir/cmake_clean.cmake
.PHONY : CMakeFiles/10_09_21Less.dir/clean

CMakeFiles/10_09_21Less.dir/depend:
	cd /home/tardis3/InfLessons_3_sem/10_09_21Less/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tardis3/InfLessons_3_sem/10_09_21Less /home/tardis3/InfLessons_3_sem/10_09_21Less /home/tardis3/InfLessons_3_sem/10_09_21Less/cmake-build-debug /home/tardis3/InfLessons_3_sem/10_09_21Less/cmake-build-debug /home/tardis3/InfLessons_3_sem/10_09_21Less/cmake-build-debug/CMakeFiles/10_09_21Less.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/10_09_21Less.dir/depend

