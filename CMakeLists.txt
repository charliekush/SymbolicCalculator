cmake_minimum_required(VERSION 3.14)
project(SymbolicCalculator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




add_subdirectory(external/googletest)
include_directories(external/googletest/googletest/include src/ tests)
include_directories(src/ tests/)

set(CALCULATOR_SOURCE_FILES
    src/main.cpp
    src/tokenizer.cpp
    src/token.cpp
    src/MWT.cpp
    src/postfix.cpp
    src/token_queue.cpp
)

set(GTEST_SOURCE_FILES
    src/tokenizer.cpp
    src/token.cpp
    src/MWT.cpp
    src/postfix.cpp
    src/token_queue.cpp
    tests/tokenizer_tests.cpp

    
)


add_executable(symbolic ${CALCULATOR_SOURCE_FILES})

add_executable(googletests ${GTEST_SOURCE_FILES})
target_link_libraries(googletests gtest gtest_main)

enable_testing()
add_test(NAME GoogleTests COMMAND googletests)
