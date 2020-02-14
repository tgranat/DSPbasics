cmake_minimum_required (VERSION 3.8)

project ("DSPbasics")

message (${CMAKE_CURRENT_BINARY_DIR})
# Add source to this project's executable.
add_executable (dspbasics "dspbasics.cpp" "dspbasics.h")

# Specify the default startup project in a Visual Studio solution.
set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT "dspbasics")

# Add libraries to linker

# sndfile
# Install Linux: 
#   sudo apt-get install libsndfile-dev
# Install Windows:
#   download Windows intaller from: http://www.mega-nerd.com/libsndfile/#Download and install
#   add <where-it-is-installed>/libsndfile/bin to PATH
#
# Change paths below if necessary (I'm a cmake newbie so this is how I have done it)
add_library(sndfile SHARED IMPORTED)
if (WIN32)
  set_target_properties(sndfile PROPERTIES
    IMPORTED_LOCATION "C:/Program Files/Mega-Nerd/libsndfile/bin/libsndfile-1.dll"
	IMPORTED_IMPLIB "C:/Program Files/Mega-Nerd/libsndfile/lib/libsndfile-1.lib"
    INTERFACE_INCLUDE_DIRECTORIES "C:/Program Files/Mega-Nerd/libsndfile/include")
else ()
  set_target_properties(sndfile PROPERTIES
    IMPORTED_LOCATION "/usr/lib/x86_64-linux-gnu/libsndfile.so"
    INTERFACE_INCLUDE_DIRECTORIES "/usr/include")
endif ()

target_link_libraries(dspbasics sndfile)

