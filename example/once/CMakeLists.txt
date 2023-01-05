cmake_minimum_required(VERSION 3.24.1)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(once  
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)