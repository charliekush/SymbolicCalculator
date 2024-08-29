cmake_minimum_required(VERSION 3.14)
project(SymbolicCalculator)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)





include_directories(src/)

set(CALCULATOR_SOURCE_FILES
    src/main.cpp
)


add_executable(symbolic ${CALCULATOR_SOURCE_FILES})

