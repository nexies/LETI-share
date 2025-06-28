cmake_minimum_required(VERSION 3.0)
project(Diamonds)

set(CMAKE_CXX_STANDARD 20)

add_executable(Diamonds diamond/main.cpp
        nx/nx_list.h
        nx/nx_vector.cpp
        nx/nx_vector.h
        nx/nx_string.h
        diamond/diamond_engine.cpp
        diamond/diamond_engine.h
        nx/nx_iostream.h
        nx/nx_iostream.cpp
        nx/nx_type_traits.h
        nx/logger.cpp
        nx/logger.h
        nx/nx_forward_list.h
        nx/nx_string_extension.cpp
        nx/nx_string_extension.h
)

add_executable(Lab2 lab2/main.cpp
        nx/nx_forward_list.h
        nx/marker_string.cpp
        nx/marker_string.h
)

add_executable(Lab1 lab1/main.cpp
        nx/marker_string.h
        nx/marker_string.cpp
)

target_include_directories(Diamonds PRIVATE ${PROJECT_SOURCE_DIR}/nx)
target_include_directories(Lab2     PRIVATE ${PROJECT_SOURCE_DIR}/nx)
target_include_directories(Lab1     PRIVATE ${PROJECT_SOURCE_DIR}/nx)

message("[${PROJECT_NAME}] : PROJECT_SOURCE_DIR -- ${PROJECT_SOURCE_DIR}")
