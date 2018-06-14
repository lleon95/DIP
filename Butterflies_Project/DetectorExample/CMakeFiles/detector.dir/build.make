# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample

# Include any dependencies generated for this target.
include CMakeFiles/detector.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/detector.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/detector.dir/flags.make

CMakeFiles/detector.dir/detector.o: CMakeFiles/detector.dir/flags.make
CMakeFiles/detector.dir/detector.o: detector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lleon95/Git/DIP/Butterflies_Project/DetectorExample/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/detector.dir/detector.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/detector.dir/detector.o -c /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample/detector.cpp

CMakeFiles/detector.dir/detector.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/detector.dir/detector.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample/detector.cpp > CMakeFiles/detector.dir/detector.i

CMakeFiles/detector.dir/detector.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/detector.dir/detector.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample/detector.cpp -o CMakeFiles/detector.dir/detector.s

CMakeFiles/detector.dir/detector.o.requires:

.PHONY : CMakeFiles/detector.dir/detector.o.requires

CMakeFiles/detector.dir/detector.o.provides: CMakeFiles/detector.dir/detector.o.requires
	$(MAKE) -f CMakeFiles/detector.dir/build.make CMakeFiles/detector.dir/detector.o.provides.build
.PHONY : CMakeFiles/detector.dir/detector.o.provides

CMakeFiles/detector.dir/detector.o.provides.build: CMakeFiles/detector.dir/detector.o


# Object files for target detector
detector_OBJECTS = \
"CMakeFiles/detector.dir/detector.o"

# External object files for target detector
detector_EXTERNAL_OBJECTS =

detector: CMakeFiles/detector.dir/detector.o
detector: CMakeFiles/detector.dir/build.make
detector: /usr/local/lib/libopencv_viz.so.2.4.9
detector: /usr/local/lib/libopencv_videostab.so.2.4.9
detector: /usr/local/lib/libopencv_ts.a
detector: /usr/local/lib/libopencv_superres.so.2.4.9
detector: /usr/local/lib/libopencv_stitching.so.2.4.9
detector: /usr/local/lib/libopencv_contrib.so.2.4.9
detector: /usr/lib/x86_64-linux-gnu/libGLU.so
detector: /usr/lib/x86_64-linux-gnu/libGL.so
detector: /usr/local/lib/libopencv_nonfree.so.2.4.9
detector: /usr/local/lib/libopencv_ocl.so.2.4.9
detector: /usr/local/lib/libopencv_gpu.so.2.4.9
detector: /usr/local/lib/libopencv_photo.so.2.4.9
detector: /usr/local/lib/libopencv_objdetect.so.2.4.9
detector: /usr/local/lib/libopencv_legacy.so.2.4.9
detector: /usr/local/lib/libopencv_video.so.2.4.9
detector: /usr/local/lib/libopencv_ml.so.2.4.9
detector: /usr/local/lib/libopencv_calib3d.so.2.4.9
detector: /usr/local/lib/libopencv_features2d.so.2.4.9
detector: /usr/local/lib/libopencv_highgui.so.2.4.9
detector: /usr/local/lib/libopencv_imgproc.so.2.4.9
detector: /usr/local/lib/libopencv_flann.so.2.4.9
detector: /usr/local/lib/libopencv_core.so.2.4.9
detector: CMakeFiles/detector.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lleon95/Git/DIP/Butterflies_Project/DetectorExample/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable detector"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/detector.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/detector.dir/build: detector

.PHONY : CMakeFiles/detector.dir/build

CMakeFiles/detector.dir/requires: CMakeFiles/detector.dir/detector.o.requires

.PHONY : CMakeFiles/detector.dir/requires

CMakeFiles/detector.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/detector.dir/cmake_clean.cmake
.PHONY : CMakeFiles/detector.dir/clean

CMakeFiles/detector.dir/depend:
	cd /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample /home/lleon95/Git/DIP/Butterflies_Project/DetectorExample/CMakeFiles/detector.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/detector.dir/depend

