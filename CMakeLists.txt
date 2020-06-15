cmake_minimum_required(VERSION 3.13.4)
project(stats_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(src
        lib/spdlog/include)

enable_testing()

add_library(stats STATIC
        src/engine.cpp
        src/engine.h
        src/handler.h
        src/datadog_handler.h
        src/datadog_handler.cpp
        src/print_handler.cpp
        src/print_handler.h
        src/stats.cpp
        src/stats.h
        src/datadog_udp_writer.h
        src/datadog_udp_writer.cpp
        src/tag.h
        src/measure.h
        src/metric_type.h
        src/writer.h)

add_subdirectory(test)
add_subdirectory(lib/googletest)
add_subdirectory(lib/spdlog)


