cmake_minimum_required(VERSION 3.6)
project(multiplotter)

set(Python_ADDITIONAL_VERSIONS 3.5)

find_package(PythonInterp 2 REQUIRED)
find_package(PythonLibs 2 REQUIRED)
add_definitions(-Dpython="${PYTHON_EXECUTABLE}")

include_directories(${PYTHON_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_FILES main.cpp src/Plotter.cpp include/Plotter.hpp)
add_executable(multiplotter ${SOURCE_FILES})
target_link_libraries(multiplotter ${PYTHON_LIBRARY})