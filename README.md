# CiA402Device
Library under CiA 402 standard for device control

# How to use

This library is intended for the use with CMake build system. Under CMake building, it automatically holds all the include and link directories in variables:

``
SUBDIR_INCLUDE_DIRECTORIES
SUBDIR_LINK_NAMES
``

Then, assuming the library is placed at "${PROJECT_SOURCE_DIR}/lib/CiA402Device/", it is enough to add the following lines to CMakeLists.txt to add includes:

``
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/CiA402Device/)
INCLUDE_DIRECTORIES(${SUBDIR_INCLUDE_DIRECTORIES})
``

Also after "add_executable( ${name} ${sourcefile} )" line, add the following to link the library:

``
target_link_libraries( ${name} ${SUBDIR_LINK_NAMES} )
``


![Main class collaboration graph](docs/classCiA402Device__coll__graph.png "Main class collaboration graph")
