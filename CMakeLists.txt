cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
project(bowling)

set(FLAGS -Wall -Wextra -pedantic)

add_executable(${PROJECT_NAME}-ut Tests.cpp catch_amalgamated.cpp)

target_compile_options(${PROJECT_NAME}-ut PRIVATE ${FLAGS})

enable_testing()
add_test(NAME Tests COMMAND ${PROJECT_NAME}-ut)