cmake_minimum_required(VERSION 3.6)
project(multiplotter)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile @ONLY)
    add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
            SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
endif(DOXYGEN_FOUND)

set(Python_ADDITIONAL_VERSIONS 3.5)

find_package(PythonInterp 2 REQUIRED)
find_package(PythonLibs 2 REQUIRED)
add_definitions(-Dpython="${PYTHON_EXECUTABLE}")

include_directories(${PYTHON_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_FILES src/Plotter.cpp include/Plotter.hpp)
add_library(multiplotter SHARED ${SOURCE_FILES})
target_link_libraries(multiplotter ${PYTHON_LIBRARY})

add_executable(testing main.cpp)
target_link_libraries(testing multiplotter)
