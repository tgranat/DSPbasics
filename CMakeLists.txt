cmake_minimum_required (VERSION 3.8)

project ("DSPbasics")

# You may need to give the location of the installed sndfile lib with
# cmake -B build -DSNDFILE_DIR=<location>

if ( NOT SNDFILE_DIR )
  set(Copy_Plugin "TRUE")
  if (WIN32)
    set(SNDFILE_DIR "C:/Program Files/Mega-Nerd/libsndfile")
  else()
    set(SNDFILE_DIR "/usr/lib/x86_64-linux-gnu")
  endif()
endif()

message("SNDFILE_DIR=${SNDFILE_DIR}")

# Add source to this project's executable.
add_executable (dspbasics 
    "dspbasics.cpp"
    "dspbasics.h"
    "Biquad.cpp"
    "Biquad.h")

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
# If you have time, you can implement a 'find' module for sndfile, and then use find_package
# instead. I had problems getting it to work on Windows so I gave up.

add_library(sndfile SHARED IMPORTED)
if (WIN32)
  set_target_properties(sndfile PROPERTIES
    IMPORTED_LOCATION "${SNDFILE_DIR}/bin/libsndfile-1.dll"
	IMPORTED_IMPLIB "${SNDFILE_DIR}/lib/libsndfile-1.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SNDFILE_DIR}/include")
else ()
  set_target_properties(sndfile PROPERTIES
    IMPORTED_LOCATION "${SNDFILE_DIR}/libsndfile.so"
    INTERFACE_INCLUDE_DIRECTORIES "/usr/include")
endif ()

target_link_libraries(dspbasics sndfile)

