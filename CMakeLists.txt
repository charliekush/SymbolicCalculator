cmake_minimum_required(VERSION 3.14)
project(SymbolicCalculator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/googletest)
include_directories(external/googletest/googletest/include src/ tests)
include_directories(src/ tests/)

# Define the project source files once
set(PROJECT_SOURCE_FILES 
    src/MWT.cpp
    src/derivative.cpp      
    src/expression_tree.cpp
    src/lookup.cpp
    src/postfix.cpp
    src/token_container.cpp 
    src/token_stack.cpp     
    src/tokenizer.cpp
    src/arithmetic.cpp      
    src/expression_node.cpp 
    src/function_defs.cpp   
    src/token.cpp
    src/token_queue.cpp     
    src/token_vector.cpp
    src/latex_converter.cpp
    src/operation.cpp
    src/tree_fixer.cpp
    src/tree_modifier.cpp
    src/arithmetic.cpp
    src/text_converter.cpp
    src/log.cpp
    src/approx.cpp
)

# Create a static library for the common source files
add_library(symbolic_core STATIC ${PROJECT_SOURCE_FILES})

# Define the executable for the main project
add_executable(symbolic src/main.cpp)
target_link_libraries(symbolic symbolic_core)



# Define the source files for the tests
set(GTEST_SOURCE_FILES
    #tests/tokenizer_tests.cpp
    #tests/postfix_tests.cpp
    tests/expression_node_tests.cpp
)

# Create the test executable and link it against the library and gtest
add_executable(googletests ${GTEST_SOURCE_FILES})
target_link_libraries(googletests symbolic_core gtest gtest_main)

enable_testing()
add_test(NAME GoogleTests COMMAND googletests)
