cmake_minimum_required(VERSION 3.24.1)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(log 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(${PROJECT_NAME} 
            Boost::log
            Boost::log_setup)