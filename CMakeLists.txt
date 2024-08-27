cmake_minimum_required(VERSION 3.23)
project(magicRuben)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenCV path - can be overridden from command line
if(NOT DEFINED OpenCV_DIR)
    set(OpenCV_DIR "C:/opencv/opencv/build")
endif()

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Find Python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Set pybind11 path and version - can be overridden from command line
if(NOT DEFINED PYBIND11_PYTHON_VERSION)
    set(PYBIND11_PYTHON_VERSION 3.10)
endif()

if(NOT DEFINED pybind11_DIR)
    set(pybind11_DIR "C:/Users/Jared/AppData/Local/Programs/Python/Python310/Lib/site-packages/pybind11/share/cmake/pybind11")
endif()

# Find pybind11
find_package(pybind11 CONFIG REQUIRED)

# Rest of your CMakeLists.txt remains the same
set(SOURCE_FILES
        main.cpp
        gesture_wrapper.cpp
        common.cpp
        cube.cpp
        pyramid.cpp
        donut.cpp
)

set(HEADER_FILES
        common.h
        cube.h
        pyramid.h
        donut.h
        gesture_wrapper.h
)

add_executable(magicRuben ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(magicRuben PRIVATE
        ${OpenCV_LIBS}
        pybind11::embed
        Python3::Python
)

target_include_directories(magicRuben PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${OpenCV_INCLUDE_DIRS}
        ${PYBIND11_INCLUDE_DIRS}
        ${Python3_INCLUDE_DIRS}
)

message(STATUS "OpenCV_FOUND: ${OpenCV_FOUND}")
message(STATUS "OpenCV_VERSION: ${OpenCV_VERSION}")
message(STATUS "Python3_FOUND: ${Python3_FOUND}")
message(STATUS "Python3_VERSION: ${Python3_VERSION}")
message(STATUS "pybind11_FOUND: ${pybind11_FOUND}")
message(STATUS "pybind11_VERSION: ${pybind11_VERSION}")
