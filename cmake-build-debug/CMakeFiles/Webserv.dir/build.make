# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /snap/clion/137/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/137/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rinne/School42/5lvl/Webserv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rinne/School42/5lvl/Webserv/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Webserv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Webserv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Webserv.dir/flags.make

CMakeFiles/Webserv.dir/Server.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/Server.cpp.o: ../Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rinne/School42/5lvl/Webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Webserv.dir/Server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Webserv.dir/Server.cpp.o -c /home/rinne/School42/5lvl/Webserv/Server.cpp

CMakeFiles/Webserv.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/Server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rinne/School42/5lvl/Webserv/Server.cpp > CMakeFiles/Webserv.dir/Server.cpp.i

CMakeFiles/Webserv.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/Server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rinne/School42/5lvl/Webserv/Server.cpp -o CMakeFiles/Webserv.dir/Server.cpp.s

CMakeFiles/Webserv.dir/main.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rinne/School42/5lvl/Webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Webserv.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Webserv.dir/main.cpp.o -c /home/rinne/School42/5lvl/Webserv/main.cpp

CMakeFiles/Webserv.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rinne/School42/5lvl/Webserv/main.cpp > CMakeFiles/Webserv.dir/main.cpp.i

CMakeFiles/Webserv.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rinne/School42/5lvl/Webserv/main.cpp -o CMakeFiles/Webserv.dir/main.cpp.s

CMakeFiles/Webserv.dir/get_page_text.cpp.o: CMakeFiles/Webserv.dir/flags.make
CMakeFiles/Webserv.dir/get_page_text.cpp.o: ../get_page_text.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rinne/School42/5lvl/Webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Webserv.dir/get_page_text.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Webserv.dir/get_page_text.cpp.o -c /home/rinne/School42/5lvl/Webserv/get_page_text.cpp

CMakeFiles/Webserv.dir/get_page_text.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Webserv.dir/get_page_text.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rinne/School42/5lvl/Webserv/get_page_text.cpp > CMakeFiles/Webserv.dir/get_page_text.cpp.i

CMakeFiles/Webserv.dir/get_page_text.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Webserv.dir/get_page_text.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rinne/School42/5lvl/Webserv/get_page_text.cpp -o CMakeFiles/Webserv.dir/get_page_text.cpp.s

# Object files for target Webserv
Webserv_OBJECTS = \
"CMakeFiles/Webserv.dir/Server.cpp.o" \
"CMakeFiles/Webserv.dir/main.cpp.o" \
"CMakeFiles/Webserv.dir/get_page_text.cpp.o"

# External object files for target Webserv
Webserv_EXTERNAL_OBJECTS =

Webserv: CMakeFiles/Webserv.dir/Server.cpp.o
Webserv: CMakeFiles/Webserv.dir/main.cpp.o
Webserv: CMakeFiles/Webserv.dir/get_page_text.cpp.o
Webserv: CMakeFiles/Webserv.dir/build.make
Webserv: CMakeFiles/Webserv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rinne/School42/5lvl/Webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Webserv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Webserv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Webserv.dir/build: Webserv

.PHONY : CMakeFiles/Webserv.dir/build

CMakeFiles/Webserv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Webserv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Webserv.dir/clean

CMakeFiles/Webserv.dir/depend:
	cd /home/rinne/School42/5lvl/Webserv/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rinne/School42/5lvl/Webserv /home/rinne/School42/5lvl/Webserv /home/rinne/School42/5lvl/Webserv/cmake-build-debug /home/rinne/School42/5lvl/Webserv/cmake-build-debug /home/rinne/School42/5lvl/Webserv/cmake-build-debug/CMakeFiles/Webserv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Webserv.dir/depend

