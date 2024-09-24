cmake_minimum_required(VERSION 3.14)
project(SymbolicCalculator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/googletest)
include_directories(external/googletest/googletest/include src/ tests)
include_directories(src/ tests/)

# Define the project source files once
set(PROJECT_SOURCE_FILES 
    src/lookup.cpp
    src/tokenizer.cpp
    src/token.cpp
    src/MWT.cpp
    src/postfix.cpp
    src/token_queue.cpp
    src/token_container.cpp
    src/expression_node.cpp
    src/expression_tree.cpp
)

# Create a static library for the common source files
add_library(symbolic_core STATIC ${PROJECT_SOURCE_FILES})

# Define the executable for the main project
add_executable(symbolic src/main.cpp)
target_link_libraries(symbolic symbolic_core)

# Define the source files for the tests
set(GTEST_SOURCE_FILES
    tests/test_definitions.cpp
    tests/tokenizer_tests.cpp
    tests/postfix_tests.cpp
)

# Create the test executable and link it against the library and gtest
add_executable(googletests ${GTEST_SOURCE_FILES})
target_link_libraries(googletests symbolic_core gtest gtest_main)

enable_testing()
add_test(NAME GoogleTests COMMAND googletests)
