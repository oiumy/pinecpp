cmake_minimum_required(VERSION 3.15)
project(pinecpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pinecpp
    src/main.cpp
    src/lexer/lexer.cpp
)

add_executable(test_lexer
    tests/test_lexer.cpp
    src/lexer/lexer.cpp
)
