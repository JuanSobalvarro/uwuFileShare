cmake_minimum_required(VERSION 3.20)
project(UwU_FileShare)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find Pybind11
find_package(pybind11 REQUIRED)

# Add core network library
add_library(network STATIC
    src/core/network.cpp
)

set_target_properties(network PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_options(network PRIVATE -fPIC)
target_include_directories(network PRIVATE src/core)

# Create Python module using Pybind11
pybind11_add_module(bindings src/bindings/bindings.cpp)

# Ensure bindings library is placed in the correct folder
set_target_properties(bindings PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

target_link_libraries(bindings PRIVATE network)
