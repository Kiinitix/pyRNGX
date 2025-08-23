cmake_minimum_required(VERSION 3.18)
project(pyrngx LANGUAGES CXX)

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG QUIET)
if(NOT pybind11_FOUND)
  execute_process(COMMAND "${Python_EXECUTABLE}" -c "import pybind11, sys; sys.stdout.write(pybind11.get_cmake_dir())"
                  OUTPUT_VARIABLE pybind11_DIR)
  list(APPEND CMAKE_PREFIX_PATH "${pybind11_DIR}")
  find_package(pybind11 REQUIRED CONFIG)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

pybind11_add_module(_pyrngx
  src/cpp/bindings.cpp
  src/cpp/cpu_backend.cpp
)

target_include_directories(_pyrngx PRIVATE src/cpp)

if (MSVC)
  target_compile_options(_pyrngx PRIVATE /O2)
else()
  target_compile_options(_pyrngx PRIVATE -O3 -fno-fast-math)
endif()

install(TARGETS _pyrngx DESTINATION pyrngx)
