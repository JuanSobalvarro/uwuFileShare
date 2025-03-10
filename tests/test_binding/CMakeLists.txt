# Minimum required CMake version
cmake_minimum_required(VERSION 3.20)

# Project name
project(hello)

# Set C++ standard (we'll use C++23 as per your setup)
set(CMAKE_CXX_STANDARD 23)

# Find the Python3 installation (Interpreter and Development tools)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Include the pybind11 package. This assumes pybind11 is installed (e.g., via pip)
# Or, you can point CMake to the location where pybind11 is installed
find_package(pybind11 REQUIRED)

# Add the source file and specify that we are creating a shared module
add_library(hello MODULE hello.cpp)

# Link the pybind11 module with the Python libraries
target_link_libraries(hello PRIVATE pybind11::module Python3::Python)

# Set the output location for the module (optional, but helpful for debugging)
set_target_properties(hello PROPERTIES
    PREFIX ""
    SUFFIX ".so"
)

# Optionally specify where the compiled module should be placed
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

message(STATUS "Library output directory: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
