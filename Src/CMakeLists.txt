cmake_minimum_required(VERSION 3.5)
project(TestEfficiency)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_BUILD_TYPE Debug)

## Specify additional locations of header files
## Your package locations should be listed before other locations

SET( PROJECT_INCLUDES "../include" )

include_directories(
  ${PROJECT_INCLUDES}
)

# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)

# Must find individual components.
find_package(Boost REQUIRED COMPONENTS system)

message(STATUS  "Boost_FOUND: ${Boost_FOUND}")
message(STATUS  "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS  "Boost_LIBRARIES: ${Boost_LIBRARIES}")

# >>>
# Search the folder for .cpp files.

FILE(GLOB_RECURSE SRC_LIST *.cpp)

IF (SRC_LIST)
	FOREACH (SFILE ${SRC_LIST})
		MESSAGE(STATUS "CPP: ${SFILE}")
	ENDFOREACH (SFILE)
ENDIF (SRC_LIST) 

# Declare a C++ executable
# With catkin_make all packages are built within a single CMake context
# The recommended prefix ensures that target names across packages don't collide
add_executable(exe_${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(exe_${PROJECT_NAME} ${Boost_LIBRARIES})
