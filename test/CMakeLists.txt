cmake_minimum_required(VERSION 3.13.4)
project(stats_cpp)

set(BINARY ${CMAKE_PROJECT_NAME}_tst)
set(TEST_SOURCES
        engine_test.cpp
        buffer_test.cpp
        test_handler.h
        main.cpp)

include_directories(../src
        ../lib/spdlog/include)

add_executable(${BINARY} ${TEST_SOURCES})
add_test(NAME ${BINARY} COMMAND ${BINARY})
target_link_libraries(${BINARY} PUBLIC stats gtest)